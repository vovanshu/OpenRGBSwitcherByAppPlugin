#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <QString>
#include <iostream>
#include <iomanip>
#include <chrono>
#include "libcron/Cron.h"

using Cron = libcron::Cron<>;

class TimeUtils{

public:

    static QString nextRun(Cron& cron)
    {
        return QString::fromStdString(
                    beautify_duration(
                        std::chrono::duration_cast<std::chrono::seconds>(
                            cron.time_until_next())));
    }

    static std::string beautify_duration(std::chrono::seconds input_seconds)
    {
        using namespace std::chrono;
        typedef duration<int, std::ratio<86400>> days;
        auto d = duration_cast<days>(input_seconds);
        input_seconds -= d;
        auto h = duration_cast<hours>(input_seconds);
        input_seconds -= h;
        auto m = duration_cast<minutes>(input_seconds);
        input_seconds -= m;
        auto s = duration_cast<seconds>(input_seconds);

        auto dc = d.count();
        auto hc = h.count();
        auto mc = m.count();
        auto sc = s.count();

        std::stringstream ss;
        ss.fill('0');
        if (dc) {
            ss << d.count() << "d";
        }
        if (dc || hc) {
            if (dc) { ss << std::setw(2); } //pad if second set of numbers
            ss << h.count() << "h";
        }
        if (dc || hc || mc) {
            if (dc || hc) { ss << std::setw(2); }
            ss << m.count() << "m";
        }

        if (dc || hc || mc || sc) {
            if (dc || hc || mc) { ss << std::setw(2); }
            ss << s.count() << 's';
        }

        std::string str = ss.str();

        if(str.empty())
        {
            return "0s";
        }

        return str;
    }
};

#endif // TIMEUTILS_H
