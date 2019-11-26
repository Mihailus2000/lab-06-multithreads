// Copyright 2018 Your Name <your_email>

#include <thread>
#include <cstdlib>
#include <boost/log/trivial.hpp>
#include <csignal>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup.hpp>
#include "../third-party/PicoSHA2/picosha2.h"
#include <chrono>
#include <boost/log/detail/trivial_keyword.hpp>

class HashResolver{
public:
    HashResolver() {
        _numOfThreads = std::thread::hardware_concurrency();
        _arrayOfThreads.reserve(_numOfThreads);

        Init();

        for(size_t index = 0; index < _numOfThreads; index++){
            _arrayOfThreads.emplace_back(&HashResolver::Resolve,this);
        }
        BOOST_LOG_TRIVIAL(trace) << "Number of threads = " << _numOfThreads;

        for(auto &thread : _arrayOfThreads){
            thread.join();
        }
    }

private:
    std::vector<std::thread> _arrayOfThreads;
    size_t _numOfThreads;

    void Init(){
        boost::log::register_simple_formatter_factory<
                boost::log::trivial::severity_level,
                char
        >("Severity");
//    static const std::string format = "[%TimeStamp%][%Severity%][%ThreadID%]: %Message%";

        auto addFileLog = boost::log::add_file_log(
                boost::log::keywords::file_name = "logs/log_%N.log",
                boost::log::keywords::rotation_size = 128 * 1024 * 1024
        );
        addFileLog->set_filter(
                boost::log::trivial::severity >= boost::log::trivial::trace
        );          // Log file setup

        auto addConsoleLog = boost::log::add_console_log(
                std::cout);
        addConsoleLog->set_filter(
                boost::log::trivial::severity >= boost::log::trivial::info
        );      // Log console setup

        boost::log::add_common_attributes();

        srand(time(nullptr));
    }
    void Resolve(){
        static const std::string expectedEnd = "0000";
        const size_t expectedEndSize = expectedEnd.size();

        while (true) {
            std::string randomString = std::to_string(std::rand());
            std::string result = picosha2::hash256_hex_string(randomString);

            if (result.substr(result.size() - expectedEndSize) == expectedEnd) {
                BOOST_LOG_TRIVIAL(info) << "Found correct value '" << randomString << "' with hash '" << result << "'";
            } else {
                BOOST_LOG_TRIVIAL(trace) << "Current value '" << randomString << "' with hash '" << result << "'";
            }
        }
    }
};




int main(){
    srand(time(nullptr));

    HashResolver exs;

    return 0;

}