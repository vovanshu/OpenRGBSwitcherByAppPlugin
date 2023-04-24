#include "OpenRGBSwitcherByAppPlugin.h"
#include "SchedulerMainView.h"
#include <QHBoxLayout>

bool OpenRGBSwitcherByAppPlugin::DarkTheme = false;
ResourceManager* OpenRGBSwitcherByAppPlugin::RMPointer = nullptr;

OpenRGBPluginInfo OpenRGBSwitcherByAppPlugin::GetPluginInfo()
{
    printf("[OpenRGBSwitcherByAppPlugin] Loading plugin info.\n");

    OpenRGBPluginInfo info;

    info.Name           = "Switcher By App Plugin";
    info.Description    = "Switcher By App Plugin";
    info.Version        = VERSION_STRING;
    info.Commit         = GIT_COMMIT_ID;
    info.URL            = "";

    info.Icon.load(":/OpenRGBSwitcherByAppPlugin.png");

    info.Location       =  OPENRGB_PLUGIN_LOCATION_TOP;
    info.Label          =  "SwitcherByApp";
    info.TabIconString  =  "SwitcherByApp";

    info.TabIcon.load(":/OpenRGBSwitcherByAppPlugin.png");

    return info;
}

unsigned int OpenRGBSwitcherByAppPlugin::GetPluginAPIVersion()
{
    printf("[OpenRGBSwitcherByAppPlugin] Loading plugin API version.\n");

    return OPENRGB_PLUGIN_API_VERSION;
}

void OpenRGBSwitcherByAppPlugin::Load(bool dark_theme, ResourceManager* resource_manager_ptr)
{
    printf("[OpenRGBSwitcherByAppPlugin] Loading plugin.\n");

    RMPointer = resource_manager_ptr;
    DarkTheme = dark_theme;
}

QWidget* OpenRGBSwitcherByAppPlugin::GetWidget()
{
    printf("[OpenRGBSwitcherByAppPlugin] Creating widget.\n");

    return new SchedulerMainView();
}

QMenu* OpenRGBSwitcherByAppPlugin::GetTrayMenu()
{
    printf("[OpenRGBSwitcherByAppPlugin] Creating tray menu.\n");

    return nullptr;
}

void OpenRGBSwitcherByAppPlugin::Unload()
{
    printf("[OpenRGBSwitcherByAppPlugin] Time to call some cleaning stuff.\n");
}

OpenRGBSwitcherByAppPlugin::~OpenRGBSwitcherByAppPlugin()
{
    printf("[OpenRGBSwitcherByAppPlugin] Time to free some memory.\n");
}
