#include "panel.h"
#include "ui_panel.h"

#include "rep_radio_replica.h"

#include <QDebug>

Panel::Panel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Panel)
{
    ui->setupUi(this);

    /*QRemoteObjectNode *node = new QRemoteObjectNode(this);
    if(!node->connectToNode(QUrl(QStringLiteral("local:/tmp/wsdr")))) {
        qDebug() << "some horrible error";
    }
    radio.reset(node->acquire<RadioReplica>());*/
}

Panel::~Panel()
{
    delete ui;
}

void Panel::on_pushButton_clicked()
{
    qDebug("hello");
    //radio->pushFrequency(12300);
}
