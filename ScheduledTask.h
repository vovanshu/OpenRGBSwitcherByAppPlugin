#ifndef SCHEDULEDTASK_H
#define SCHEDULEDTASK_H

#include <QWidget>
#include "libcron/Cron.h"
#include <chrono>
#include <thread>
#include "json.hpp"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

using json = nlohmann::json;
using Cron = libcron::Cron<>;

namespace Ui {
class ScheduledTask;
}

class ScheduledTask : public QWidget
{
    Q_OBJECT

public:
    explicit ScheduledTask(QWidget *parent = nullptr);
    ~ScheduledTask();

    void Start();
    void Stop();
    bool IsRunning();

    json ToJson();
    static ScheduledTask* FromJson(QWidget *parent, json);

    std::string GetName();
    void Rename(std::string);

    std::chrono::time_point<std::chrono::system_clock> GetNextRun();
    void RecalculateNextRun();

    void TaskFunction();
signals:
    void Renamed(std::string);
    void TaskState(bool);
    void NextRunChanged();

private slots:
    void on_reload_sub_actions_clicked();
    void on_reload_sub_actions2_clicked();
    void on_cron_line_editingFinished();
    void on_action_currentIndexChanged(int);
    void on_action2_currentIndexChanged(int);
    void on_cron_presets_currentIndexChanged(int);

private:
    Ui::ScheduledTask *ui;

    Cron cron;

    std::thread* thread = nullptr;

    bool run = false;
    std::string name = "Untitled";
    std::chrono::time_point<std::chrono::system_clock> next_run{std::chrono::system_clock::duration{0}};

    void ThreadFunction();
    void ReloadSubActions();
    void ReloadSubActions2();
    void ReloadCron();
    std::string GetProcessName( DWORD processID );
    int cheackappslist( void );
    bool IsForegroundProcess(std::string nameapp);
};


#endif // SCHEDULEDTASK_H
