#ifndef SUBSCRIBERS_H
#define SUBSCRIBERS_H
#include <fstream>

class Subscriber{
public:
    virtual void output(std::vector<std::shared_ptr<figure>>& Vec) = 0;
    virtual ~Subscriber()() = default;
};

class Consol : public Subscriber {
public:
    void output(std::vector<std::shared_ptr<figure>>& Vec) override {
        for (auto& figure : Vec) {
            figure->print(std::cout);
        }
    }
};

class File : public Subscriber{
public:
    File() : in(1) {}
    void output(std::vector<std::shared_ptr<figure>>& Vec) override  {
        std::string filename;
        filename = std::to_string(in);
        filename += ".txt";
        std::ofstream file;
        file.open(filename);
        for (auto &figure : Vec) {
            figure->print(file);
        }
        in++;
    }
private :
    int in;
};



#endif