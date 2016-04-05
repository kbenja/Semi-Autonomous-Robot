#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>

#ifndef IPC_Module_H
#define IPC_Module_H


class IPC_Module
{
const char *socket_path;
struct sockaddr_un addr;
char buffer[225];
int fd;

public:
    IPC_Module(const char* path) {
        socket_path = path;
    };
    int unix_socket_initialize() {
        // create socket – functions will return -1 if error occurs
        if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
            return -1;
        }
        memset(&addr, 0, sizeof(addr)); // initialize all socket memory to 0
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
        return (connect(fd, (struct sockaddr*)&addr, sizeof(addr))); // connect to socket
    }
    int unix_socket_write(int16_t sending[]) {
        return write(fd, sending, sizeof(sending));
    }
    void unix_socket_read(int16_t * instructions) {
        // write(fd,"Hello", 5);
        read(fd,buffer,4);
        *(instructions) = buffer[0];
        *(instructions+1) = buffer[1];
    }
};

#endif
