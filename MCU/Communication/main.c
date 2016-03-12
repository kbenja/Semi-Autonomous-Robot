#include <stdio.h>   /* Standard input/output definitions */
#include <fcntl.h>   /* File control definitions */
int main()
{
    int fd;
    unsigned char data;
    unsigned char buf[100];
    int i, n;
    fd = open("/dev/ttymcu0", O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        printf("open ttymcu0 failed!\n");
        return 1;
    }

    i = 0;

    do
    {
        n = read(fd, &data, 1);
        buf[i] = data;
        i++;
    } while(data != '\n' && i < 100);

    i = (i >=99? 99:i);
    buf[i] = '\0';
    printf("Response from MCU: %s", buf);

    close(fd);
    return 0;
}
