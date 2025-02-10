#if 1
#include <iostream>
#include <windows.h>
#include <string>
#include "fingerprint_action.h"
#include "fingerprint_type.h"
#include "fingerprint_protocol.h"

static std::string adjustComPort(const std::string &comPort)
{
    if (comPort.size() >= 4 && comPort.substr(0, 3) == "COM") {
        std::string portNumberStr = comPort.substr(3); // 提取 "COM" 后面的部分
        try {
            int portNumber = std::stoi(portNumberStr); // 将字符串转换为整数
            if (portNumber > 9) {
                // 当串口号大于 9 时，使用特殊路径格式 "\\.\COMxx"
//                return "\\\\.\\" + comPort;
                return R"(\\.\)" + comPort;
            }
        } catch (const std::invalid_argument &e) {
            // 不是一个有效的数字，可以选择处理异常或者直接返回原始串口号
        }
    }
    // 否则直接返回原始串口号
    return comPort;
}


int serial_init(HANDLE &serial_port, std::string com_port, DWORD baud_rate)
{
    COMMTIMEOUTS timeouts = {0};
    DCB dcbSerialParams = {0};

    com_port = adjustComPort(com_port); // 调整串口号格式，当串口号大于 9 时，使用特殊路径格式 "\\.\COMxx"

    serial_port = CreateFile(com_port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                             OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (serial_port == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open serial port" << std::endl;
        return 1;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(serial_port, &dcbSerialParams)) {
        std::cout << "Failed to get current serial port settings" << std::endl;
        CloseHandle(serial_port);
        return 1;
    }

//    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.BaudRate = baud_rate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(serial_port, &dcbSerialParams)) {
        std::cout << "Failed to set serial port settings" << std::endl;
        CloseHandle(serial_port);
        return 1;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(serial_port, &timeouts)) {
        std::cout << "Failed to set serial port timeouts" << std::endl;
        CloseHandle(serial_port);
        return 1;
    }
    return 0;
}


#ifdef __cplusplus
extern "C" {
#endif
HANDLE serialPort;
//extern "C" void demo_main(U8Bit command);
void demo_main(U8Bit command);
void setSysPolicy(void);
void setLed(void);
#ifdef __cplusplus
}
#endif

/*
 * 海凌科技指纹识别模块串口通信示例程序
 * 移植于 Windows 平台
 * 注意事项：
 *      1. 在fingerprint_device.c中，对接为当前平台的串口接收、发送函数
 *      2. 在demo.c中，对接延时函数 void Delay_ms(U32Bit delaytime)
 *      3. 在fingerprint_action.c中，对接函数void ASSERT_BUFFER(void)实现断言，用于检查缓冲区是否为空，并clear sent and recv buffer
 *      4. 非常重要：
 *           (1)在fingerprint_protocol.h中，确保宏定义 __packed 被定义为一字节对齐属性
 *           (2)在当前平台上，使用 #define __packed __attribute__((packed))
 *           (3)将原来的 typedef __packed struct xxx 修改为 typedef struct __packed xxx
 *
 *  #define __packed __attribute__((packed))
 * */


int main()
{
    if (serial_init(serialPort, "COM4", CBR_57600))
        return 1;
    printf("serial init success\n");
    static DWORD bytesTransferred;
//    {
//        uint8_t data[] = {0xF1, 0x1F, 0xE2, 0x2E, 0xB6, 0x6B, 0xA8, 0x8A, 0x00, 0x07, 0x86, 0x00, 0x00, 0x00, 0x00, 0x03, 0x08, 0xF5};
//        WriteFile(serialPort, data, sizeof(data), &bytesTransferred, nullptr);
//    }
//    Sleep(200);
//    {
//        // 5.34. 获取指纹模块 ID
//        uint8_t data[] = {0xF1, 0x1F, 0xE2, 0x2E, 0xB6, 0x6B, 0xA8, 0x8A, 0x00, 0x07, 0x86, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0xFC };
//        WriteFile(serialPort, data, sizeof(data), &bytesTransferred, nullptr);
//    }
//    Sleep(200);
//    {
//        uint8_t data[] = {0xF1, 0x1F, 0xE2, 0x2E, 0xB6, 0x6B, 0xA8, 0x8A, 0x00, 0x07, 0x86, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0E, 0xF0 };
//        WriteFile(serialPort, data, sizeof(data), &bytesTransferred, nullptr);
//    }
//    Sleep(200);
//    {
//        // 5.32. 获取系统策略
//        uint8_t data[] = {0xF1, 0x1F, 0xE2, 0x2E, 0xB6, 0x6B, 0xA8, 0x8A, 0x00, 0x07, 0x86, 0x00, 0x00, 0x00, 0x00, 0x02, 0xFB, 0x03 };
//        WriteFile(serialPort, data, sizeof(data), &bytesTransferred, nullptr);
//    }
//    Sleep(200);

//    demo_main(0);
    int command;
    while (true) {
        std::cin >> command;
        if (command == -1)
            break;
        demo_main(command);
    }
    return 0;
}
#endif

#if 0
#include <iostream>
#include <windows.h>
#include <string>


std::string adjustComPort(const std::string &comPort)
{
    if (comPort.size() >= 4 && comPort.substr(0, 3) == "COM") {
        std::string portNumberStr = comPort.substr(3); // 提取 "COM" 后面的部分
        try {
            int portNumber = std::stoi(portNumberStr); // 将字符串转换为整数
            if (portNumber > 9) {
                // 当串口号大于 9 时，使用特殊路径格式 "\\.\COMxx"
//                return "\\\\.\\" + comPort;
                return R"(\\.\)" + comPort;
            }
        } catch (const std::invalid_argument &e) {
            // 不是一个有效的数字，可以选择处理异常或者直接返回原始串口号
        }
    }
    // 否则直接返回原始串口号
    return comPort;
}

int readFromSerialPort(HANDLE serialPort, unsigned char *buffer, unsigned int length)
{
    DWORD bytesRead;
    if (!ReadFile(serialPort, buffer, length, &bytesRead, nullptr)) {
        std::cerr << "Failed to read from serial port" << std::endl;
        return -1;
    }
    return (int) bytesRead;
}

int main()
{
    HANDLE serialPort;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    DWORD bytesTransferred;
    char data[] = "Hello, Serial Port!";
    unsigned char buffer[256]; // 假设缓冲区大小为 256

    std::string com_port = "COM11";
    com_port = adjustComPort(com_port); // 调整串口号格式，当串口号大于 9 时，使用特殊路径格式 "\\.\COMxx"

    serialPort = CreateFile(com_port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (serialPort == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open serial port" << std::endl;
        return 1;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(serialPort, &dcbSerialParams)) {
        std::cout << "Failed to get current serial port settings" << std::endl;
        CloseHandle(serialPort);
        return 1;
    }

//    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.BaudRate = CBR_57600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(serialPort, &dcbSerialParams)) {
        std::cout << "Failed to set serial port settings" << std::endl;
        CloseHandle(serialPort);
        return 1;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(serialPort, &timeouts)) {
        std::cout << "Failed to set serial port timeouts" << std::endl;
        CloseHandle(serialPort);
        return 1;
    }

    if (!WriteFile(serialPort, data, strlen(data), &bytesTransferred, nullptr)) {
        std::cout << "Failed to write to serial port" << std::endl;
        CloseHandle(serialPort);
        return 1;
    }

    std::cout << "Data written to serial port" << std::endl;

    // 读取数据
    int bytesRead = readFromSerialPort(serialPort, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        std::cout << "Read " << bytesRead << " bytes from serial port" << std::endl;
        // 处理读取到的数据
        std::cout << "Data received: " << buffer << std::endl;
    } else {
        std::cerr << "Failed to read from serial port" << std::endl;
    }

    CloseHandle(serialPort);
    return 0;
}
#endif
