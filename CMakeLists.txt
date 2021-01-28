cmake_minimum_required(VERSION 3.17)
project(ak3_m135_christidis)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(ak3_m135_christidis
        main.cpp
        behaviour_test.cpp behaviour_test.h
        queue.cpp queue.h
        subscriber.cpp subscriber.h
        publisher.cpp publisher.h
        flowable.cpp flowable.h
        mem_investigation.cpp mem_investigation.h
        simple/B.h simple/C.h simple/D.h)