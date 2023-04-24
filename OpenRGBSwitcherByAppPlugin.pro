QT +=                  \
    gui                \
    widgets            \
    core               \

win32:CONFIG += QTPLUGIN

TEMPLATE = lib
DEFINES += ORGBSCHEDULERPLUGIN_LIBRARY

win32:CONFIG += c++17

unix:!macx {
  QMAKE_CXXFLAGS += -std=c++17
}

#-----------------------------------------------------------------------------------------------#
# Application Configuration                                                                     #
#-----------------------------------------------------------------------------------------------#
PLUGIN_VERSION     = 0.81

#-----------------------------------------------------------------------------------------------#
# Automatically generated build information                                                     #
#-----------------------------------------------------------------------------------------------#
win32:BUILDDATE = $$system(date /t)
unix:BUILDDATE  = $$system(date -R -d "@${SOURCE_DATE_EPOCH:-$(date +%s)}")
GIT_COMMIT_ID   = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse HEAD)
GIT_COMMIT_DATE = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ show -s --format=%ci HEAD)
GIT_BRANCH      = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse --abbrev-ref HEAD)

win32:LATEST_BUILD_URL="https://gitlab.com/OpenRGBDevelopers/OpenRGBSwitcherbyappPlugin/-/jobs/artifacts/master/download?job=Windows 64"
unix:!macx:LATEST_BUILD_URL="https://gitlab.com/OpenRGBDevelopers/OpenRGBSwitcherbyappPlugin/-/jobs/artifacts/master/download?job=Linux 64"

#-----------------------------------------------------------------------------------------------#
# Inject vars in defines                                                                        #
#-----------------------------------------------------------------------------------------------#
DEFINES +=                                                                                      \
    VERSION_STRING=\\"\"\"$$PLUGIN_VERSION\\"\"\"                                               \
    BUILDDATE_STRING=\\"\"\"$$BUILDDATE\\"\"\"                                                  \
    GIT_COMMIT_ID=\\"\"\"$$GIT_COMMIT_ID\\"\"\"                                                 \
    GIT_COMMIT_DATE=\\"\"\"$$GIT_COMMIT_DATE\\"\"\"                                             \
    GIT_BRANCH=\\"\"\"$$GIT_BRANCH\\"\"\"                                                       \
    LATEST_BUILD_URL=\\"\"\"$$LATEST_BUILD_URL\\"\"\"                                           \

#-------------------------------------------------------------------#
# Includes                                                          #
#-------------------------------------------------------------------#
INCLUDEPATH +=                                                      \
    OpenRGB/                                                        \
    OpenRGB/i2c_smbus                                               \
    OpenRGB/RGBController                                           \
    OpenRGB/net_port                                                \
    OpenRGB/hidapi_wrapper                                          \
    OpenRGB/dependencies/json                                       \
    OpenRGB/dependencies/hidapi                                     \


HEADERS +=                                                          \
    CronPresets.h                                                   \
    OpenRGB/OpenRGBPluginInterface.h                                \
    OpenRGB/ResourceManager.h                                       \
    OpenRGBPluginsFont.h \
    OpenRGBSwitcherByAppPlugin.h \
    ScheduledTask.h                                                 \
    SchedulerMainView.h                                             \
    SchedulerSettings.h                                             \
    TimeUtils.h                                                     \
    PluginInfo.h                                                    \
    TabHeader.h                                                     \
    EditableLabel.h                                                 \

SOURCES +=                                                          \
    OpenRGBPluginsFont.cpp \
    OpenRGBSwitcherByAppPlugin.cpp \
    ScheduledTask.cpp                                               \
    SchedulerMainView.cpp                                           \
    SchedulerSettings.cpp                                           \
    PluginInfo.cpp                                                  \
    TabHeader.cpp                                                   \
    EditableLabel.cpp                                               \


FORMS +=                                                            \
    ScheduledTask.ui                                                \
    SchedulerMainView.ui                                            \
    PluginInfo.ui                                                   \
    TabHeader.ui                                                    \

#-------------------------------------------------------------------#
# Libcron                                                           #
#-------------------------------------------------------------------#
INCLUDEPATH +=                                                      \
    dependencies/libcron/libcron/include                            \
    dependencies/date/include                                       \

SOURCES +=                                                          \
    dependencies/libcron/libcron/src/CronClock.cpp                  \
    dependencies/libcron/libcron/src/CronData.cpp                   \
    dependencies/libcron/libcron/src/CronRandomization.cpp          \
    dependencies/libcron/libcron/src/CronSchedule.cpp               \
    dependencies/libcron/libcron/src/Task.cpp                       \

#-------------------------------------------------------------------#
# Windows GitLab CI Configuration                                   #
#-------------------------------------------------------------------#
win32:CONFIG(debug, debug|release) {
    win32:DESTDIR = debug
}

win32:CONFIG(release, debug|release) {
    win32:DESTDIR = release
}

win32:OBJECTS_DIR = _intermediate_$$DESTDIR/.obj
win32:MOC_DIR     = _intermediate_$$DESTDIR/.moc
win32:RCC_DIR     = _intermediate_$$DESTDIR/.qrc
win32:UI_DIR      = _intermediate_$$DESTDIR/.ui

win32:contains(QMAKE_TARGET.arch, x86_64) {
    LIBS +=                                                             \
        -lws2_32                                                        \
        -lole32                                                         \
}

win32:contains(QMAKE_TARGET.arch, x86) {
    LIBS +=                                                             \
        -lws2_32                                                        \
        -lole32                                                         \
}

win32:DEFINES +=                                                        \
    _MBCS                                                               \
    WIN32                                                               \
    _CRT_SECURE_NO_WARNINGS                                             \
    _WINSOCK_DEPRECATED_NO_WARNINGS                                     \
    WIN32_LEAN_AND_MEAN                                                 \

#-----------------------------------------------------------------------#
# Linux-specific Configuration                                          #
#-----------------------------------------------------------------------#
unix:!macx {
}

#-----------------------------------------------------------------------#
# MacOS-specific Configuration                                          #
#-----------------------------------------------------------------------#
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

macx: {
    CONFIG += c++17
}

RESOURCES += \
        resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64/' -lUser32
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64/' -lUser32d
else:unix: LIBS += -L'C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64/' -lUser32

INCLUDEPATH += 'C:/Program Files (x86)/Windows Kits/10/Include/10.0.22000.0/um'
DEPENDPATH += 'C:/Program Files (x86)/Windows Kits/10/Include/10.0.22000.0/um'
