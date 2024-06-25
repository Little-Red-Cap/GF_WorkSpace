#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "ff.h"


DWORD get_fattime()
{
    time_t t;
    struct tm *stm;

    t = time(0);
    stm = localtime(&t);

    return (DWORD) (stm->tm_year - 80) << 25 |
           (DWORD) (stm->tm_mon + 1) << 21 |
           (DWORD) (stm->tm_mday - 0) << 16 |
           (DWORD) (stm->tm_hour - 0) << 11 |
           (DWORD) (stm->tm_min - 0) << 5 |
           (DWORD) (stm->tm_sec - 0) >> 1;
}

static void print_file(FILE *file)
{
    while (1) {
        static char read_buf[1024];
        UINT read_size;
        FRESULT res = f_read(file, read_buf, sizeof(read_buf) - 1, &read_size);
        if (res != FR_OK) {
            printf("\n --- read error --- \n");
            break;
        }
        read_buf[sizeof(read_buf) - 1] = '\0';
        read_buf[read_size] = '\0';
        printf("%s", read_buf);
        if (read_size < sizeof(read_buf) - 1) {
            printf("\n --- read complete --- \n");
            break;
        }
    }
}

static FATFS disk1_fatfs;
static FATFS disk2_fatfs;
// 中文文件名需要修改ffconf.h中的 FF_CODE_PAGE
// 长文件名需要使能ffconf,h中的 FF_USE_LFN，将其置1
// SFN + LFN
// SFN: 8 + dot +EXT(3)
// LFN:

int main(void)
{
    FRESULT res = f_mount(&disk1_fatfs, "0:", 1);
    if (res != FR_OK) {
        printf("Failed to mount disk1\n");
        return -1;
    }

    res = f_mount(&disk2_fatfs, "1:", 1);
    if (res != FR_OK) {
        printf("Failed to mount disk2\n");
        return -1;
    }

    FIL file1, file2;
    FRESULT open_res = f_open(&file1, "0:/file1.txt", FA_READ | FA_WRITE);
    if (open_res != FR_OK) {
        printf("Failed to open file1\n");
        return -1;
    }
    printf("reading file1:\n");
    print_file(&file1);


//    //open_res = f_open(&file2, "1:/file2.txt", FA_READ | FA_WRITE);
//    open_res = f_open(&file2, "1:/中文文件.txt", FA_READ | FA_WRITE);
//    if (open_res != FR_OK) {
//        printf("Failed to open file2\n");
//        return -1;
//    }
//    printf("\nreading file2:\n");
//    print_file(&file2);

    UINT bytes_written = 0;
    char buffer[] = "\nHello, World!";
    f_write(&file1, buffer, sizeof(buffer), &bytes_written);
    printf("Wrote %d bytes to file1\n", bytes_written);

//    UINT bytes_read = 0;
//    char read_buffer[100];
//    f_read(&file2, read_buffer, sizeof(read_buffer), &bytes_read);
//    printf("Read %d bytes from file2: %s\n", bytes_read, read_buffer);

    f_close(&file1);
    open_res = f_open(&file1, "0:/write.bin", FA_WRITE | FA_CREATE_ALWAYS);
    if (open_res != FR_OK) {
        printf("Failed to open file1\n");
        return -1;
    }
    static UINT16 write_buf[1024];
    for (int i = 0; i < 1024; i++) {
        write_buf[i] = i;
    }
    UINT write_size;
    f_write(&file1, write_buf, sizeof(write_buf), &write_size);

    f_close(&file1);
//    f_close(&file2);

    return 0;
}
