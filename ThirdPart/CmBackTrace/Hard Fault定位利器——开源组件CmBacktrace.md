前言
记录学习过程STM32/GD32+FreeRTOS+CmBacktrace移植。在MCU的开发中，由于程序设计的疏忽或者没有考虑到的一些异常情况，非常容易触发Hard Fault，在开发前期阶段触发Hard Fault时，我们可以通过连接调试工具利用keil比较容易定位到问题所在，但是如果已经处于快接近成品阶段，有时不那么容易能接上调试器了。或者说，这个Hard Fault非常难触发，我们也不可能一直接上调试器等着。所以这里可以使用CmBackTrace开源组件，帮助我们不需要连接调试器就可以定位到问题原因。

CmBackTrace介绍
CmBacktrace（Cortex Microcontroller Backtrace）是一个开源库,可以自动跟踪和定位 ARM Cortex-M 系列 MCU 的错误代码，并自动分析错误原因。该库可以将错误信息输出到控制台，或者把错误信息保存在FLASH中，之后通过读取控制台输出内容或者flash中存储内容，再配合addr2line工具，可以看到函数调用堆栈、故障诊断结果、堆栈、故障寄存器和产品固件信息，大大提高了错误定位的效率和准确性。开源项目链接: https://github.com/armink/CmBacktrace

        该开源组件的作者有一句话说的很有道理，值得学习。在此分享给大家："工欲善其事，必先利其器。所以有时候效率低下的原因可能是你会使用的工具种类太少"。学习嵌入式，就是要多面手，需要见多识别广。

组件移植
移植平台环境：

        GD32F103+Freertos + keil5

1、添加源文件
下载源码文件，把cm_backtrace文件下所有文件放入自己工程下。



然后在自己工程中添加了源码，根据自己选用的编译器，把fault_handler文件下对应的cmb_fault.S添加进去，我使用的是keil 就选择如下图所示。





再把源码路径添加进去，开启C99的支持。



2、配置cmb_cfg.h
这些配置必须得配置，不然会编译不过，具体的配置根据平台和使用场景配置。以我使用场景为例子，自己使用printf输出信息，使用了RTOS且类型为FreeRTOS，GD32F303内核为M4核，使能DUMP，默认语言英语，所以我的设置如下：

/* print line, must config by user */
#define cmb_println(...)          printf(__VA_ARGS__);printf("\r\n")       /* e.g., printf(__VA_ARGS__);printf("\r\n")  or  SEGGER_RTT_printf(0, __VA_ARGS__);SEGGER_RTT_WriteString(0, "\r\n")  */
/* enable bare metal(no OS) platform */
/* #define CMB_USING_BARE_METAL_PLATFORM */
/* enable OS platform */
#define CMB_USING_OS_PLATFORM
/* OS platform type, must config when CMB_USING_OS_PLATFORM is enable */
#define CMB_OS_PLATFORM_TYPE       CMB_OS_PLATFORM_FREERTOS   // CMB_OS_PLATFORM_RTT or CMB_OS_PLATFORM_UCOSII or CMB_OS_PLATFORM_UCOSIII or CMB_OS_PLATFORM_FREERTOS or CMB_OS_PLATFORM_RTX5 */
/* cpu platform type, must config by user */
#define CMB_CPU_PLATFORM_TYPE      CMB_CPU_ARM_CORTEX_M4  /* CMB_CPU_ARM_CORTEX_M0 or CMB_CPU_ARM_CORTEX_M3 or CMB_CPU_ARM_CORTEX_M4 or CMB_CPU_ARM_CORTEX_M7 or CMB_CPU_ARM_CORTEX_M33 */
/* enable dump stack information */
#define CMB_USING_DUMP_STACK_INFO
/* language of print information */
#define CMB_PRINT_LANGUAGE         CMB_PRINT_LANGUAGE_ENGLISH  /* CMB_PRINT_LANGUAGE_ENGLISH(default) or CMB_PRINT_LANGUAGE_CHINESE or CMB_PRINT_LANGUAGE_CHINESE_UTF8 */

2、解决编译报错
直接编译会报Error: L6200E: Symbol HardFault_Handler multiply defined (by cmb_fault.o and gd32f30x_it.o).错误，CmBacetrace库提供的 cmb_fault.s 中的HardFault_Handler 和gd32f30x_it.c重复定义了，我们直接把gd32f30x_it.c 中HardFault_Handler 注释了。

//TODO:注释HardFault_Handler
// /*!
//     \brief      this function handles HardFault exception
//     \param[in]  none
//     \param[out] none
//     \retval     none
// */
// void HardFault_Handler(void)
// {
//     /* if Hard Fault exception occurs, go to infinite loop */
//     while (1){
//     }
// }
继续编译，报错。因为该组件还不能直接FreeRTOS，因为 FreeRTOS 的 TCB 中没有 StackSize 信息，所以需要自己修改一下FreeRTOS源码。



好消是应该下个版本就可以直接使用了，在PR记录中已经更新，等着验证后合并了。



那么正式版本还没发布，我们还是自己来修改一下FreeRTOS的源码，移植demo里面也有FreeRTOS修改的参考demo，具体参考这里：



主要修改三个地方，第一在在 `FreeRTOS/tasks.c` 中任务TCB控制块中增加 `uxSizeOfStack` 字段：



第二在  `FreeRTOS/tasks.c` ， prvInitialiseNewTask 函数里面添加

pxNewTCB->uxSizeOfStack = ulStackDepth;   /*< Support For CmBacktrace >*/
具体位置如下：



第三在 `FreeRTOS/tasks.c`末尾增加 `vTaskStackAddr()` 、 `vTaskStackSize()` 、 `vTaskName()` 函数。具体如下：

//TODO:支持CmBacktrace
/*-----------------------------------------------------------*/
/*< Support For CmBacktrace >*/
uint32_t * vTaskStackAddr()
{
return pxCurrentTCB->pxStack;
}

uint32_t vTaskStackSize()
{
#if ( portSTACK_GROWTH > 0 )

    return (pxNewTCB->pxEndOfStack - pxNewTCB->pxStack + 1);
    
    #else /* ( portSTACK_GROWTH > 0 )*/
    
    return pxCurrentTCB->uxSizeOfStack;
    
    #endif /* ( portSTACK_GROWTH > 0 )*/
}

char * vTaskName()
{
return pxCurrentTCB->pcTaskName;
}
/*-----------------------------------------------------------*/

具体使用
我们只需要程序初始化的时候调用一次init即可。

//CmBacktrace
#define HARDWARE_VERSION               "V1.0.0"
#define SOFTWARE_VERSION               "V0.1.0"

int main(void)
{
uint32_t clk;
uint32_t ahb_clk;
uint32_t apb1_clk;
uint32_t apb2_clk;

    SystemInit();
    SystemCoreClockUpdate();
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
 
    /* CmBacktrace initialize */
    cm_backtrace_init("CmBacktrace", HARDWARE_VERSION, SOFTWARE_VERSION);
    
    //后面省略 示意
    while(1)
    {
        
    }

}

我们利用官方提供的测试函数，人为制造一个除0错误看触发Hard Fault后，这个组件实际情况：这里设置SCB->CCR寄存器目的是打开除0异常触发，否则即使除以0也不会触发Hard Fault。

void fault_test_by_div0(void) {
volatile int * SCB_CCR = (volatile int *) 0xE000ED14; // SCB->CCR
int x, y, z;

    *SCB_CCR |= (1 << 4); /* bit4: DIV_0_TRP. */
 
    x = 10;
    y = 0;
    z = x / y;
    printf("z:%d\n", z);
}
运行程序之后，触发除0 Hard Fault后，串口会输出打印信息如下，通过串口输出的提示信息可以看到，触发错误的原因是除0导致，具体函数触发位置在0x080024b4位置：



上面的信息不够直观，无法直接判断是哪个函数出了问题，我们使用addr2line 工具获取函数调用栈详细信息。addr2line （它是标准的 GNU Binutils 中的一部分）是一个可以将指令的地址和可执行映像转换成文件名、函数名和源代码行数的工具。关于addr2line更多详细信息可以参考https://github.com/armink/CmBacktrace/blob/master/docs/zh/how_to_use_addr2line_for_call_stack.md

把下载源码里面tools 文件夹中 addr2line.exe ，直接拷贝至 C:\Windows 下。cmd把路径切换到自己工程文件axf文件下，再运行下面的命令即可：

addr2line -e gd32f303cct6_template.axf -afpiC 080024b4 08003786

// 其中 gd32f303cct6_template.axf 是自己工程编译生成的axf文件
// 080024b4 是PC所指向地址,可以理解为当前触发错误的地方
// 08003786 是LR所指向地址,理解为程序调用PC之后所返回的地址


从输出信息看到发生Hard Fault 的地方是serialrecv_task.c 文件中的第23行出现了除0错误。也就是我们源码的位置除0位置。



拓展
实际产品中，可以通过把打印的信息写入FLASH中，死机了就复位重启继续运行程序。需要查询具体信息时，直接把FLASH里面的内容读取输出即可。
————————————————

                            版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。

原文链接：https://blog.csdn.net/ylzmm/article/details/135038843