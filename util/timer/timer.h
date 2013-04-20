/**
    @brief Timer class. Used for timing processes.

    Simple timer class.

    @author Aman Orazaev
*/
#pragma once
#include <stdlib.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/microsec_time_clock.hpp>
#include <boost/date_time/time_duration.hpp>

class TTimer {
    boost::posix_time::ptime start;
    boost::posix_time::ptime end;

    public:
    TTimer()
        : start()
        , end() {
    }

    void Start() {
        start = boost::posix_time::microsec_clock::local_time();
    }

    void Stop() {
        end = boost::posix_time::microsec_clock::local_time();
    }

    double GetSeconds() const {
    boost::posix_time::time_duration diff = end -start;
        return static_cast<double>(diff.total_milliseconds() / 1000.);
    }
};
