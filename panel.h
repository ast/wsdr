#ifndef PANEL_H
#define PANEL_H

#include <QWidget>

#include <QtRemoteObjects>
#include "rep_radio_replica.h"

namespace Ui {
class Panel;
}

class Panel : public QWidget
{
    Q_OBJECT
    QSharedPointer<RadioReplica> radio;

public:
    explicit Panel(QWidget *parent = 0);
    ~Panel();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Panel *ui;
};

#endif // PANEL_H
