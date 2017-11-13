#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtNetwork/QUdpSocket>
#include <QLocalSocket>
#include <QDebug>
#include <QVBoxLayout>

#include "wdgramsocket.h"
#include "panel.h"

#include <complex>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect to remote object
    QRemoteObjectNode *node = new QRemoteObjectNode(this);
    if(!node->connectToNode(QUrl(QStringLiteral("local:/tmp/wsdr")))) {
        throw std::runtime_error("connectToNode");
    }
    radio.reset(node->acquire<RadioReplica>());

    const int fft_size = 2048;
    m_glspectrum = new GLSpectrum(fft_size, 192000.0, this);
    m_glspectrum->setFilterBandwidth(4000.0);

    m_glwaterfall = new GLWaterfall(fft_size, this);
    m_panel = new Panel(this);
    m_panel->setFixedHeight(200);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_glspectrum);
    layout->addWidget(m_glwaterfall);
    layout->addWidget(m_panel);

    centralWidget()->setLayout(layout);


    m_fft_data = new float[m_fft_size];

    connect(m_glspectrum, &GLSpectrum::selectedOffsetChanged, radio.data(), &RadioReplica::setFrequencyOffset);


    udpSocket = new WDgramSocket("/tmp/sdrclient");
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
}

void MainWindow::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        int d = udpSocket->readDatagram((char*)m_fft_data, m_fft_size * sizeof(float));
        m_glspectrum->updateWaterfall(m_fft_data);
        m_glwaterfall->updateWaterfall(m_fft_data);
    }
}


MainWindow::~MainWindow()
{
    delete m_glspectrum;
    delete m_glwaterfall;
    delete m_fft_data;
    delete ui;
}
