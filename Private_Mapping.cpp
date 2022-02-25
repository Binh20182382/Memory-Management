#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
    char* addr;
    int fd;
    struct stat sb;
    char str[16] = "Overwrite file";

    fd = open("hello.txt", O_RDWR);
    if (-1 == fd)
    {
        printf("Open file error\n");
    }

    /*Obtain the size of the file and save to sb structure*/
    /*Use it to specify the size of the mapping*/

    if (-1 == fstat(fd, &sb))
    {
        printf("Stat file error\n");
    }

    /*Create memory mapping to fd with flag MAP_PRIVATE*/
    addr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    if (MAP_FAILED == addr)
    {
        printf("mmap error\n");
    }

    if (write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size)
    {
        printf("write to stdout error\n");
    }

    /*Overwrite content of string “str” to addr*/
    memset(addr, 0x0, sizeof(addr));
    sprintf(addr, "%s", str);

    printf("Mapped memory content: %s\n", addr);

    exit(EXIT_SUCCESS);
}