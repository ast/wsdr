#ifndef WDGRAMSOCKET_H
#define WDGRAMSOCKET_H

#include <QUdpSocket>

class WDgramSocket : public QUdpSocket
{
    Q_OBJECT
public:
    WDgramSocket(QString name);
};

#endif // WDGRAMSOCKET_H
