#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int* addr; /* Địa chỉ trỏ đến shared memory mapping */

    /*Dùng mmap() với cờ MAP_SHARED tạo ra shared anonymous mapping*/

    addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
    {
        printf("mmap fail\n");
    }

    *addr = 1; /* Khởi tạo giá trị của addr */

    switch (fork())
    {
    case -1:
        printf("fork fail\n");
    case 0: /* Trong tiến trình con, tăng *addr 1 đơn vị */
        printf("Child started, value = %d\n", *addr);
        (*addr)++;
        if (munmap(addr, sizeof(int)) == -1)
        {
            printf("munmap fail\n");
        }
        exit(EXIT_SUCCESS);
    default: /* Trong tiến trình cha, chờ con kết thúc rồi truy xuất *addr */
        if (wait(NULL) == -1)
        {
            printf("wait fail\n");
        }
        printf("In parent, value = %d\n", *addr);

        if (munmap(addr, sizeof(int)) == -1)
        {
            printf("munmap fail\n");
        }

        exit(EXIT_SUCCESS);
    }
}