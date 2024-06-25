/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


#include <stdio.h>
//#include "stdio.h"
#define DEV_DISK1 0
#define DEV_DISK2 1
#define SECTOR_SIZE 512
//#define SECTOR_PER_CLUSTER 8
//#define CLUSTER_SIZE (SECTOR_SIZE * SECTOR_PER_CLUSTER)
//#define NUM_OF_CLUSTERS 1024
//#define NUM_OF_SECTORS (NUM_OF_CLUSTERS * SECTOR_PER_CLUSTER)
static const char *disk_path[] = {
//        "../../disk1.vhd",
//        "../../disk2.vhd",
        "G:\\Project\\FileSystem\\FATFS\\disk1.vhd",
        "G:\\Project\\FileSystem\\FATFS\\disk2.vhd",
};
static FILE *disk_file[2] = {NULL, NULL};


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
        BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    int result;

//	switch (pdrv) {
//	case DEV_RAM :
//		result = RAM_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case DEV_MMC :
//		result = MMC_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case DEV_USB :
//		result = USB_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
//	}
//	return STA_NOINIT;
    return disk_file[pdrv] ? RES_OK : STA_NOINIT;
    //return disk_file[pdrv] != NULL ? RES_OK : STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
        BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    int result;

//	switch (pdrv) {
//	case DEV_RAM :
//		result = RAM_disk_initialize();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case DEV_MMC :
//		result = MMC_disk_initialize();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case DEV_USB :
//		result = USB_disk_initialize();
//
//		// translate the reslut code here
//
//		return stat;
//	}
    if ((pdrv != DEV_DISK1) && (pdrv != DEV_DISK2)) {
        printf("parameter error\n");
        return RES_PARERR;
    }
//    fopen(disk_path[pdrv], "r+");
    disk_file[pdrv] = fopen(disk_path[pdrv], "rb+");  // open the file in binary read/write mode
    if (disk_file[pdrv] == NULL) {
        printf("open file error in path %s\n", disk_path[pdrv]);
//        return RES_NOTRDY;
        return RES_ERROR;
    }
    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
        BYTE pdrv,		/* Physical drive nmuber to identify the drive */
        BYTE *buff,		/* Data buffer to store read data */
        LBA_t sector,	/* Start sector in LBA */
        UINT count		/* Number of sectors to read */
)
{
    DRESULT res;
    int result;

//	switch (pdrv) {
//	case DEV_RAM :
//		// translate the arguments here
//
//		result = RAM_disk_read(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case DEV_MMC :
//		// translate the arguments here
//
//		result = MMC_disk_read(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case DEV_USB :
//		// translate the arguments here
//
//		result = USB_disk_read(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//	}
//
//	return RES_PARERR;
    fseek(disk_file[pdrv], sector * SECTOR_SIZE, SEEK_SET);
    size_t read_size = count * SECTOR_SIZE;
//    if (fread(buff, 1, read_size, disk_file[pdrv]) != read_size) {
//        printf("read file error in path %s\n", disk_path[pdrv]);
//        return RES_PARERR;
//    }
//    return RES_OK;
    size_t read_count = fread(buff, 1, read_size, disk_file[pdrv]);
    if (read_count == read_size) {
        return RES_OK;
    }
    printf("read file error in path %s\n", disk_path[pdrv]);
    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
        BYTE pdrv,			/* Physical drive nmuber to identify the drive */
        const BYTE *buff,	/* Data to be written */
        LBA_t sector,		/* Start sector in LBA */
        UINT count			/* Number of sectors to write */
)
{
    //DRESULT res;
    //int result;

//	switch (pdrv) {
//	case DEV_RAM :
//		// translate the arguments here
//
//		result = RAM_disk_write(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case DEV_MMC :
//		// translate the arguments here
//
//		result = MMC_disk_write(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
//
//	case DEV_USB :
//		// translate the arguments here
//
//		result = USB_disk_write(buff, sector, count);
//
//		// translate the reslut code here
//
//		return res;
    //}

    //return RES_PARERR;

    fseek(disk_file[pdrv], sector * SECTOR_SIZE, SEEK_SET);
    size_t size = count * SECTOR_SIZE;
    size_t write_count = fwrite(buff, 1, size, disk_file[pdrv]);
    if (write_count == size) {
        return RES_OK;
    }
    printf("write file error in path %s\n", disk_path[pdrv]);
    return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
        BYTE pdrv,		/* Physical drive nmuber (0..) */
        BYTE cmd,		/* Control code */
        void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT res = 0;
    int result;

    //switch (pdrv) {
    //case DEV_RAM :

    //	// Process of the command for the RAM drive

    //	return res;

    //case DEV_MMC :

    //	// Process of the command for the MMC/SD card

    //	return res;

    //case DEV_USB :

    //	// Process of the command the USB drive

    //	return res;
    //}

    return RES_PARERR;
}

