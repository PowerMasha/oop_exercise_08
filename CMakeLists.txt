cmake_minimum_required(VERSION 3.10.2)
project(oop_exercise_08)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g3 -Wextra -pthread")
add_executable(oop_exercise_08
        main.cpp
        point.h
        trapez.h
        figure.h
        rectangle.h
        square.h
        factory.h
        subscriber.h)
