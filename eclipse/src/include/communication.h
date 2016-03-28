#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>

#ifndef IPC_H
#define IPC_H


class IPC
{
const char *socket_path;
struct sockaddr_un addr;
char buffer[255];
int fd;

public:
    IPC() {
        socket_path = "/tmp/breakerbot.socket";
    };
    // functions will return -1 if error occurs
    int unix_socket_connect() {
        // create socket
        if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
            return -1;
        }
        // initialize all socket memory to 0
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
        // connect to socket
        return (connect(fd, (struct sockaddr*)&addr, sizeof(addr)));
    }
    int unix_socket_write() {
        return write(fd,"Hello",5);
    }
    int unix_socket_read() {
        int status = read(fd,buffer,100);
        if (status > 0) {
            printf("Reading from buffer: ");
            printf("%02x",buffer[0]);
            printf("%02x",buffer[1]);
            printf("\n");
        }
    }
};

#endif
