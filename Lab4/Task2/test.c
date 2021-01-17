#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


int main() {
    int a[2], maxx;
    int fd = open("/dev/mydev", O_RDWR);
    int len;
    if(fd < 0) {
        perror("Open dev fail\n");
        return 0;
    }
    printf("Input 2 int:");
    scanf("%d%d", &a[0], &a[1]);
    len = write(fd, a, 2*sizeof(int));
    if(len == 2*sizeof(int)) printf("Write: %d, %d\n", a[0], a[1]);
    len = read(fd, &maxx, sizeof(int));
    if(len == sizeof(int)) printf("Read max one: %d\n", maxx);
    close(fd);
    return 0;
}