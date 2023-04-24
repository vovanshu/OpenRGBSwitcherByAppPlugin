#include "ScheduledTask.h"
#include "ui_ScheduledTask.h"
#include "TimeUtils.h"
#include "OpenRGBSwitcherByAppPlugin.h"
#include "CronPresets.h"
#include <chrono>
#include <cstdio>
#include <exception>
#include <functional>

#include <QMenu>
#include <QAction>
#include <QMainWindow>

ScheduledTask::ScheduledTask(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScheduledTask)
{
    ui->setupUi(this);

    ui->cron_presets->addItem("Choose a preset...");

    for (auto& preset : CRON_PRESETS)
    {
        ui->cron_presets->addItem(QString::fromStdString(preset.first));
    }

    ui->action->addItems({"Load profile", "Lights Off", "Effects plugin action"});

    ui->action2->addItems({"Load profile", "Lights Off", "Effects plugin action"});

    ReloadSubActions();
    ReloadSubActions2();
}

ScheduledTask::~ScheduledTask()
{
    Stop();
    delete ui;
}

void ScheduledTask::TaskFunction()
{

    if( cheackappslist() == 1 ){

        int action_index = ui->action->currentIndex();

        if(!ui->check_action->isChecked()){
            switch (action_index) {
                case 0:
                {
                    std::string profile = ui->sub_action->currentText().toStdString();
                    printf("Scheduled task run: load profile [%s]\n", profile.c_str());

                    OpenRGBSwitcherByAppPlugin::RMPointer->GetProfileManager()->LoadProfile(profile);

                    for(RGBController* controller : OpenRGBSwitcherByAppPlugin::RMPointer->GetRGBControllers())
                    {
                        controller->UpdateLEDs();
                    }

                    break;
                }
                case 1:
                {
                    for(RGBController* controller : OpenRGBSwitcherByAppPlugin::RMPointer->GetRGBControllers())
                    {
                        controller->SetAllLEDs(ToRGBColor(0,0,0));
                        controller->UpdateLEDs();
                    }
                    break;
                }
                case 2:
                {
                    QString object_name = ui->sub_action->currentData().toString();

                    for (QWidget *w : QApplication::topLevelWidgets())
                    {
                        if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
                        {
                            QAction* action = mainWin->findChild<QAction*>(object_name);

                            if(action)
                            {
                                action->trigger();
                            }
                        }
                    }

                }
                default: break;
            }
            ui->check_action2->setChecked(0);
            ui->check_action->setChecked(1);
            ui->statusapps->setText("Execute action for application is foreground");
        }else{
            ui->statusapps->setText("Already executed action for application is foreground");
        }
    }else{
        int action_index2 = ui->action2->currentIndex();
        if(!ui->check_action2->isChecked()){
            switch (action_index2) {
                case 0:
                {
                    std::string profile = ui->sub_action2->currentText().toStdString();
                    printf("Scheduled task run: load profile [%s]\n", profile.c_str());

                    OpenRGBSwitcherByAppPlugin::RMPointer->GetProfileManager()->LoadProfile(profile);

                    for(RGBController* controller : OpenRGBSwitcherByAppPlugin::RMPointer->GetRGBControllers())
                    {
                        controller->UpdateLEDs();
                    }

                    break;
                }
                case 1:
                {
                    for(RGBController* controller : OpenRGBSwitcherByAppPlugin::RMPointer->GetRGBControllers())
                    {
                        controller->SetAllLEDs(ToRGBColor(0,0,0));
                        controller->UpdateLEDs();
                    }
                    break;
                }
                case 2:
                {
                    QString object_name = ui->sub_action2->currentData().toString();

                    for (QWidget *w : QApplication::topLevelWidgets())
                    {
                        if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
                        {
                            QAction* action = mainWin->findChild<QAction*>(object_name);

                            if(action)
                            {
                                action->trigger();
                            }
                        }
                    }

                }
                default: break;
            }
            ui->check_action->setChecked(0);
            ui->check_action2->setChecked(1);
            ui->statusapps->setText("Execute action for application is not foreground");
        }else{
            ui->statusapps->setText("Already executed action for application is not foreground");
        }
    }

    this->cron.recalculate_schedule();
    RecalculateNextRun();
}

void ScheduledTask::Start()
{
    if(thread == nullptr)
    {

        std::function<void(const libcron::TaskInformation&)> callback = [=] (auto&) {
            TaskFunction();
        };

        bool added = cron.add_schedule(name, ui->cron_line->text().toStdString(), callback);

        if(!added)
        {
            ui->cron_line->setStyleSheet("border:2px solid red");
            return;
        }

        ui->cron_line->setStyleSheet("");

        RecalculateNextRun();

        run = true;
        thread = new std::thread(&ScheduledTask::ThreadFunction, this);

        emit TaskState(true);
    }
}

void ScheduledTask::Stop()
{
    cron.clear_schedules();

    if(thread != nullptr)
    {
        run = false;
        thread->join();
        delete thread;
        thread = nullptr;
    }

    ui->next_run->clear();

    emit TaskState(false);
}

bool ScheduledTask::IsRunning()
{
    return run;
}

void ScheduledTask::ThreadFunction()
{
    printf("[OpenRGBVisualMapPlugin] Task thread started\n");

    while(run)
    {
        cron.tick();
        ui->next_run->setText(TimeUtils::nextRun(cron));
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    printf("[OpenRGBVisualMapPlugin] Task thread stopped\n");
}

void ScheduledTask::ReloadCron()
{
    bool was_running = run;

    Stop();

    if(was_running)
    {
        Start();
    }
}

void ScheduledTask::on_cron_line_editingFinished()
{
    ReloadCron();
}


void ScheduledTask::on_reload_sub_actions_clicked()
{
    ReloadSubActions();
}

void ScheduledTask::on_reload_sub_actions2_clicked()
{
    ReloadSubActions2();
}

void ScheduledTask::on_action_currentIndexChanged(int value)
{
    ui->sub_action->setVisible(value == 0|| value == 2);
    ui->reload_sub_actions->setVisible(value == 0 || value == 2);

    ReloadSubActions();
}

void ScheduledTask::on_action2_currentIndexChanged(int value)
{
    ui->sub_action2->setVisible(value == 0|| value == 2);
    ui->reload_sub_actions2->setVisible(value == 0 || value == 2);

    ReloadSubActions2();
}

void ScheduledTask::on_cron_presets_currentIndexChanged(int value)
{
    if(value == 0)
    {
        return;
    }

    ui->cron_line->setText(QString::fromStdString(
        CRON_PRESETS.at(ui->cron_presets->currentText().toStdString())
    ));

    ReloadCron();
}

json ScheduledTask::ToJson()
{
    json j;

    j["cron_line"] = ui->cron_line->text().toStdString();
    j["run"] = run;
    j["action"] = ui->action->currentIndex();
    j["action2"] = ui->action2->currentIndex();
    j["name"] = name;
    j["next_run"] = next_run.time_since_epoch().count();

    switch (ui->action->currentIndex()) {
    case 0:
    case 2:
    {
        j["sub_action"] = ui->sub_action->currentText().toStdString();

        break;
    }
    default: break;
    }

    switch (ui->action2->currentIndex()) {
    case 0:
    case 2:
    {
        j["sub_action2"] = ui->sub_action2->currentText().toStdString();

        break;
    }
    default: break;
    }

    j["appslist"] = ui->appslist->toPlainText().toStdString();

    return j;
}

ScheduledTask* ScheduledTask::FromJson(QWidget *parent, json j)
{
    ScheduledTask* task = new ScheduledTask(parent);

    if(j.contains("cron_line"))
        task->ui->cron_line->setText(QString::fromStdString(j["cron_line"]));

    if(j.contains("action"))
        task->ui->action->setCurrentIndex(j["action"]);

    if(j.contains("sub_action"))
        task->ui->sub_action->setCurrentText(QString::fromStdString(j["sub_action"]));

    if(j.contains("action2"))
        task->ui->action2->setCurrentIndex(j["action2"]);

    if(j.contains("sub_action2"))
        task->ui->sub_action2->setCurrentText(QString::fromStdString(j["sub_action2"]));

    if(j.contains("appslist"))
        task->ui->appslist->setPlainText(QString::fromStdString(j["appslist"]));

    if(j.contains("next_run"))
    {
        std::chrono::system_clock::time_point next_run_from_epoch {std::chrono::system_clock::duration{j["next_run"]}};
        task->next_run = next_run_from_epoch;
    }

    if(j.contains("name"))
    {
        task->name = j["name"];
    }

    if(j.contains("run") && j["run"])
    {
        task->Start();
    }

    return task;
}

std::string ScheduledTask::GetName()
{
    return name;
}

void ScheduledTask::Rename(std::string value)
{
    name = value;
    emit Renamed(name);
}

std::chrono::time_point<std::chrono::system_clock> ScheduledTask::GetNextRun()
{
    return next_run;
}

void ScheduledTask::RecalculateNextRun()
{
    auto new_next_run = std::chrono::system_clock::now() + cron.time_until_next();
    if (std::chrono::duration_cast<std::chrono::seconds>(new_next_run - next_run).count() != 0)
    {
        next_run = new_next_run;
        emit NextRunChanged();
    }
}

void ScheduledTask::ReloadSubActions()
{
    ui->sub_action->clear();

    int action_index = ui->action->currentIndex();

    switch (action_index) {
    case 0:
    {
        std::vector<std::string> profiles = OpenRGBSwitcherByAppPlugin::RMPointer->GetProfileManager()->profile_list;

        for(std::string& profile: profiles)
        {
            ui->sub_action->addItem(QString::fromStdString(profile));
        }

        break;
    }
    case 2:
    {
        QRegularExpression action_filter("^OpenRGBEffectsPlugin::Action::");

        for (QWidget *w : QApplication::topLevelWidgets())
        {
            if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
            {
                QList<QAction*> profilesActions = mainWin->findChildren<QAction*>(action_filter);

                for(const QAction* action : profilesActions)
                {
                    ui->sub_action->addItem(action->property("OpenRGBEffectsPlugin::ActionTitle").toString(), action->objectName());
                }
            }
        }
        break;
    }
    default: break;
    }
}

void ScheduledTask::ReloadSubActions2()
    {
    ui->sub_action2->clear();

    int action_index2 = ui->action2->currentIndex();

    switch (action_index2) {
    case 0:
    {
        std::vector<std::string> profiles = OpenRGBSwitcherByAppPlugin::RMPointer->GetProfileManager()->profile_list;

        for(std::string& profile: profiles)
        {
            ui->sub_action2->addItem(QString::fromStdString(profile));
        }

        break;
    }
    case 2:
    {
        QRegularExpression action_filter("^OpenRGBEffectsPlugin::Action::");

        for (QWidget *w : QApplication::topLevelWidgets())
        {
            if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
            {
                QList<QAction*> profilesActions = mainWin->findChildren<QAction*>(action_filter);

                for(const QAction* action : profilesActions)
                {
                    ui->sub_action2->addItem(action->property("OpenRGBEffectsPlugin::ActionTitle").toString(), action->objectName());
                }
            }
        }
        break;
    }
    default: break;
    }

}

std::string ScheduledTask::GetProcessName( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    CloseHandle( hProcess );

    std::string AppProcessName;
    for(int i=0; i<sizeof(szProcessName); i++)
        AppProcessName.push_back( (char) szProcessName[i]);

    return AppProcessName;
}

int ScheduledTask::cheackappslist( void )
{

    ui->statusapps->setText("Checking application...");

    QString vapp = ui->appslist->toPlainText();
    if(vapp == ""){
        ui->statusapps->setText("Wait Applications list!");
        return 0;
    }
    QStringList lapp = vapp.split("\n");

    for (int i = 0; i < lapp.size(); ++i)
    {

        if(IsForegroundProcess(lapp.at(i).toLocal8Bit().constData()))
        {
            ui->statusapps->setText("Application foreground.");
            return 1;
        }

    }

    ui->statusapps->setText("Application no foreground.");
    return 0;
}

bool ScheduledTask::IsForegroundProcess(std::string nameapp)
{
    HWND hwnd = GetForegroundWindow();
    if (hwnd == NULL) return false;

    DWORD foregroundPid;
    if (GetWindowThreadProcessId(hwnd, &foregroundPid) == 0) return false;

    if (GetProcessName(foregroundPid).c_str() == nameapp)
        return true;
   return false;
}
