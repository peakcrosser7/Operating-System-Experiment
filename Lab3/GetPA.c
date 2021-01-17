#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

//获取物理地址
void getMemAddr(unsigned long va) {     //va为虚拟内存地址
    size_t pageSize = getpagesize();    //页面大小
    unsigned long pageIdx = va / pageSize;  //页号
    unsigned long offset = va % pageSize;   //页内偏移
    uint64_t it;
    FILE* fp;

    printf("Virtual Addr: 0x%lx\n",va);
    printf("Page Index: 0x%lx\nPage Offset: 0x%lx\n", pageIdx, offset);
    //打开当前进程的pagemap文件
    if((fp=fopen("/proc/self/pagemap", "rb")) == NULL) {
        printf("Open /proc/self/pagemap Error.\n");
        return;
    }
    //每项记录为8字节，找到当前页号对应的记录
    unsigned long fileOffset = pageIdx * sizeof(uint64_t);
    if(fseek(fp,fileOffset,SEEK_SET)!=0) {
        printf("fSeek Error.\n");
        return;
    }
    //读取记录
    if(fread(&it,sizeof(uint64_t),1,fp) != 1) {
        printf("fread Error.\n");
        return;
    }
    fclose(fp);
    printf("item: 0x%" PRIx64 "\n", it);
    //记录的第63位记录当前页面位置：1为在物理内存中，0表示不在物理内存中
    if((it >> 63) & 1 == 0) {
        printf("Page Present is 0.\nThe present page isn't in the Physical Memory.\n");
        return;
    }
    //记录的0-54位位物理页号
    uint64_t phyPageIdx = (((uint64_t)1 << 55) - 1) & it;
    printf("Physical Page Index: 0x%" PRIx64 "\n",phyPageIdx);
    //物理地址=物理页号*页大小+页内偏移
    unsigned long pa = phyPageIdx * pageSize + offset;
    printf("Physical Addr: 0x%lx\n\n", pa);
}

int a = 100;

int max(int a, int b) {
    return a >= b ? a : b;
}

int main() {
    int b = 10, c = 20;

    int pid = fork();
    if(pid == 0) {
        printf("ChildProcess: pid = %d\n", getpid());
        printf("Global variable - a:\n");
        // printf("a=%d\n", a+=10);
        getMemAddr(&a);
        printf("Function - max:\n");
        getMemAddr(&max);
        printf("Function - fork:\n");
        getMemAddr(&fork);
       
    }
    else { 
        printf("Local variable - b:\n");
        getMemAddr(&b);

        printf("Local variable - c:\n");
        getMemAddr(&c);

        printf("ParentProcess: pid = %d\n", getpid());
        printf("Function - max:\n");
        getMemAddr(&max);
        printf("Global variable - a:\n");
        // printf("a=%d\n", a+=5);
        getMemAddr(&a);
        printf("Function - fork:\n");
        getMemAddr(&fork);
    }

    return 0;

}