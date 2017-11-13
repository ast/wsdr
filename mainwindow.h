#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QUdpSocket>
#include <QLocalSocket>

#include "glspectrum.h"
#include "glwaterfall.h"
#include "panel.h"
#include <QtRemoteObjects>
#include "rep_radio_replica.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Global handle to radio process
    QSharedPointer<RadioReplica> radio;

private slots:
    void readPendingDatagrams();

private:
    const int m_fft_size = 2048;
    float *m_fft_data;

    QUdpSocket *udpSocket;
    Ui::MainWindow *ui;
    GLSpectrum *m_glspectrum;
    GLWaterfall *m_glwaterfall;
    Panel *m_panel;
};

#endif // MAINWINDOW_H
