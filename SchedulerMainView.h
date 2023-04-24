#ifndef SCHEDULERMAINVIEW_H
#define SCHEDULERMAINVIEW_H

#include <QWidget>
#include "ui_SchedulerMainView.h"
#include "ScheduledTask.h"

namespace Ui {
class SchedulerMainView;
}

class SchedulerMainView : public QWidget
{
    Q_OBJECT

public:
    explicit SchedulerMainView(QWidget *parent = nullptr);
    ~SchedulerMainView();

private slots:
    void on_save_clicked();
    void AddTabSlot();
    void AboutSlot();

private:
    Ui::SchedulerMainView *ui;

    std::vector<ScheduledTask*> tasks;

    void AddTask(ScheduledTask*);
};

#endif // SCHEDULERMAINVIEW_H
