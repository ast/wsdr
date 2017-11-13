#include "wdgramsocket.h"

extern "C" {
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
}

WDgramSocket::WDgramSocket(QString name)
{
    int err;
    struct sockaddr_un addr;
    const char *socketpath = name.toLocal8Bit().constData();

    // Remove just in case
    unlink(socketpath);

    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(fd < 0) {
        throw std::runtime_error("socket");
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socketpath);

    // Set size of receive buffer
    // TODO
    int recv_buf = 2048 * sizeof(float) * 4;
    err = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &recv_buf, sizeof(recv_buf));
    if(err < 0) {
        throw std::runtime_error("setsockopt");
    }

    if(::bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error("bind");
    }

    // Finally use this socket fd
    setSocketDescriptor(fd);
}
