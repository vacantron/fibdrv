#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define FIB_DEV "/dev/fibonacci"

#define UINT64_MAX 0xFFFFFFFFFFFFFFFFULL

void print_u128(int i, __uint128_t u128)
{
    unsigned long long lower = u128 & UINT64_MAX;
    unsigned long long upper = u128 >> 64 & UINT64_MAX;

    if (upper)
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%llu%llu.\n",
               i, upper, lower);
    else
        printf("Reading from " FIB_DEV
               " at offset %d, returned the sequence "
               "%llu.\n",
               i, lower);
}

int main()
{
    char *buf;
    char write_buf[] = "testing writing";
    int offset = 100; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    for (int i = 0; i <= offset; i++) {
        long long sz;
        sz = write(fd, write_buf, strlen(write_buf));
        printf("Writing to " FIB_DEV ", returned the sequence %lld\n", sz);
    }

    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        read(fd, buf, sizeof(__uint128_t));
        print_u128(i, *buf);
    }

    for (int i = offset; i >= 0; i--) {
        lseek(fd, i, SEEK_SET);
        read(fd, buf, sizeof(__uint128_t));
        print_u128(i, *buf);
    }

    close(fd);
    return 0;
}
