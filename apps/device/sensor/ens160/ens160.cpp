#include "ens160.h"
#include "usart.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>

//static const uint8_t ENS160_ADDR = 0x52 << 1; // Use 7-bit address
static const uint8_t ENS160_ADDR = 0x53 << 1; // Use 8-bit address
// Register addresses
static const uint8_t REG_PART_ID = 0x00;
static const uint8_t REG_OPMODE = 0x10;
static const uint8_t REG_CONFIG = 0x11;
static const uint8_t REG_COMMAND = 0x12;
static const uint8_t REG_TEMP_IN = 0x13;
static const uint8_t REG_RH_IN = 0x15;
static const uint8_t REG_DATA_T = 0x30;
static const uint8_t REG_DATA_AQI = 0x21;
static const uint8_t REG_DATA_TVOC = 0x22;
static const uint8_t REG_DATA_ECO2 = 0x24;
// Register values
static const uint8_t OPMODE_STANDARD = 0x02;
static const uint8_t OPMODE_DEP_SLEEP = 0x01;
static const uint8_t OPMODE_IDLE = 0x01;
//static const uint8_t OPMODE_STD = 0x02;

HAL_StatusTypeDef ret;
uint8_t buf[24];
int16_t val;

enum Command {
    /// No operation
    Nop = 0x00,
    /// Get FW version
    GetAppVersion = 0x0E,
    /// Clears GPR Read Registers
    Clear = 0xCC,
};

/// Operation Mode of the sensor.
enum OperationMode {
    /// DEEP SLEEP mode (low power standby).
    Sleep = 0x00,
    /// IDLE mode (low-power).
    Idle = 0x01,
    /// STANDARD Gas Sensing Modes.
    Standard = 0x02,
    /// Reset device.
    Reset = 0xF0,
};









#define POLY   uint8_t(0x1D)   ///< 0b00011101 = x^8+x^4+x^3+x^2+x^0 (x^8 is implicit)
#define ENS160_PART_ID   uint16_t(0x160)   ///< ENS160 chip version

/* ENS160 register address */
#define ENS160_PART_ID_REG     uint8_t(0x00)   ///< This 2-byte register contains the part number in little endian of the ENS160.

#define ENS160_OPMODE_REG      uint8_t(0x10)   ///< This 1-byte register sets the Operating Mode of the ENS160.
#define ENS160_CONFIG_REG      uint8_t(0x11)   ///< This 1-byte register configures the action of the INTn pin.
#define ENS160_COMMAND_REG     uint8_t(0x12)   ///< This 1-byte register allows some additional commands to be executed on the ENS160.

#define ENS160_TEMP_IN_REG     uint8_t(0x13)   ///< This 2-byte register allows the host system to write ambient temperature data to ENS160 for compensation.
#define ENS160_RH_IN_REG       uint8_t(0x15)   ///< This 2-byte register allows the host system to write relative humidity data to ENS160 for compensation.

#define ENS160_DATA_STATUS_REG uint8_t(0x20)   ///< This 1-byte register indicates the current STATUS of the ENS160.

#define ENS160_DATA_AQI_REG    uint8_t(0x21)   ///< This 1-byte register reports the calculated Air Quality Index according to the UBA.
#define ENS160_DATA_TVOC_REG   uint8_t(0x22)   ///< This 2-byte register reports the calculated TVOC concentration in ppb.
#define ENS160_DATA_ECO2_REG   uint8_t(0x24)   ///< This 2-byte register reports the calculated equivalent CO2-concentration in ppm, based on the detected VOCs and hydrogen.
#define ENS160_DATA_ETOH_REG   uint8_t(0x22)   ///< This 2-byte register reports the calculated ethanol concentration in ppb.

#define ENS160_DATA_T_REG      uint8_t(0x30)   ///< This 2-byte register reports the temperature used in its calculations (taken from TEMP_IN, if supplied).
#define ENS160_DATA_RH_REG     uint8_t(0x32)   ///< This 2-byte register reports the relative humidity used in its calculations (taken from RH_IN if supplied).

#define ENS160_DATA_MISR_REG   uint8_t(0x38)   ///< This 1-byte register reports the calculated checksum of the previous DATA_ read transaction (of n-bytes).

#define ENS160_GPR_WRITE_REG   uint8_t(0x40)   ///< This 8-byte register is used by several functions for the Host System to pass data to the ENS160.
#define ENS160_GPR_READ_REG    uint8_t(0x48)   ///< This 8-byte register is used by several functions for the ENS160 to pass data to the Host System.

/* CMD(0x12) register command */
#define ENS160_COMMAND_NOP          uint8_t(0x00)   ///< reserved. No command.
#define ENS160_COMMAND_GET_APPVER   uint8_t(0x0E)   ///< Get FW Version Command.
#define ENS160_COMMAND_CLRGPR       uint8_t(0xCC)   ///< Clears GPR Read Registers Command.

/* OPMODE(Address 0x10) register mode */
#define ENS160_SLEEP_MODE      uint8_t(0x00)   ///< DEEP SLEEP mode (low power standby).
#define ENS160_IDLE_MODE       uint8_t(0x01)   ///< IDLE mode (low-power).
#define ENS160_STANDARD_MODE   uint8_t(0x02)   ///< STANDARD Gas Sensing Modes.
typedef enum
{
    eINTDataDrdyDIS = 0<<1,   /**< Disable */
    eINTDataDrdyEN = 1<<1,   /**< Enable */
}eINTDataDrdy_t;
typedef enum
{
    eIntGprDrdyDIS = 0<<3,   /**< Disable */
    eIntGprDrdyEN = 1<<3,   /**< Enable */
}eIntGprDrdy_t;
typedef struct
{
    uint8_t   GPRDrdy: 1; /**< General purpose register data ready */
    uint8_t   dataDrdy: 1; /**< Measured data ready */
    uint8_t   validityFlag: 2; /**< 0: Normal operation, 1: Warm-Up phase, 2: Initial Start-Up phase, 3: Invalid output */
    uint8_t   reserved: 2; /**< Reserved bit */
    uint8_t   stater: 1; /**< High indicates that an error is detected. E.g. Invalid Operating Mode has been selected. */
    uint8_t   status: 1; /**< High indicates that an OPMODE is running */
} __attribute__ ((packed)) sSensorStatus_t;
sSensorStatus_t ENS160Status;
#define ENS160_CONCAT_BYTES(msb, lsb)   (((uint16_t)msb << 8) | (uint16_t)lsb)

void writeReg(uint8_t reg, uint8_t *buff, uint8_t len)
{
    uint8_t buff1[20];
    buff[0] = reg;
    for (int i = 1; i < len+1; ++i) {
        buff1[i] = buff[i-1];
    }
//    HAL_I2C_Mem_Write(&hi2c2, ENS160_ADDR, reg, len, buff, len, HAL_MAX_DELAY);
//    HAL_I2C_Master_Transmit(&hi2c2, ENS160_ADDR, buff, len, HAL_MAX_DELAY);
//    HAL_I2C_Master_Transmit(&hi2c2, ENS160_ADDR, &reg, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c2, ENS160_ADDR, buff1, len+1, HAL_MAX_DELAY);
}

int readReg(uint8_t reg, void* pBuf, size_t size)
{
//    HAL_I2C_Mem_Read(&hi2c2, ENS160_ADDR, reg, 1, (uint8_t *)pBuf, size, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c2, ENS160_ADDR, &reg, 1, HAL_MAX_DELAY);
    HAL_I2C_Master_Receive(&hi2c2, ENS160_ADDR, (uint8_t *)pBuf, size, HAL_MAX_DELAY);
}

void setPWRMode(uint8_t mode)
{
    writeReg(ENS160_OPMODE_REG, &mode, sizeof(mode));
    HAL_Delay(20);   // Give it some time to switch mode
}

void setINTMode(uint8_t mode)
{
    mode |= (eINTDataDrdyEN | eIntGprDrdyDIS);
    writeReg(ENS160_CONFIG_REG, &mode, sizeof(mode));
    HAL_Delay(20);   // Give it some time to switch mode
}

void setTempAndHum(float ambientTemp, float relativeHumidity)
{
    uint16_t temp = (ambientTemp + 273.15) * 64;
    uint16_t rh = relativeHumidity * 512;
    uint8_t buf[4];

    buf[0] = temp & 0xFF;
    buf[1] = (temp & 0xFF00) >> 8;
    buf[2] = rh & 0xFF;
    buf[3] = (rh & 0xFF00) >> 8;
    writeReg(ENS160_TEMP_IN_REG, buf, sizeof(buf));
}

uint8_t getENS160Status(void)
{
    readReg(ENS160_DATA_STATUS_REG, &ENS160Status, sizeof(ENS160Status));
    return ENS160Status.validityFlag;
}

uint8_t getAQI(void)
{
    uint8_t data = 0;
    readReg(ENS160_DATA_AQI_REG, &data, sizeof(data));
    return data;
}

uint16_t getTVOC(void)
{
    uint8_t buf[2];
    readReg(ENS160_DATA_TVOC_REG, buf, sizeof(buf));
    return ENS160_CONCAT_BYTES(buf[1], buf[0]);
}

uint16_t getECO2(void)
{
    uint8_t buf[2];
    readReg(ENS160_DATA_ECO2_REG, buf, sizeof(buf));
    return ENS160_CONCAT_BYTES(buf[1], buf[0]);
}


extern "C" _Noreturn __weak void func(I2C_HandleTypeDef *hi2c)
{
    uint8_t idBuf[2];
    if(0 == readReg(ENS160_PART_ID_REG, idBuf, sizeof(idBuf)))   // Judge whether the data bus is successful
    {
//        DBG("ERR_DATA_BUS");
//        return ERR_DATA_BUS;
    }

//    DBG("real sensor id=");DBG(ENS160_CONCAT_BYTES(idBuf[1], idBuf[0]));
    if(ENS160_PART_ID != ENS160_CONCAT_BYTES(idBuf[1], idBuf[0]))   // Judge whether the chip version matches
    {
//        DBG("ERR_IC_VERSION");
//        return ERR_IC_VERSION;
    }
    setPWRMode(ENS160_STANDARD_MODE);
    setINTMode(0x00);
    setTempAndHum(25.0, 50.0);
//    buf[0] = REG_PART_ID;
//    ret = HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//    if (ret != HAL_OK) {
//        strcpy((char*)buf, "Error TX\r\n");
//    } else {
//        ret = HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, buf, 2, HAL_MAX_DELAY);
//        if (ret != HAL_OK) {
//            strcpy((char*)buf, "Error Rx\r\n");
//        } else {
//            sprintf((char*)buf, "Device ID: 0x%x 0x%x\r\n", buf[0], buf[1]);
//        }
//    }
//    HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//
//    // Change the sensor mode to standard mode
//    buf[0] = REG_OPMODE;
//    buf[1] = OPMODE_STANDARD;
//    ret = HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 2, HAL_MAX_DELAY);
//    if (ret != HAL_OK) {
//        strcpy((char*)buf, "Error TX\r\n");
//        HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//    }
//
//    bool newData = false;
//    uint8_t status;
//    bool waitForNew = true;
//#define ENS160_DATA_STATUS_NEWDAT	0x02
//#define IS_NEWDAT(x) 			(ENS160_DATA_STATUS_NEWDAT == (ENS160_DATA_STATUS_NEWDAT & (x)))
//    // Set default status for early bail out
//    if (waitForNew) {
//        do {
//            HAL_Delay(1);
//            buf[0] = 0x20;
////            status = this->read8(_slaveaddr, ENS160_REG_DATA_STATUS);
//            HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//            HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, &status, 1, HAL_MAX_DELAY);
//
////            if (debugENS160) {
////                Serial.print("Status: ");
////                Serial.println(status);
////            }
//            sprintf((char*)buf, "Status: 0x%x\r\n", status);
//            HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//
//        } while (!IS_NEWDAT(status));
//    } else {
//        buf[0] = 0x20;
////        status = this->read8(_slaveaddr, ENS160_REG_DATA_STATUS);
//        HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//        HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, &status, 1, HAL_MAX_DELAY);
//    }
//
//    // Read predictions
//    if (IS_NEWDAT(status)) {
//        newData = true;
////        this->read(_slaveaddr, ENS160_REG_DATA_AQI, i2cbuf, 7);
////        _data_aqi = i2cbuf[0];
////        _data_tvoc = i2cbuf[1] | ((uint16_t)i2cbuf[2] << 8);
////        _data_eco2 = i2cbuf[3] | ((uint16_t)i2cbuf[4] << 8);
////        if (_revENS16x > 0) _data_aqi500 = ((uint16_t)i2cbuf[5]) | ((uint16_t)i2cbuf[6] << 8);
////        else _data_aqi500 = 0;
//        buf[0] = 0x21;
//        HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//        HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, buf, 7, HAL_MAX_DELAY);
//        sprintf((char*)buf, "buf: 0x%x%x%x%x%x%x%d\r\n", buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6]);
//        HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//    }
//
//    HAL_Delay(5000);
//    {
//        float ambientTemp = 25.0, relativeHumidity = 50.0;
//        uint16_t temp = (ambientTemp + 273.15) * 64;
//        uint16_t rh = relativeHumidity * 512;
//        uint8_t buf1[4];
//
//        buf1[0] = temp & 0xFF;
//        buf1[1] = (temp & 0xFF00) >> 8;
//        buf1[2] = rh & 0xFF;
//        buf1[3] = (rh & 0xFF00) >> 8;
//        HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf1, 4, HAL_MAX_DELAY);
//    }
//    buf[0] = 0x20;
//    HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//    HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//    sprintf((char*)buf, "Status: 0x%x\r\n", buf[0]);
//    HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//    HAL_Delay(1000);
//
//    buf[0] = 0x21;
//    HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//    HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, buf, 7, HAL_MAX_DELAY);
////    sprintf((char*)buf, "AQI: 0x%x\r\n", buf[0]);
//    sprintf((char*)buf, "AQI: 0x%x%d%d%d%d\r\n", buf[0],buf[1],buf[2],buf[3],buf[4]);
//    HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//
//
//
//    // Read the sensor mode
//    buf[0] = REG_OPMODE;
//    ret = HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//    if (ret != HAL_OK) {
//        strcpy((char*)buf, "Error TX\r\n");
//    } else {
//        ret = HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, buf, 2, HAL_MAX_DELAY);
//        if (ret != HAL_OK) {
//            strcpy((char*)buf, "Error Rx\r\n");
//        } else {
//            val = ((int16_t)buf[0] << 4) | (buf[1] >> 4);
//        }
//        sprintf((char*)buf, "Mode: 0x%x\r\n", (unsigned int) val);
//    }
//    HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//
//
//
//    buf[0] = REG_TEMP_IN;
//    buf[1] = 0x4A;
//    buf[2] = 0x8A;
//    HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 3, HAL_MAX_DELAY);
//    buf[0] = REG_RH_IN;
//    HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 3, HAL_MAX_DELAY);

    while (true)
    {
        uint8_t Status = getENS160Status();
        sprintf((char*)buf, "Status: 0x%x\r\n", Status);
        HAL_UART_Transmit(&huart1, buf, strlen((char *) buf), HAL_MAX_DELAY);

        uint8_t AQI = getAQI();
        sprintf((char*)buf, "AQI: 0x%x\r\n", AQI);
        HAL_UART_Transmit(&huart1, buf, strlen((char *) buf), HAL_MAX_DELAY);

        uint16_t TVOC = getTVOC();
        sprintf((char*)buf, "TVOC: 0x%xppb\r\n", TVOC);
        HAL_UART_Transmit(&huart1, buf, strlen((char *) buf), HAL_MAX_DELAY);

        uint16_t ECO2 = getECO2();
        sprintf((char*)buf, "ECO2: 0x%xppm\r\n", ECO2);
        HAL_UART_Transmit(&huart1, buf, strlen((char *) buf), HAL_MAX_DELAY);
//        sprintf((char*)buf, "---------------------\r\n");
//        HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//
//        // Read the temperature data used for calculation
//        buf[0] = REG_DATA_T;
//        ret = HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//        if (ret != HAL_OK) {
//            strcpy((char*)buf, "Error TX\r\n");
//        } else {
//            ret = HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, buf, 2, HAL_MAX_DELAY);
//            if (ret != HAL_OK) {
//                strcpy((char*)buf, "Error Rx\r\n");
//            } else {
//                sprintf((char*)buf, "Temperature: 0x%x 0x%x\r\n", buf[0], buf[1]);
//            }
//        }
//        HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//
//        // Read the TVOC data
//        buf[0] = REG_DATA_TVOC;
//        ret = HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//        if (ret != HAL_OK) {
//            strcpy((char*)buf, "Error TX\r\n");
//        } else {
//            ret = HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, buf, 2, HAL_MAX_DELAY);
//            if (ret != HAL_OK) {
//                strcpy((char*)buf, "Error Rx\r\n");
//            } else {
//                int total_value = ((uint16_t)(buf[0] << 4)) | (buf[1] >> 4);
//                sprintf((char*)buf, "TVOC: %u ppb\r\n", total_value);
//            }
//        }
//        HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);
//        // Read the AQI data
//        buf[0] = REG_DATA_AQI;
//        ret = HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, buf, 1, HAL_MAX_DELAY);
//        if (ret != HAL_OK) {
//            strcpy((char*)buf, "Error TX\r\n");
//        } else {
//            ret = HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, buf, 2, HAL_MAX_DELAY);
//            if (ret != HAL_OK) {
//                strcpy((char*)buf, "Error Rx\r\n");
//            } else {
//                uint16_t total_value = ((uint16_t)(buf[0] << 8)) | (buf[1]);
//                sprintf((char*)buf, "AQI: 0x%x \r\n", total_value);
//            }
//        }
//        HAL_UART_Transmit(&huart1, buf, strlen((char*)buf), HAL_MAX_DELAY);

        HAL_Delay(1000);
    }
}



#if 0
#include <math.h>

#include "../../../shell/Arduino/Wire/Wire.h"
#include "../../../shell/Arduino/HAL/gf-hal-gpio.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define min fmin

class {
public:
    void print(...){}
    void println(...){}
} Serial;

static inline void delay(uint32_t ms)
{
//    void delay_us(volatile unsigned long nTime);
//    delay_us(us)
    HAL_Delay(ms);
}

ScioSense_ENS160::ScioSense_ENS160(uint8_t slaveaddr) {
    this->_slaveaddr = slaveaddr;

    this->_ADDR = 0;
    this->_nINT = 0;
    this->_nCS = 0;
}

ScioSense_ENS160::ScioSense_ENS160(uint8_t ADDR, uint8_t nCS, uint8_t nINT) {
    this->_slaveaddr = ENS160_I2CADDR_0;

    this->_ADDR = ADDR;
    this->_nINT = nINT;
    this->_nCS = nCS;
}

ScioSense_ENS160::ScioSense_ENS160(uint8_t slaveaddr, uint8_t ADDR, uint8_t nCS, uint8_t nINT) {
    this->_slaveaddr = slaveaddr;

    this->_ADDR = ADDR;
    this->_nINT = nINT;
    this->_nCS = nCS;
}

// Function to redefine I2C pins
void ScioSense_ENS160::setI2C(uint8_t sda, uint8_t scl) {
    this->_sdaPin = sda;
    this->_sclPin = scl;
}

// Init I2C communication, resets ENS160 and checks its PART_ID. Returns false on I2C problems or wrong PART_ID.
bool ScioSense_ENS160::begin(bool debug)
{
    debugENS160 = debug;

    //Set pin levels
    if (this->_ADDR > 0) {
        pinMode(this->_ADDR, OUTPUT);
        digitalWrite(this->_ADDR, LOW);
    }
    if (this->_nINT > 0) pinMode(this->_nINT, INPUT_PULLUP);
    if (this->_nCS > 0) {
        pinMode(this->_nCS, OUTPUT);
        digitalWrite(this->_nCS, HIGH);
    }

    //init I2C
    _i2c_init();
    if (debugENS160) {
        Serial.println("begin() - I2C init done");
    }
    delay(ENS160_BOOTING);                   // Wait to boot after reset

    this->_available = false;
    this->_available = this->reset();

    this->_available = this->checkPartID();

    if (this->_available) {
        this->_available = this->setMode(ENS160_OPMODE_IDLE);
        this->_available = this->clearCommand();
        this->_available = this->getFirmware();
    }
    if (debugENS160) {
        Serial.println("ENS160 in idle mode");
    }
    return this->_available;
}

// Sends a reset to the ENS160. Returns false on I2C problems.
bool ScioSense_ENS160::reset(void)
{
    uint8_t result = this->write8(_slaveaddr, ENS160_REG_OPMODE, ENS160_OPMODE_RESET);

    if (debugENS160) {
        Serial.print("reset() result: ");
        Serial.println(result == 0 ? "ok" : "nok");
    }
    delay(ENS160_BOOTING);                   // Wait to boot after reset

    return result == 0;
}

// Reads the part ID and confirms valid sensor
bool ScioSense_ENS160::checkPartID(void) {
    uint8_t i2cbuf[2];
    uint16_t part_id;
    bool result = false;

    this->read(_slaveaddr, ENS160_REG_PART_ID, i2cbuf, 2);
    part_id = i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8);

    if (debugENS160) {
        Serial.print("checkPartID() result: ");
        if (part_id == ENS160_PARTID) Serial.println("ENS160 ok");
        else if (part_id == ENS161_PARTID) Serial.println("ENS161 ok");
        else Serial.println("nok");
    }
    delay(ENS160_BOOTING);                   // Wait to boot after reset

    if (part_id == ENS160_PARTID) { this->_revENS16x = 0; result = true; }
    else if (part_id == ENS161_PARTID) { this->_revENS16x = 1; result = true; }

    return result;
}

// Initialize idle mode and confirms
bool ScioSense_ENS160::clearCommand(void) {
    uint8_t status;
    uint8_t result;

    result = this->write8(_slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_NOP);
    result = this->write8(_slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_CLRGPR);
    if (debugENS160) {
        Serial.print("clearCommand() result: ");
        Serial.println(result == 0 ? "ok" : "nok");
    }
    delay(ENS160_BOOTING);                   // Wait to boot after reset

    status = this->read8(_slaveaddr, ENS160_REG_DATA_STATUS);
    if (debugENS160) {
        Serial.print("clearCommand() status: 0x");
        Serial.println(status, HEX);
    }
    delay(ENS160_BOOTING);                   // Wait to boot after reset

    return result == 0;
}

// Read firmware revisions
bool ScioSense_ENS160::getFirmware() {
    uint8_t i2cbuf[3];
    uint8_t result;

    this->clearCommand();

    delay(ENS160_BOOTING);                   // Wait to boot after reset

    result = this->write8(_slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_GET_APPVER);
    result = this->read(_slaveaddr, ENS160_REG_GPR_READ_4, i2cbuf, 3);

    this->_fw_ver_major = i2cbuf[0];
    this->_fw_ver_minor = i2cbuf[1];
    this->_fw_ver_build = i2cbuf[2];

    if (this->_fw_ver_major > 6) this->_revENS16x = 1;
    else this->_revENS16x = 0;

    if (debugENS160) {
        Serial.println(this->_fw_ver_major);
        Serial.println(this->_fw_ver_minor);
        Serial.println(this->_fw_ver_build);
        Serial.print("getFirmware() result: ");
        Serial.println(result == 0 ? "ok" : "nok");
    }
    delay(ENS160_BOOTING);                   // Wait to boot after reset

    return result == 0;
}

// Set operation mode of sensor
bool ScioSense_ENS160::setMode(uint8_t mode) {
    uint8_t result;

    //LP only valid for rev>0
    if ((mode == ENS160_OPMODE_LP) and (_revENS16x == 0)) result = 1;
    else result = this->write8(_slaveaddr, ENS160_REG_OPMODE, mode);

    if (debugENS160) {
        Serial.print("setMode() activate result: ");
        Serial.println(result == 0 ? "ok" : "nok");
    }

    delay(ENS160_BOOTING);                   // Wait to boot after reset

    return result == 0;
}

// Initialize definition of custom mode with <n> steps
bool ScioSense_ENS160::initCustomMode(uint16_t stepNum) {
    uint8_t result;

    if (stepNum > 0) {
        this->_stepCount = stepNum;

        result = this->setMode(ENS160_OPMODE_IDLE);
        result = this->clearCommand();

        result = this->write8(_slaveaddr, ENS160_REG_COMMAND, ENS160_COMMAND_SETSEQ);
    } else {
        result = 1;
    }
    delay(ENS160_BOOTING);                   // Wait to boot after reset

    return result == 0;
}

// Add a step to custom measurement profile with definition of duration, enabled data acquisition and temperature for each hotplate
bool ScioSense_ENS160::addCustomStep(uint16_t time, bool measureHP0, bool measureHP1, bool measureHP2, bool measureHP3, uint16_t tempHP0, uint16_t tempHP1, uint16_t tempHP2, uint16_t tempHP3) {
    uint8_t seq_ack;
    uint8_t temp;

    if (debugENS160) {
        Serial.print("setCustomMode() write step ");
        Serial.println(this->_stepCount);
    }
    delay(ENS160_BOOTING);                   // Wait to boot after reset

    temp = (uint8_t)(((time / 24)-1) << 6);
    if (measureHP0) temp = temp | 0x20;
    if (measureHP1) temp = temp | 0x10;
    if (measureHP2) temp = temp | 0x8;
    if (measureHP3) temp = temp | 0x4;
    this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_0, temp);

    temp = (uint8_t)(((time / 24)-1) >> 2);
    this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_1, temp);

    this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_2, (uint8_t)(tempHP0/2));
    this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_3, (uint8_t)(tempHP1/2));
    this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_4, (uint8_t)(tempHP2/2));
    this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_5, (uint8_t)(tempHP3/2));

    this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_6, (uint8_t)(this->_stepCount - 1));

    if (this->_stepCount == 1) {
        this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_7, 128);
    } else {
        this->write8(_slaveaddr, ENS160_REG_GPR_WRITE_7, 0);
    }
    delay(ENS160_BOOTING);

    seq_ack = this->read8(_slaveaddr, ENS160_REG_GPR_READ_7);
    delay(ENS160_BOOTING);                   // Wait to boot after reset

    if ((ENS160_SEQ_ACK_COMPLETE | this->_stepCount) != seq_ack) {
        this->_stepCount = this->_stepCount - 1;
        return 0;
    } else {
        return 1;
    }

}

// Perform prediction measurement and stores result in internal variables
bool ScioSense_ENS160::measure(bool waitForNew) {
    uint8_t i2cbuf[8];
    uint8_t status;
    bool newData = false;

    // Set default status for early bail out
    if (debugENS160) Serial.println("Start measurement");

    if (waitForNew) {
        do {
            delay(1);
            status = this->read8(_slaveaddr, ENS160_REG_DATA_STATUS);

            if (debugENS160) {
                Serial.print("Status: ");
                Serial.println(status);
            }

        } while (!IS_NEWDAT(status));
    } else {
        status = this->read8(_slaveaddr, ENS160_REG_DATA_STATUS);
    }

    // Read predictions
    if (IS_NEWDAT(status)) {
        newData = true;
        this->read(_slaveaddr, ENS160_REG_DATA_AQI, i2cbuf, 7);
        _data_aqi = i2cbuf[0];
        _data_tvoc = i2cbuf[1] | ((uint16_t)i2cbuf[2] << 8);
        _data_eco2 = i2cbuf[3] | ((uint16_t)i2cbuf[4] << 8);
        if (_revENS16x > 0) _data_aqi500 = ((uint16_t)i2cbuf[5]) | ((uint16_t)i2cbuf[6] << 8);
        else _data_aqi500 = 0;
    }

    return newData;
}

// Perfrom raw measurement and stores result in internal variables
bool ScioSense_ENS160::measureRaw(bool waitForNew) {
    uint8_t i2cbuf[8];
    uint8_t status;
    bool newData = false;

    // Set default status for early bail out
    if (debugENS160) Serial.println("Start measurement");

    if (waitForNew) {
        do {
            delay(1);
            status = this->read8(_slaveaddr, ENS160_REG_DATA_STATUS);

            if (debugENS160) {
                Serial.print("Status: ");
                Serial.println(status);
            }
        } while (!IS_NEWGPR(status));
    } else {
        status = this->read8(_slaveaddr, ENS160_REG_DATA_STATUS);
    }

    if (IS_NEWGPR(status)) {
        newData = true;

        // Read raw resistance values
        this->read(_slaveaddr, ENS160_REG_GPR_READ_0, i2cbuf, 8);
        _hp0_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8)));
        _hp1_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[2] | ((uint16_t)i2cbuf[3] << 8)));
        _hp2_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[4] | ((uint16_t)i2cbuf[5] << 8)));
        _hp3_rs = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[6] | ((uint16_t)i2cbuf[7] << 8)));

        // Read baselines
        this->read(_slaveaddr, ENS160_REG_DATA_BL, i2cbuf, 8);
        _hp0_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[0] | ((uint16_t)i2cbuf[1] << 8)));
        _hp1_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[2] | ((uint16_t)i2cbuf[3] << 8)));
        _hp2_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[4] | ((uint16_t)i2cbuf[5] << 8)));
        _hp3_bl = CONVERT_RS_RAW2OHMS_F((uint32_t)(i2cbuf[6] | ((uint16_t)i2cbuf[7] << 8)));

        this->read(_slaveaddr, ENS160_REG_DATA_MISR, i2cbuf, 1);
        _misr = i2cbuf[0];
    }

    return newData;
}


// Writes t (degC) and h (%rh) to ENV_DATA. Returns false on I2C problems.
bool ScioSense_ENS160::set_envdata(float t, float h) {

    uint16_t t_data = (uint16_t)((t + 273.15f) * 64.0f);

    uint16_t rh_data = (uint16_t)(h * 512.0f);

    return this->set_envdata210(t_data, rh_data);
}

// Writes t and h (in ENS210 format) to ENV_DATA. Returns false on I2C problems.
bool ScioSense_ENS160::set_envdata210(uint16_t t, uint16_t h) {
    //uint16_t temp;
    uint8_t trh_in[4];

    //temp = (uint16_t)((t + 273.15f) * 64.0f);
    trh_in[0] = t & 0xff;
    trh_in[1] = (t >> 8) & 0xff;

    //temp = (uint16_t)(h * 512.0f);
    trh_in[2] = h & 0xff;
    trh_in[3] = (h >> 8) & 0xff;

    uint8_t result = this->write(_slaveaddr, ENS160_REG_TEMP_IN, trh_in, 4);

    return result;
}

/**************************************************************************/

void ScioSense_ENS160::_i2c_init() {
    if (this->_sdaPin != this->_sclPin) Wire.begin(this->_sdaPin, this->_sclPin);
    else Wire.begin();
}

/**************************************************************************/

uint8_t ScioSense_ENS160::read8(uint8_t addr, uint8_t reg) {
    uint8_t ret;
    this->read(addr, reg, &ret, 1);

    return ret;
}

uint8_t ScioSense_ENS160::read(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num) {
    uint8_t pos = 0;
    uint8_t result = 0;

    if (debugENS160) {
        Serial.print("I2C read address: 0x");
        Serial.print(addr, HEX);
        Serial.print(" - register: 0x");
        Serial.println(reg, HEX);
    }

    //on arduino we need to read in 32 byte chunks
    while(pos < num){

        uint8_t read_now = min((uint8_t)32, (uint8_t)(num - pos));
        Wire.beginTransmission((uint8_t)addr);

        Wire.write((uint8_t)reg + pos);
        result = Wire.endTransmission();
        Wire.requestFrom((uint8_t)addr, read_now);

        for(int i=0; i<read_now; i++){
            buf[pos] = Wire.read();
            pos++;
        }
    }
    return result;
}

/**************************************************************************/

uint8_t ScioSense_ENS160::write8(uint8_t addr, uint8_t reg, uint8_t value) {
    uint8_t result = this->write(addr, reg, &value, 1);
    return result;
}

uint8_t ScioSense_ENS160::write(uint8_t addr, uint8_t reg, uint8_t *buf, uint8_t num) {
    if (debugENS160) {
        Serial.print("I2C write address: 0x");
        Serial.print(addr, HEX);
        Serial.print(" - register: 0x");
        Serial.print(reg, HEX);
        Serial.print(" -  value:");
        for (int i = 0; i<num; i++) {
            Serial.print(" 0x");
            Serial.print(buf[i], HEX);
        }
        Serial.println();
    }

    Wire.beginTransmission((uint8_t)addr);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t *)buf, num);
    uint8_t result = Wire.endTransmission();
    return result;
}
#endif


