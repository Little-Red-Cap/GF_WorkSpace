//#include "core_cm0.h"
//#include <core_cm3.h>
//#include <core_cm4.h>
//#include <core_cm7.h>
//#include <stdint.h>

#define CPU_FREQUENCY_MHZ   72

//#define VSF_I2C_CFG_IMP_PREFIX vsf_${i2c_ip}


void gc_print(const char *data, int len);
__attribute__((__noreturn__)) void main_application();
#if   defined ( __CC_ARM )      // // Arm Compiler 5
int fputc(int ch,FILE *f)
{
//    HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,100);
    (void)f;
    gc_print((const char *) &ch, 1);
    return ch;
}
__declspec(noreturn)
int $Sub$$main() { main_application(); }
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) // Arm Compiler 6 (armclang)
int fputc(int ch,FILE *f)
{
    (void)f;
    gc_print((const char *) &ch, 1);
    return ch;
}
__attribute__((__noreturn__))
int $Sub$$main() { main_application(); }
#elif defined ( __GNUC__ )      // GNU Compiler
__attribute__((used)) int _write(int file, const char *data, int len)
{
    (void)file;
    gc_print(data, len);
    return len;
}

__attribute__((constructor())) __attribute__((used))//__attribute__((__noreturn__)) __attribute__((unused))
static void GF_Startup_entry() { main_application(); }
#elif defined ( __ICCARM__ )    // IAR Compiler
#else
#error Unknown compiler.
#endif



typedef struct {
    volatile unsigned long CTRL;                   /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
    volatile unsigned long LOAD;                   /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */
    volatile unsigned long VAL;                    /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
    volatile const unsigned long CALIB;                  /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */
} SysTick_Type;

#define SysTick ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL))

__attribute__((weak))
void delay_us(volatile unsigned long nTime)
{
    int old_val, new_val, val;
    if (nTime > 900) {
        for (old_val = 0; old_val < nTime / 900; old_val++) {
            delay_us(900);
        }
        nTime = nTime % 900;
    }
    old_val = SysTick->VAL;
    new_val = old_val - CPU_FREQUENCY_MHZ * nTime;
    if (new_val >= 0) {
        do {
            val = SysTick->VAL;
        } while ((val < old_val) && (val >= new_val));
    } else {
        new_val += CPU_FREQUENCY_MHZ * 1000;
        do {
            val = SysTick->VAL;
        } while ((val <= old_val) || (val > new_val));
    }
}
