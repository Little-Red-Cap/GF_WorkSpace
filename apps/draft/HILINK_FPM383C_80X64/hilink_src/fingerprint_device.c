#include "fingerprint_device.h"

#include <windows.h>
#include <stdio.h>

extern HANDLE serialPort;
#define DEBUG_PRINT 1

/* read one byte */
S32Bit FP_device_read_one_byte(U8Bit * data, U32Bit timeout)
{
/*****2???*******
	S32Bit i=0 , readBytes = 0;

	for(i=0; i<=timeout; i++)
	{
		
		readBytes = Chip_UART_Read(LPC_USART1, data, 1);
		if(readBytes == 1)
	  {
		  return FP_OK;
	  }

	}
	
	return FP_DEVICE_TIMEOUT_ERROR;
*****************/
//    return 0;
    DWORD bytesRead;
    ReadFile(serialPort, data, 1, &bytesRead, NULL);
    if (bytesRead == 1)
        return FP_OK;
    return FP_DEVICE_TIMEOUT_ERROR;
}


/* read data */
S32Bit FP_device_read_data(U8Bit * data, U32Bit length, U32Bit timeout)
{
/****************
		 U32Bit i;
    for(i = 0; i < length; i++)
    {
        if(FP_device_read_one_byte(data + i, timeout) != FP_OK)
            return FP_DEVICE_TIMEOUT_ERROR;
    }
    
    return FP_OK;
****************/
//    DWORD bytesRead;
//    ReadFile(serialPort, data, length, &bytesRead, NULL);
//    return 0;
    for(U32Bit i = 0; i < length; i++)
    {
        if(FP_device_read_one_byte(data + i, timeout) != FP_OK)
            return FP_DEVICE_TIMEOUT_ERROR;
    }
#if DEBUG_PRINT
    printf("Read data:\n\t");
    for (int i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
#endif
    return FP_OK;
}

/* write data */
S32Bit FP_device_write_data(U8Bit * data, U32Bit length, U32Bit timeout)
{
/***********************
	  U32Bit sent, i;
	
	for(i = 0; i <= timeout; i++)
	{
		sent = Chip_UART_SendBlocking(LPC_USART1, data, length);
		if(sent == length)
		{
			return FP_OK;
		}
	}

		return FP_DEVICE_TIMEOUT_ERROR;
*************************/
    static DWORD bytesTransferred;
    WriteFile(serialPort, data, length, &bytesTransferred, NULL);
#if DEBUG_PRINT
    printf("Trans data:\n\t");
    for (int i = 0; i < length; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
#endif
    return 0;
}


