#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "factory.h"
#include "subscriber.h"

int main(int argc, char** argv){
    int SizeVector = std::atoi(argv[1]); //размер вектора
    std::vector<std::shared_ptr<figure>> Figure; //вектор-буфер для хранения фигур
    std::condition_variable k1;// примитивы синхронизации
    std::condition_variable k2;
    std::mutex mutex;

    factory Factory; // фабрика создания фигур

    bool done = false;
    char cmd;
    int in = 1;
    std::vector<std::shared_ptr<Subscriber>> subs;// вектор с обработчиками
    subs.push_back(std::make_shared<Consol>());
    subs.push_back(std::make_shared<File>());
    std::thread subscriber([&]() {
        std::unique_lock<std::mutex> subscriber_lock(mutex);// универсальная оболочка для владения мьютексом, поток-обработчиков
        while(!done) {
            k1.wait(subscriber_lock);// блокирует текущий поток до тех пор, пока переменная не будет пробужена

            if (done) {
                k2.notify_all(); // уведомляет все потоки ожидающие k2
                break;
            }
            for (unsigned int i = 0; i < subs.size(); ++i) {
                subs[i]->output(Figure);
            }
            in++;
            Figure.resize(0);
            k2.notify_all();
        }
    });
    while(cmd != 'q') {
        std::cout << "'q'-quit, 'c'-continue , Figures: square, trapez, rectangle" << std::endl;
        std::cin >> cmd;
        if (cmd != 'q') {
            std::unique_lock<std::mutex> main_lock(mutex); // главный поток
            for (int i = 0; i < SizeVector; i++) {
                Figure.push_back(Factory.FigureCreate(std::cin));
                std::cout << "Added" << std::endl;
            }
            k1.notify_all();
            k2.wait(main_lock);
        }
    }
    done = true;
    k1.notify_all();
    subscriber.join(); //Блокирует текущий поток до тех пор, пока поток, обозначенный *this, не завершит свое выполнение
    return 0;
}