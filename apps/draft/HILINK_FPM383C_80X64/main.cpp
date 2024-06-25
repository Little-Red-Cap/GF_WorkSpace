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
        std::string portNumberStr = comPort.substr(3); // ��ȡ "COM" ����Ĳ���
        try {
            int portNumber = std::stoi(portNumberStr); // ���ַ���ת��Ϊ����
            if (portNumber > 9) {
                // �����ںŴ��� 9 ʱ��ʹ������·����ʽ "\\.\COMxx"
//                return "\\\\.\\" + comPort;
                return R"(\\.\)" + comPort;
            }
        } catch (const std::invalid_argument &e) {
            // ����һ����Ч�����֣�����ѡ�����쳣����ֱ�ӷ���ԭʼ���ں�
        }
    }
    // ����ֱ�ӷ���ԭʼ���ں�
    return comPort;
}


int serial_init(HANDLE &serial_port, std::string com_port, DWORD baud_rate)
{
    COMMTIMEOUTS timeouts = {0};
    DCB dcbSerialParams = {0};

    com_port = adjustComPort(com_port); // �������ںŸ�ʽ�������ںŴ��� 9 ʱ��ʹ������·����ʽ "\\.\COMxx"

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
 * ����Ƽ�ָ��ʶ��ģ�鴮��ͨ��ʾ������
 * ��ֲ�� Windows ƽ̨
 * ע�����
 *      1. ��fingerprint_device.c�У��Խ�Ϊ��ǰƽ̨�Ĵ��ڽ��ա����ͺ���
 *      2. ��demo.c�У��Խ���ʱ���� void Delay_ms(U32Bit delaytime)
 *      3. ��fingerprint_action.c�У��ԽӺ���void ASSERT_BUFFER(void)ʵ�ֶ��ԣ����ڼ�黺�����Ƿ�Ϊ�գ���clear sent and recv buffer
 *      4. �ǳ���Ҫ��
 *           (1)��fingerprint_protocol.h�У�ȷ���궨�� __packed ������Ϊһ�ֽڶ�������
 *           (2)�ڵ�ǰƽ̨�ϣ�ʹ�� #define __packed __attribute__((packed))
 *           (3)��ԭ���� typedef __packed struct xxx �޸�Ϊ typedef struct __packed xxx
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
//        // 5.34. ��ȡָ��ģ�� ID
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
//        // 5.32. ��ȡϵͳ����
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
        std::string portNumberStr = comPort.substr(3); // ��ȡ "COM" ����Ĳ���
        try {
            int portNumber = std::stoi(portNumberStr); // ���ַ���ת��Ϊ����
            if (portNumber > 9) {
                // �����ںŴ��� 9 ʱ��ʹ������·����ʽ "\\.\COMxx"
//                return "\\\\.\\" + comPort;
                return R"(\\.\)" + comPort;
            }
        } catch (const std::invalid_argument &e) {
            // ����һ����Ч�����֣�����ѡ�����쳣����ֱ�ӷ���ԭʼ���ں�
        }
    }
    // ����ֱ�ӷ���ԭʼ���ں�
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
    unsigned char buffer[256]; // ���軺������СΪ 256

    std::string com_port = "COM11";
    com_port = adjustComPort(com_port); // �������ںŸ�ʽ�������ںŴ��� 9 ʱ��ʹ������·����ʽ "\\.\COMxx"

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

    // ��ȡ����
    int bytesRead = readFromSerialPort(serialPort, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        std::cout << "Read " << bytesRead << " bytes from serial port" << std::endl;
        // �����ȡ��������
        std::cout << "Data received: " << buffer << std::endl;
    } else {
        std::cerr << "Failed to read from serial port" << std::endl;
    }

    CloseHandle(serialPort);
    return 0;
}
#endif
