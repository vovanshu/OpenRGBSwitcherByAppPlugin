#ifndef SCHEDULERSETTINGS_H
#define SCHEDULERSETTINGS_H

#include "json.hpp"
#include "filesystem.h"

using json = nlohmann::json;

class SchedulerSettings
{
public:
    static void Save(json);
    static json Load();

private:
    static bool CreateSettingsDirectory();
    static filesystem::path SettingsFolder();
};

#endif // SCHEDULERSETTINGS_H
