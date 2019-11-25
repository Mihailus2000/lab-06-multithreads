// Copyright 2018 Your Name <your_email>
#define BOOST_LOG_DYN_LINK 1

#include <boost/log/trivial.hpp>
#include <thread>
#include <vector>
#include "PicoSHA2/picosha2.h"


void Resolve(){
    BOOST_LOG_TRIVIAL(info) << "Found correct value '";
}

int main(){
//    size_t numOfThreads = std::thread::hardware_concurrency();
//    std::vector<std::thread> arrayOfThreads;
    Resolve();

    return 0;

}