#include <stdint.h>
#include <stdio.h>
#include "fingerprint_protocol.h"
#include "fingerprint_device.h"

typedef enum command_type
{
    cmd_fingerprint = 0x01,
    cmd_system      = 0x02,
    cmd_maintenance = 0x03
}FP_cmd_type;

typedef enum fingerprint_type_word
{
    fp_capture_start        = 0x01,
    fp_capture_result       = 0x02,
    
    fp_enroll_start         = 0x11,
    fp_enroll_result        = 0x12,
    fp_enroll_save_start    = 0x13,
    fp_enroll_save_result   = 0x14,
    fp_enroll_cancel        = 0x15,
    
    fp_update_start         = 0x16,
    fp_update_result        = 0x17,

    fp_auto_enroll          = 0x18,
    
    fp_match_start          = 0x21,
    fp_match_result         = 0x22,
    fp_match_syn            = 0x23,

    fp_delete_start         = 0x31,
    fp_delete_result        = 0x32,
    fp_is_fp_id_exist       = 0x33,
    fp_get_store_info       = 0x34,
    fp_is_touch_sensor      = 0x35,

    fp_delete_ftr_syn       = 0x36,

    fp_write_ftr_info       = 0x51,
    fp_write_ftr_data       = 0x52,
    fp_read_ftr_info        = 0x53,
    fp_read_ftr_data        = 0x54
}FP_fp_cmd_word;

typedef enum system_command_word
{
    sys_set_passwd         = 0x01,
    sys_reset              = 0x02,
    sys_get_count          = 0x03,
    sys_set_gain           = 0x08,
    sys_get_gain           = 0x09,
    sys_set_valve          = 0x0A,
    sys_get_valve          = 0x0B,
    sys_sleep              = 0x0C,
    sys_set_enroll_max_num = 0x0D,
    sys_set_led            = 0x0F,
    sys_get_syspolicy      = 0xFB,
    sys_set_syspolicy      = 0xFC  
}FP_sys_cmd_word;

typedef enum maintenance_command_word
{
    maintenance_read_id          = 0x01,
    maintenance_set_id           = 0x02,
    maintenance_heart_beat       = 0x03,
    maintenance_set_baudrate     = 0x04,
    maintenance_set_com_passwd   = 0x05
}FP_mtnce_cmd_word;


static const U8Bit s_forhead[8] = {0xF1,0x1F,0xE2,0x2E,0xB6,0x6B,0xA8,0x8A};

//发送的数据长度
static U32Bit FP_protocol_get_data_area_length(FP_send_p send)
{
    U32Bit len = 0;
    if(cmd_fingerprint == (FP_cmd_type)(send->cmd_type)) 
    {
        switch((FP_fp_cmd_word)(send->cmd_word)) 
        {
        case fp_capture_start:
            len = 2;
            break;
        case fp_capture_result:
            len = 2;
            break;
        case fp_enroll_start:
            len = 1;
            break;
        case fp_enroll_result:
            len = 0;
            break;
        case fp_enroll_save_start:
            len = 2;
            break;
        case fp_enroll_save_result:
            len = 0;
            break;
        case fp_enroll_cancel:
            len = 0;
            break;
        case fp_update_start:
            len = 2;
            break;
        case fp_update_result:
            len = 0;
            break;
        case fp_match_start:
            len = 0;
            break;
        case fp_match_result:
            len = 0;
            break;
        case fp_delete_start:
            len = 3;
            break;
        case fp_delete_result:
            len = 0;
            break;
        case fp_auto_enroll:
            len = 4;
            break;
        case fp_match_syn:
            len = 0;
            break;
        case fp_delete_ftr_syn:
            len = 3;
            break;  
        case fp_is_fp_id_exist:
            len = 2;
            break;
        case fp_get_store_info:
            len = 0;
            break;
        case fp_is_touch_sensor:
            len = 0;
            break;
        case fp_write_ftr_info:
            len = 4;
            break;

        case fp_read_ftr_info:
            len = 2;
            break;
        case fp_read_ftr_data:
            len = 2;
            break;          
        }
    } 
    else if(cmd_system == (FP_cmd_type)(send->cmd_type)) 
    {
        switch((FP_sys_cmd_word)(send->cmd_word)) 
        {
        case sys_set_passwd:
            len = 4;
            break;
        case sys_reset:
            len = 0;
            break;
        case sys_get_count:
            len = 0;
            break;
        case sys_set_gain:
            len = 3;
            break;
        case sys_get_gain:
            len = 0;
            break;
        case sys_set_valve:
            len = 2;
            break;
        case sys_get_valve:
            len = 0;
            break;
        case sys_sleep:
            len = 1;
            break;
        case sys_set_led:
            len = 5;
            break;
        case sys_get_syspolicy:
            len = 0;
            break;
        case sys_set_syspolicy:
            len = 4;
            break;
        case sys_set_enroll_max_num:
            len = 1;
            break;
        }
    } 
    else if(cmd_maintenance == (FP_cmd_type)(send->cmd_type)) 
    {
        switch((FP_mtnce_cmd_word)(send->cmd_word)) 
        {
        case maintenance_read_id:
            len = 0;
            break;
        case maintenance_set_id:
            len = 16;
            break;
        case maintenance_heart_beat:
            len = 0;
            break;
        case maintenance_set_baudrate:
            len = 4;
            break;
        case maintenance_set_com_passwd:
            len = 4;
            break;
        }
    }

    return len;
}

static U32Bit FP_protocol_get_recv_data_length(FP_recv_p recv)
{
    U16Bit length = recv->frame_head.length;
    length = (U32Bit)(*(U8Bit *)&length) * 0x100 + *((U8Bit *)&length + 1);
    
    return length - (sizeof(FP_recv_t) - sizeof(FP_frame_head_t)) - 1;
}

static U8Bit FP_protocol_get_checksum(U8Bit * data, U32Bit length)
{
    U32Bit i = 0;
    S8Bit sum = 0;
    
    for(i = 0; i < length; i++)
        sum += data[i];

    return (U8Bit)((~sum)+1);
}

static S32Bit FP_protocol_checkout_recv_head_checksum(FP_frame_head_p head)
{
    U8Bit checksum = FP_protocol_get_checksum((U8Bit *)head, sizeof(FP_frame_head_t) - 1);
    if(*((U8Bit *)head + sizeof(FP_frame_head_t) - 1) != checksum)
        return FP_PROTOCOL_UART_HEAD_CHECKSUM_ERROR;
    else
        return FP_OK;
}

static S32Bit FP_protocol_checkout_recv_data_checksum(FP_recv_p recv)
{
    U8Bit checksum;
    
    U16Bit length = recv->frame_head.length;
    length = (U32Bit)(*(U8Bit *)&length) * 0x100 + *((U8Bit *)&length + 1);
    
    checksum = FP_protocol_get_checksum((U8Bit *)recv + sizeof(FP_frame_head_t), length - 1);
    
    if(*((U8Bit *)recv + sizeof(FP_frame_head_t) + length - 1) != checksum)
        return FP_PROTOCOL_DATA_CHECKSUM_ERROR;
    else
        return FP_OK;
}

/*
 --------------------------------------------------------------------------------------------------------------------
 |                         |            |          |              |          |          |               |          |
 |          forhead        |            |          |              |          |          |               |          |
 |                         |   length   | checksum |     pwd      | cmd_type | cmd_word |   data_area   | checksum |
 | F1 1F E2 2E B6 6B A8 8A |            |          |              |          |          |               |          |
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
*/

static void FP_protocol_get_frame_head(FP_frame_head_p head, unsigned short length)
{
    /*
     * forhead
     */
    U32Bit i = 0;
    for(i = 0; i < 8; i++)
    {
        *((U8Bit*)head->forhead + i) = *(s_forhead + i);
    }

    /*
     * length
     */
    *((U8Bit *) &head->length) = length/0x100;
    *((U8Bit *) &head->length + 1) = length%0x100;

    /*
     * checksum
     */
    head->checksum = FP_protocol_get_checksum((U8Bit *)head, sizeof(FP_frame_head_t) - 1);
}


static S32Bit FP_protocol_get_complete_send_frame(FP_send_p send, FP_data_area_t data_area)
{   
    U16Bit data_area_len, frame_data_len; 
    U32Bit i;
    U8Bit  *p;
    
    data_area_len = FP_protocol_get_data_area_length(send);
    
    if ((send->cmd_type == cmd_fingerprint) &&(send->cmd_word == fp_write_ftr_data))
    {
         data_area_len = data_area.length;
    }
    
    if(data_area.length != data_area_len)
        return FP_PROTOCOL_SEND_DATA_LENGTH_ERROR;

    /* FP_frame_head_t */
    frame_data_len = data_area_len + sizeof(FP_send_t) - sizeof(FP_frame_head_t) + 1;
//    printf("sizeof(FP_send_t):%d\n", sizeof(FP_send_t));
//    printf("sizeof(FP_frame_head_t):%d\n", sizeof(FP_frame_head_t));
//    printf("frame_data_len:%02X\n", frame_data_len);
    FP_protocol_get_frame_head((FP_frame_head_p)send, frame_data_len);
//    printf("data_area_len:%02X\n", data_area_len);
//    printf("frame_data_len:%02X\n", frame_data_len);
    /* pwd */
    send->pwd = 0x00;

    /* data_area */
    p = (U8Bit*)send + sizeof(FP_send_t);
    if(data_area.data != 0 && data_area.length!= 0)
    {
        for(i = 0; i < data_area.length; i++)
        {
            *(p + i) = *(data_area.data + i);
        }
    }

    /* check sum */
    p = (U8Bit*)send;
    *(p + sizeof(FP_send_t) + data_area.length)  = FP_protocol_get_checksum(p + sizeof(FP_frame_head_t), frame_data_len - 1);
    
    return FP_OK;
}

/*
 * get fingerprint frame
 */
S32Bit FP_protocol_get_fp_enroll_start_frame(FP_send_p send, U8Bit index)
{
    FP_data_area_t data_area;
    U8Bit idx = index;

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_enroll_start;

    data_area.data = &idx;
    data_area.length = 1;
    
    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_enroll_result_frame(FP_send_p send)
{

    FP_data_area_t data_area;
    
    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_enroll_result;

    data_area.data = 0;
    data_area.length = 0;
    
    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_enroll_save_start_frame(FP_send_p send, U16Bit id)
{
    FP_data_area_t data_area;
    
    U8Bit buff[2];
    
    buff[0] = id/0x100;
    buff[1] = id%0x100;

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_enroll_save_start;

    data_area.data = buff;
    data_area.length = 2;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_enroll_save_result_frame(FP_send_p send)
{
    FP_data_area_t data_area;

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_enroll_save_result;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_enroll_cancel_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_enroll_cancel;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_match_start_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_match_start;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_match_result_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_match_result;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_delete_start_frame(FP_send_p send, S16Bit id)
{
    U8Bit buff[3]={0};
    FP_data_area_t data_area;

    if(id < 0) 
    {
        buff[0] = 0x01;
    } 
    else 
    {
        buff[0] = 0x00;
        buff[1] = id/0x100;
        buff[2] = id%0x100;
    }

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_delete_start;

    data_area.data = buff;
    data_area.length = 3;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_delete_result_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_delete_result;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_is_id_exist_frame(FP_send_p send, U16Bit id)
{
    U8Bit buff[2];
    FP_data_area_t data_area;

    buff[0] = id/0x100;
    buff[1] = id%0x100;
    
    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_is_fp_id_exist;

    data_area.data = buff;
    data_area.length = 2;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_get_ftr_info_frame(FP_send_p send, U16Bit id)
{
    U8Bit buff[2];
    FP_data_area_t data_field;

    buff[0] = id/0x100;
    buff[1] = id%0x100;

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_read_ftr_info;

    data_field.data = buff;
    data_field.length = 2;

    return FP_protocol_get_complete_send_frame(send, data_field);
}

S32Bit FP_protocol_get_fp_get_ftr_data_frame(FP_send_p send, U16Bit frame)
{
    U8Bit buff[2];
    FP_data_area_t data_field;

    buff[0] = frame/0x100;
    buff[1] = frame%0x100;

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_read_ftr_data;

    data_field.data = buff;
    data_field.length = 2;

    return FP_protocol_get_complete_send_frame(send, data_field);
}

S32Bit FP_protocol_get_fp_write_ftr_info_frame(FP_send_p send, U16Bit id, U16Bit length)
{
    U8Bit buff[4];
    FP_data_area_t data_field;

    buff[0] = id/0x100;
    buff[1] = id%0x100;

    buff[2] = length/0x100;
    buff[3] = length%0x100;

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_write_ftr_info;

    data_field.data = buff;
    data_field.length = 4;

    return FP_protocol_get_complete_send_frame(send, data_field);

}

S32Bit FP_protocol_get_fp_write_ftr_data_frame(FP_send_p send, U16Bit frame, U8Bit * data, U32Bit length)
{

    U8Bit buff[130];
    FP_data_area_t data_field;
    U32Bit i = 0;

    if(length > 128)
    {
        return FP_PROTOCOL_SEND_DATA_LENGTH_ERROR;
    }

    buff[0] = frame/0x100;
    buff[1] = frame%0x100;

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_write_ftr_data;

    for(i = 0; i < length; i++)
    {
        buff[i + 2] = *(data + i);
    }

    data_field.data = buff;
    data_field.length = length + 2;

    return FP_protocol_get_complete_send_frame(send, data_field);
}

S32Bit FP_protocol_get_fp_store_info_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_get_store_info;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_fp_is_touch_sensor_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_is_touch_sensor;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

/* 
 * get system frame
 */
S32Bit FP_protocol_get_sys_get_count_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_system;
    send->cmd_word = sys_get_count;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_get_sys_reset_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_system;
    send->cmd_word = sys_reset;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}


S32Bit FP_protocol_get_sys_sleep_frame(FP_send_p send, FP_sleep_type type)
{
    U8Bit buff[1];
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_system;
    send->cmd_word = sys_sleep;

    if(sleep_type_normal == type) 
    {
        buff[0] = 0x00;
    }
    else if(sleep_type_deep == type)
    {
        buff[0] = 0x01;
    }

    data_area.data = buff;
    data_area.length = 1;

    return FP_protocol_get_complete_send_frame(send, data_area);
}


S32Bit FP_protocol_get_sys_setled_frame(FP_send_p send, PFP_LED_CTRL_INFO p_stLEDCtrlInfo)
{
    U8Bit buff[5];
    FP_data_area_t data_area;

    send->cmd_type = cmd_system;
    send->cmd_word = sys_set_led;

    buff[0] = p_stLEDCtrlInfo->ucLEDCtrlMode;
    buff[1] = p_stLEDCtrlInfo->ucLEDColor;
    buff[2] = p_stLEDCtrlInfo->usLEDpara1;
    buff[3] = p_stLEDCtrlInfo->usLEDpara2;
    buff[4] = p_stLEDCtrlInfo->usLEDpara3;

    data_area.data = buff;
    data_area.length = 5;

    return FP_protocol_get_complete_send_frame(send, data_area);
}


S32Bit FP_protocol_get_sys_getpolicy_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_system;
    send->cmd_word = sys_get_syspolicy;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}


S32Bit FP_protocol_set_sys_getpolicy_frame(FP_send_p send, U32Bit *p_SysPolicy)
{
    U8Bit buff[4];
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_system;
    send->cmd_word = sys_set_syspolicy;

    buff[0] = (uint8_t)((*p_SysPolicy & 0xff000000) >> 24);
    buff[1] = (uint8_t)((*p_SysPolicy & 0x00ff0000) >> 16);
    buff[2] = (uint8_t)((*p_SysPolicy & 0x0000ff00) >> 8);
    buff[3] = (uint8_t)(*p_SysPolicy & 0x000000ff);

    data_area.data = buff;
    data_area.length = 4;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

/*
 * 
 */
S32Bit FP_protocol_get_fp_update_start_frame(FP_send_p send, S16Bit updateID)
{
    U8Bit buff[2];
    FP_data_area_t data_area;
 
    buff[0] = updateID/0x100;
    buff[1] = updateID%0x100;
    

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_update_start;

    data_area.data = buff;
    data_area.length = 2;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

/*
 * 
 */
S32Bit FP_protocol_get_fp_update_result_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_update_result;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}


S32Bit FP_protocol_fp_auto_enroll_frame(FP_send_p send , FP_auto_enroll_t enrollPara)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_auto_enroll;

    data_area.data = (U8Bit *)&enrollPara;
    data_area.length = sizeof(FP_auto_enroll_t);

    return FP_protocol_get_complete_send_frame(send, data_area);
}


S32Bit FP_protocol_fp_match_syn_frame(FP_send_p send)
{
    FP_data_area_t data_area;

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_match_syn;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}


S32Bit FP_protocol_fp_delete_syn_frame(FP_send_p send , S16Bit id)
{
    FP_data_area_t data_area;
    U8Bit buff[3]={0};

    if(id < 0)
    {
        buff[0] = 1;
    }
    else
    {
        buff[0] = 0;
        buff[1] = ((id & 0xff00) >> 8);
        buff[2] = (id & 0xff);
    }

    send->cmd_type = cmd_fingerprint;
    send->cmd_word = fp_delete_ftr_syn;

    data_area.data = buff;
    data_area.length = 3;

    return FP_protocol_get_complete_send_frame(send, data_area);
}



/*
 * get maintenace frame
 */
S32Bit FP_protocol_get_mtnce_read_id_frame(FP_send_p send)
{
    FP_data_area_t data_area;
    
    send->cmd_type = cmd_maintenance;
    send->cmd_word = maintenance_read_id;

    data_area.data = 0;
    data_area.length = 0;

    return FP_protocol_get_complete_send_frame(send, data_area);
}

S32Bit FP_protocol_send_mesg(FP_send_p send, U32Bit timeout)
{
    U32Bit length = sizeof(FP_send_t) + FP_protocol_get_data_area_length(send) + 1;

    return FP_device_write_data((unsigned char *)send, length, timeout);
}


S32Bit FP_protocol_send_mesg_ex(FP_send_p send, U32Bit length, U32Bit timeout)
{
    U32Bit sent_length = sizeof(FP_send_t) + length + 1;

    return FP_device_write_data((unsigned char *)send, sent_length, timeout);
}



static S32Bit FP_protocol_recv_forhead(FP_recv_p recv, U32Bit timeout)
{
    S32Bit ret;
    U8Bit * p = (U8Bit *)recv;
    U32Bit i = 0;

    while(1)
    {
        for(i = 0; i < sizeof(s_forhead); i++)
        {
            if(FP_OK != (ret = FP_device_read_one_byte(p + i, timeout)))
                return ret;
            else if(*(p + i) != s_forhead[i])
                break;
        }

        if(sizeof(s_forhead) == i)
            break;
    }

    return FP_OK;
}


/*
 --------------------------------------------------------------------------------------------------------------------
 |                         |            |          |              |          |          |               |          |
 |          forhead        |            |          |              |          |          |               |          |
 |                         |   length   | checksum |     pwd      | cmd_type | cmd_word |   data_area   | checksum |
 | F1 1F E2 2E B6 6B A8 8A |            |          |              |          |          |               |          |
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
 |                         |            |          |              |          |          |           |           |          |
 | F1 1F E2 2E B6 6B A8 8A |   length   | checksum |   password   | cmd_type | cmd_word | errorCode | data_area | checksum |
 |                         |            |          |              |          |          |           |           |          |
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


S32Bit FP_protocol_recv_complete_frame(FP_recv_p recv, FP_data_area_p data_area, U32Bit timeout)
{
    S32Bit ret = 0;
    U32Bit frame_data_length = 0, tmp_len = 0;

    /* forhead */
    if(FP_OK != (ret = FP_protocol_recv_forhead(recv, timeout)))
        return ret;

    /* FP_frame_head_t */
    ret = FP_device_read_data((U8Bit *)recv + sizeof(s_forhead), \
        sizeof(FP_frame_head_t) - sizeof(s_forhead), timeout);
    if(FP_OK != ret)
        return ret;

    /* frame_head checksum */
    ret = FP_protocol_checkout_recv_head_checksum((FP_frame_head_p)recv);
    if(FP_OK != ret)
        return ret;

    /* length */
    tmp_len = recv->frame_head.length;
    frame_data_length = (U32Bit)(*(U8Bit *)&tmp_len) * 0x100 + *((U8Bit *)&tmp_len + 1);

    /* FP_recv_t and data_area and check sum */
    ret = FP_device_read_data((U8Bit *)recv + sizeof(FP_frame_head_t), frame_data_length, timeout);
    if(FP_OK != ret)
        return ret;

    /*second checksum */ 
    ret = FP_protocol_checkout_recv_data_checksum(recv);
    if(FP_OK != ret)
        return ret;

    data_area->data = (U8Bit *)recv + sizeof(FP_recv_t);
    data_area->length = FP_protocol_get_recv_data_length(recv);

    return FP_OK;
}

