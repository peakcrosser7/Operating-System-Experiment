#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

char src[256];
char dest[256];
int pc = 0;

int main() {
    int len, cnt;
    int pos;
    int fd = open("/dev/mydev2", O_RDWR);
    if(fd < 0) {
        printf("Open dev2 fail\n");
        return 0;
    }
    printf("Write string:\n");
    while(scanf("%s", src) != EOF) {
        len = strlen(src);
        cnt = write(fd, src, len);
        printf("Write to dev2 %dB\n", cnt);
    }
    
    printf("pos: %ld\n", lseek(fd, 0, SEEK_SET));
    printf("Read bytes of string:\n");
    while(scanf("%d", &len) != EOF) {
        cnt = read(fd, dest, len);
        dest[cnt] = 0;
        printf("Read from dev2 %dB: %s\n", cnt, dest);
    }
    close(fd);
    return 0;
}