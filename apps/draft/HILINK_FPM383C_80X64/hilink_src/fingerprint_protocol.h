
#ifndef FINGERPRINT_PROTOCOL_H
#define FINGERPRINT_PROTOCOL_H

#include "fingerprint_type.h"

#ifdef __cplusplus   
extern "C" {   
#endif

#define __packed __attribute__((packed))
//#define __packed

typedef struct {
    U8Bit * ID;
    U32Bit length;
} FP_moduleID_t, * FP_moduleID_p;

typedef struct {
    U8Bit enroll_mode;
    U8Bit times;
	U8Bit fingerID_high;
	U8Bit fingerID_low;
} FP_auto_enroll_t, *FP_auto_enroll_p;

typedef struct {
    U8Bit state;
    U16Bit fingerID;
    U8Bit progress;
} FP_enrollResult_t, *FP_enrollResult_p;

typedef struct {
    U16Bit isPass;
    U16Bit matchScore;
    U16Bit matchID;
} FP_matchResult_t, *FP_matchResult_p;

typedef struct {
    U16Bit ftrMaxCount;
    U8Bit distributon[64];
}FP_ftrDistribution_t, *FP_ftrDistribution_p;

typedef struct {
    U16Bit id;
    U16Bit length;
    U8Bit  *data;
}FP_ftrData_t, *FP_ftrData_p;


typedef enum 
{
    EM_LED_CTRL_OFF = 0,                //关闭LED灯
    EM_LED_CTRL_ON = 1,                 //开启LED灯
    EM_LED_CTRL_PWM = 3,                //PWM控制LED灯(呼吸灯)
    EM_LED_CTRL_BLINK = 4,              //闪烁LED灯
} EM_LED_CTRL;

typedef enum
{
    EM_LED_NONE = 0,					//无颜色控制
    EM_LED_GREEN = 1,					//绿色
    EM_LED_RED = 2,						//红色
    EM_LED_GREEN_WITH_RED = 3,			//红色+绿色（黄色）
    EM_LED_BLUE = 4,					//蓝色
	EM_LED_RED_WITH_BLUE = 5,			//红色+蓝色（粉色）
	EM_LED_GREEN_WITH_BLUE = 6,			//绿色+蓝色（青色）
	EM_LED_ALL = 7,						//红色+蓝色+绿色（白色）
} EM_LED_COLOR;


typedef struct 
{
    EM_LED_CTRL ucLEDCtrlMode;			//LED控制模式
    EM_LED_COLOR ucLEDColor;			//LED颜色
    U8Bit usLEDpara1;				//LED点亮时长or最大占空比
    U8Bit usLEDpara2;			    //LED熄灭时长or最小占空比
    U8Bit usLEDpara3;				    //闪烁周期or占空比变化速率
} FP_LED_CTRL_INFO, *PFP_LED_CTRL_INFO;


typedef struct __packed FP_data_area
{
    U8Bit  *data;
    U32Bit length;
}FP_data_area_t, *FP_data_area_p;

typedef struct __packed FP_frame_head
{
    U8Bit  forhead[8];
    U16Bit length;
    U8Bit  checksum;
}FP_frame_head_t, *FP_frame_head_p;

typedef struct __packed FP_send_mesg_pkg
{
    FP_frame_head_t frame_head;
    U32Bit pwd;
    U8Bit  cmd_type;
    U8Bit  cmd_word;
}FP_send_t, *FP_send_p;

typedef struct __packed FP_recv_mesg_pkg
{
    FP_frame_head_t frame_head;
    U32Bit pwd;
    U8Bit  cmd_type;
    U8Bit  cmd_word;
    U8Bit  error_code[4];
}FP_recv_t, *FP_recv_p;

typedef enum
{
    sleep_type_normal = 0,
    sleep_type_deep   = 1
}FP_sleep_type;


/*
 --------------------------------------------------------------------------------------------------------------------
 |                         |            |          |              |          |          |               |          |
 |          forhead        |            |          |   password   |          |          |               |          |
 |                         |   length   | checksum |              | cmd_type | cmd_word |   data_area   | checksum |
 | F1 1F E2 2E B6 6B A8 8A |            |          |  0x00000000  |          |          |               |          |
 |                         |            |          |              |          |          |               |          |
 --------------------------------------------------------------------------------------------------------------------
 |                         |            |          |              |          |          |               |          |
 |           8byte         |   2 byte   |  1 byte  |    4 byte    |  1 byte  |  1 byte  |     n byte    |  1 byte  |
 |                         |            |          |              |          |          |               |          |
 --------------------------------------------------------------------------------------------------------------------
 |                                                 |                                    |
 | <<----------    FP_frame_head_t    ---------->> |                                    |
 |                                                 |                                    |
 |--------------------------------------------------------------------------------------|
 |
                                                                                      |
 | <<-----------------------------     FP_send_t    --------------------------------->> |
 |                                                                                      | 
 ----------------------------------------------------------------------------------------





 ---------------------------------------------------------------------------------------------------------------------------
 |                         |            |          |              |          |          |           |           |          |
 |          forhead        |            |          |   password   |          |          |           |           |          |
 |                         |   length   | checksum |              | cmd_type | cmd_word | errorCode | data_area | checksum |
 | F1 1F E2 2E B6 6B A8 8A |            |          |  0x00000000  |          |          |           |           |          |
 |                         |            |          |              |          |          |           |           |          |
 ---------------------------------------------------------------------------------------------------------------------------
 |                         |            |          |              |          |          |           |           |          |
 |           8byte         |   2 byte   |  1 byte  |    4 byte    |  1 byte  |  1 byte  |   4 byte  |   n byte  |  1 byte  |
 |                         |            |          |              |          |          |           |           |          |
 ---------------------------------------------------------------------------------------------------------------------------
 |                                                 |                                                |
 | <<----------    FP_frame_head_t    ---------->> |                                                |
 |                                                 |                                                |
 |--------------------------------------------------------------------------------------------------|
 |
                                                                                                  |
 | <<-----------------------------------     FP_recv_t    --------------------------------------->> |
 |                                                                                                  | 
 ----------------------------------------------------------------------------------------------------
*/




/******************************************   errorCode   ***************************************/
#define COMP_CODE_OK                            (0x00)  //正常返回
#define COMP_CODE_UNKOWN_CMD                    (0x01)  //未知命令
#define COMP_CODE_CMD_DATA_LEN_ERROR            (0x02)  //请求字段长度非法
#define COMP_CODE_CMD_DATA_ERROR                (0x03)  //请求字段非法
#define COMP_CODE_CMD_NOT_FINISHED              (0x04)  //当前有命令正在执行，不接受新命令
#define COMP_CODE_NO_REQ_CMD                    (0x05)  //没有发送该命令的请求，就查询结果
#define COMP_CODE_SYS_SOFT_ERROR                (0x06)  //系统软件上报错误
#define COMP_CODE_HARDWARE_ERROR                (0x07)  //硬件错误
#define COMP_CODE_NO_FINGER_DETECT              (0x08)  //没有检测到手指按压，超时退出
#define COMP_CODE_FINGER_EXTRACT_ERROR          (0x09)  //指纹提取发生错误，可能原因:图像质量不佳等
#define COMP_CODE_FINGER_MATCH_ERROR            (0x0A)  //指纹匹配发生错误，可能原因:没有指纹
#define COMP_CODE_STORAGE_IS_FULL               (0x0B)  //存储空间满
#define COMP_CODE_STORAGE_WRITE_ERROR           (0x0C)  //存储写入失败
#define COMP_CODE_STORAGE_READ_ERROR            (0x0D)  //存储读取失败
#define COMP_CODE_UNQUALIFIED_IMAGE_ERROR       (0x0E)  //采集的指纹图像不合格
#define COMP_CODE_SAME_ID                       (0x0F)  //重复指纹
#define COMP_CODE_IMAGE_LOW_COVERAGE_ERROR      (0x10)  //采图面积小
#define COMP_CODE_CAPTURE_LARGE_MOVE            (0x11)  //移动范围过大
#define COMP_CODE_CAPTURE_NO_MOVE               (0x12)  //移动范围过小
#define COMP_CODE_STORAGE_REPEAT_FINGERPRINT    (0x13)  //指纹索引号被占用
#define COMP_CODE_CAPTURE_IMAGE_FAIL            (0x14)  //采图失败
#define COMP_CODE_FORCE_QUIT                    (0x15)  //强制退出
#define COMP_CODE_NONE_UPDATE                   (0x16)  //没有更新
#define COMP_CODE_INVALID_FINGERPRINT_ID        (0x17)  //无效指纹ID
#define COMP_CODE_ADJUST_GAIN_ERROR				(0x18)
#define COMP_CODE_DATA_BUFFER_OVERFLOW          (0x19)  //数据缓冲区溢出
#define COMP_CODE_CURRENT_SENSOR_SLEEP			(0x1A)	//当前正在休眠
#define COMP_CODE_PASSWORD_ERROR				(0x1B)	//校验密码错误
#define COMP_CODE_CHECKSUM_ERROR				(0x1C)	//校验和错误
#define COMP_CODE_PIN_ERROR						(0x1D)	//PIN码错误

#define COMP_CODE_FLASH_ID_ERROR				(0x20)
#define COMP_CODE_PARAMETER_ERROR				(0x21)
#define COMP_CODE_READ_FTR_ERROR				(0x22)
#define COMP_CODE_FTR_CRC_ERR					(0x23)
#define COMP_CODE_OTHER_ERROR                   (0xFF)  //其他错误



/* get fingerprint frame*/
S32Bit FP_protocol_get_fp_enroll_start_frame(FP_send_p send, U8Bit index);
S32Bit FP_protocol_get_fp_enroll_result_frame(FP_send_p send);
S32Bit FP_protocol_get_fp_enroll_save_start_frame(FP_send_p send, U16Bit id);
S32Bit FP_protocol_get_fp_enroll_save_result_frame(FP_send_p send);
S32Bit FP_protocol_get_fp_enroll_cancel_frame(FP_send_p send);
S32Bit FP_protocol_get_fp_match_start_frame(FP_send_p send);
S32Bit FP_protocol_get_fp_match_result_frame(FP_send_p send);
S32Bit FP_protocol_get_fp_delete_start_frame(FP_send_p send, S16Bit id);
S32Bit FP_protocol_get_fp_delete_result_frame(FP_send_p send);
S32Bit FP_protocol_get_fp_is_touch_sensor_frame(FP_send_p send);
S32Bit FP_protocol_get_fp_enroll_verify_start_frame(FP_send_p send);
S32Bit FP_protocol_get_fp_enroll_verify_result_frame(FP_send_p send);

S32Bit FP_protocol_get_fp_update_start_frame(FP_send_p send, S16Bit updateID);
S32Bit FP_protocol_get_fp_update_result_frame(FP_send_p send);


S32Bit FP_protocol_fp_auto_enroll_frame(FP_send_p send , FP_auto_enroll_t enrollPara);
S32Bit FP_protocol_fp_match_syn_frame(FP_send_p send);
S32Bit FP_protocol_fp_delete_syn_frame(FP_send_p send , S16Bit id);


/* get system frame */
S32Bit FP_protocol_get_sys_reset_frame(FP_send_p send);
S32Bit FP_protocol_get_sys_sleep_frame(FP_send_p send, FP_sleep_type type);

S32Bit FP_protocol_get_sys_pulse_frame(FP_send_p send);

/* get maintenace frame */
S32Bit FP_protocol_get_mtnce_read_id_frame(FP_send_p send);

S32Bit FP_protocol_get_fp_is_id_exist_frame(FP_send_p send, U16Bit id);

/* send mesg */
S32Bit FP_protocol_send_mesg(FP_send_p send, U32Bit timeout);
S32Bit FP_protocol_send_mesg_ex(FP_send_p send, U32Bit length, U32Bit timeout);

S32Bit FP_protocol_send_delete_mesg(FP_send_p send, S32Bit id_len, U32Bit timeout);

/* recv complete frame */
S32Bit FP_protocol_recv_complete_frame(FP_recv_p recv,FP_data_area_p data_area, U32Bit timeout);

S32Bit FP_protocol_get_fp_store_info_frame(FP_send_p send);
S32Bit FP_protocol_get_fp_get_ftr_info_frame(FP_send_p send, U16Bit id);
S32Bit FP_protocol_get_fp_get_ftr_data_frame(FP_send_p send, U16Bit frame);
S32Bit FP_protocol_get_fp_write_ftr_info_frame(FP_send_p send, U16Bit id, U16Bit length);
S32Bit FP_protocol_get_fp_write_ftr_data_frame(FP_send_p send, U16Bit frame, U8Bit * data, U32Bit length);

S32Bit FP_protocol_get_sys_setled_frame(FP_send_p send, PFP_LED_CTRL_INFO p_stLEDCtrlInfo);
S32Bit FP_protocol_get_sys_getpolicy_frame(FP_send_p send);
S32Bit FP_protocol_set_sys_getpolicy_frame(FP_send_p send, U32Bit *p_SysPolicy);


#ifdef __cplusplus   
}   
#endif

#endif //FINGERPRINT_PROTOCOL_H
