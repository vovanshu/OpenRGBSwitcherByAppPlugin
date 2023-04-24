#include "SchedulerSettings.h"
#include <fstream>
#include <iostream>
#include <QFile>
#include <QString>
#include <QDir>
#include "OpenRGBSwitcherByAppPlugin.h"

void SchedulerSettings::Save(json Settings)
{
    if(!CreateSettingsDirectory())
    {
        printf("Cannot create settings directory.\n");
        return;
    }

    std::ofstream File((SettingsFolder() / "SwitcherByAppSettings.json"), std::ios::out | std::ios::binary);

    if(File)
    {
        try{
            File << Settings.dump(4);
        }
        catch(const std::exception& e)
        {
            printf("Cannot write settings: %s\n", e.what());
        }
        File.close();
    }
}

json SchedulerSettings::Load()
{
    json Settings;

    std::ifstream SFile(SettingsFolder() / "SwitcherByAppSettings.json", std::ios::in | std::ios::binary);

    if(SFile)
    {
        try
        {
            SFile >> Settings;
            SFile.close();
        }
        catch(const std::exception& e)
        {
            printf("Cannot read settings: %s\n", e.what());
        }
    }

    return Settings;
}

bool SchedulerSettings::CreateSettingsDirectory()
{
    filesystem::path directory = SettingsFolder();

    QDir dir(QString::fromStdString(directory.string()));

    if(dir.exists())
    {
        return true;
    }

    return QDir().mkpath(dir.path());
}

filesystem::path SchedulerSettings::SettingsFolder()
{
    return OpenRGBSwitcherByAppPlugin::RMPointer->GetConfigurationDirectory() / "plugins" / "settings";
}
