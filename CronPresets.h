#ifndef CRONPRESETS_H
#define CRONPRESETS_H

#include <string>
#include <map>

static const std::map<std::string, std::string> CRON_PRESETS = {
    {"Every 1 second","* * * * * ?"},
    {"Every 2 second","*/2 * * * * ?"},
    {"Every 3 second","*/3 * * * * ?"},
    {"Every 5 second","*/5 * * * * ?"},
    {"Every 10 second","*/10 * * * * ?"},
    {"Every minute", "0 * * * * ?"},
    {"Every hour", "* 0 * * * ?"},
};

#endif // CRONPRESETS_H
