
#include "fingerprint_protocol.h"
#include "fingerprint_action.h"

#include <string.h>
#include <stdio.h>

static FP_send_p s_send_p = 0;
static FP_recv_p s_recv_p = 0;

void ASSERT_BUFFER(void)
{
	//clear sent and recv buff
    memset(s_send_p, 0, sizeof(FP_send_t));
    memset(s_recv_p, 0, sizeof(FP_recv_t));
//    printf("s_send_p all data: ");
//    for (int i = 0; i < 256; i++) {
//        printf("%02X ", ((unsigned char*) s_send_p)[i]);
//    }
//    printf("\n");
//    printf("clear buffer\n");
//    return 0;
}

void FP_action_set_send_buffer(U8Bit *buffer)
{
    s_send_p = (FP_send_p)buffer;
}

void FP_action_set_recv_buffer(U8Bit *buffer)
{
    s_recv_p = (FP_recv_p)buffer;
}

static S32Bit FP_action_get_errorCode(U8Bit * buffer)
{
    S32Bit errorCode;
    errorCode =  (U32Bit)buffer[0] << 24;
    errorCode += (U32Bit)buffer[1] << 16;
    errorCode += (U32Bit)buffer[2] << 8;
    errorCode += (U32Bit)buffer[3];

    return errorCode;
}

S32Bit FP_action_getID(FP_moduleID_p moduleID, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    /* get a frame to send */
    ret = FP_protocol_get_mtnce_read_id_frame(s_send_p);
//    printf("s_send_p all data: ");
//    for (int i = 0; i < 256; i++) {
//        printf("%02X ", ((unsigned char*) s_send_p)[i]);
//    }
//    printf("\n");
    if(FP_OK != ret)
        return ret;

    /* send */
    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    /* receive the responce frame */
    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    /* get the useful data */
    moduleID->ID = data_area.data;
    moduleID->length = data_area.length;

    /* error code */
    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);
    
    return FP_OK;
}

S32Bit FP_action_check_finger_is_touch(U8Bit * isTouch, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_is_touch_sensor_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;
        
    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;
    
    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);
    *isTouch = *data_area.data;

    return FP_OK;
}


S32Bit FP_action_enroll_start(U8Bit index, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_enroll_start_frame(s_send_p, index);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;
        
    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}

S32Bit FP_action_get_enroll_result(FP_enrollResult_p enrollResult, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_enroll_result_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;
        
    enrollResult->fingerID = (U16Bit)*(data_area.data + 0) * 0x100 + (U16Bit)*(data_area.data + 1);
    enrollResult->progress = (U8Bit)*(data_area.data + 2);
    
    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}

S32Bit FP_action_save_start(S16Bit id, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_enroll_save_start_frame(s_send_p, id);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}

S32Bit FP_action_get_save_result(U32Bit * errorCode, U16Bit *SaveID)
{
    FP_data_area_t data_area;
    S32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_enroll_save_result_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

	*SaveID = (U16Bit)*(data_area.data + 0) * 0x100 + (U16Bit)*(data_area.data + 1);

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}


S32Bit FP_action_match_start(U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_match_start_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}

S32Bit FP_action_get_match_result(FP_matchResult_p matchResult, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_match_result_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    matchResult->isPass = data_area.data[1];
    matchResult->matchScore = (U16Bit)*(data_area.data + 2) * 0x100 + (U16Bit)*(data_area.data + 3);
    matchResult->matchID = (U16Bit)*(data_area.data + 4) * 0x100 + (U16Bit)*(data_area.data + 5);
    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}

S32Bit FP_action_delete_start(S16Bit id, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_delete_start_frame(s_send_p, id);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}

S32Bit FP_action_get_delete_result(U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_delete_result_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}


S32Bit FP_action_sleep(FP_sleep_type type, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_sys_sleep_frame(s_send_p, type);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;
        
    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}


S32Bit FP_action_set_led(PFP_LED_CTRL_INFO p_stLEDCtrlInfo, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_sys_setled_frame(s_send_p, p_stLEDCtrlInfo);

    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;
        
    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}


S32Bit FP_action_get_SysPolicy(U32Bit *p_SysPolicy, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_sys_getpolicy_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;
        
    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    *p_SysPolicy = (data_area.data[0] << 24)
           | (data_area.data[1] << 16)
           | (data_area.data[2] << 8)
           | (data_area.data[3]);

    return FP_OK;
}


S32Bit FP_action_set_SysPolicy(U32Bit *p_SysPolicy, U32Bit * errorCode, U32Bit timeout)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_set_sys_getpolicy_frame(s_send_p, p_SysPolicy);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;
        
    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, timeout);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}


S32Bit FP_action_reset(U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_sys_reset_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;
        
    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;

}

/*更新特征值*/
S32Bit FP_action_update_start(S16Bit updateID, U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_update_start_frame(s_send_p, updateID);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}

S32Bit FP_action_get_update_result(U32Bit * errorCode)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_update_result_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}



S32Bit FP_action_auto_enroll_send(FP_auto_enroll_t enrollPara)
{
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_fp_auto_enroll_frame(s_send_p , enrollPara);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    return FP_OK;
}


S32Bit FP_action_auto_enroll_recv(FP_enrollResult_p enrollResult, U32Bit * errorCode, U32Bit timeout)
{
    U32Bit ret;
    FP_data_area_t data_area;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, timeout);
    if(FP_OK != ret)
        return ret;

    enrollResult->state = (U8Bit)*(data_area.data + 0);
    enrollResult->fingerID = (U16Bit)*(data_area.data + 1) * 0x100 + (U16Bit)*(data_area.data + 2);
    enrollResult->progress = (U8Bit)*(data_area.data + 3);
    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);
    return FP_OK;
}


S32Bit FP_action_match_syn_send(void)
{
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_fp_match_syn_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    return FP_OK;
}


S32Bit FP_action_match_syn_recv(FP_matchResult_p matchResult, U32Bit * errorCode, U32Bit timeout)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, timeout);
    if(FP_OK != ret)
        return ret;

    matchResult->isPass = data_area.data[1];
    matchResult->matchScore = (U16Bit)*(data_area.data + 2) * 0x100 + (U16Bit)*(data_area.data + 3);
    matchResult->matchID = (U16Bit)*(data_area.data + 4) * 0x100 + (U16Bit)*(data_area.data + 5);
    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}


S32Bit FP_action_delete_syn_send(S16Bit id)
{
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_fp_delete_syn_frame(s_send_p, id);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    return FP_OK;
}

S32Bit FP_action_delete_syn_recv(U32Bit * errorCode, U32Bit timeout)
{
    FP_data_area_t data_area;
    U32Bit ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_area, timeout);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}


S32Bit FP_action_get_ftr_distribution(FP_ftrDistribution_p distribution_p, U32Bit * errorCode)
{
    FP_data_area_t data_field;
    U32Bit ret;
    U32Bit i;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_store_info_frame(s_send_p);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_field, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    distribution_p->ftrMaxCount = (U32Bit)*(U8Bit*)data_field.data * 0x100;
    distribution_p->ftrMaxCount += (U32Bit)*((U8Bit*)data_field.data + 1);

    for(i = 0; i < data_field.length; i++)
    {
        distribution_p->distributon[i] = *(data_field.data + i + 2);
    }

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}

S32Bit FP_action_get_ftr_info(FP_ftrData_p uploadftrData_p, U32Bit * errorCode)
{
    FP_data_area_t data_field;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_get_ftr_info_frame(s_send_p, uploadftrData_p->id);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_field, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    uploadftrData_p->length = *(U8Bit*)data_field.data * 0x100;
    uploadftrData_p->length += *((U8Bit*)data_field.data + 1);

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}

S32Bit FP_action_get_ftr_data(U16Bit frame, U8Bit *data, U32Bit * errorCode)
{
    FP_data_area_t data_field;
    U32Bit ret;
    U16Bit recvFrame;
    U32Bit i;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_get_ftr_data_frame(s_send_p, frame);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_field, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    recvFrame = *(U8Bit*)data_field.data * 0x100;
    recvFrame += *((U8Bit*)data_field.data + 1);

    if(frame != recvFrame)
    {
        return FP_PROTOCOL_COMMAND_NOT_MATH_ERROR;
    }

    for(i = 0; i < data_field.length - 2; i++)
    {
        *(data + i) = *(data_field.data + i + 2);
    }

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}


S32Bit FP_action_write_ftr_info(FP_ftrData_p downloadftrData_p, U32Bit * errorCode)
{
    FP_data_area_t data_field;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_write_ftr_info_frame(s_send_p, downloadftrData_p->id, downloadftrData_p->length);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg(s_send_p, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_field, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;

}


S32Bit FP_action_write_ftr_data(U16Bit frame, U8Bit * data, U32Bit length, U32Bit *errorCode, U32Bit timeout)
{
    FP_data_area_t data_field;
    U32Bit ret;

    ASSERT_BUFFER();

    ret = FP_protocol_get_fp_write_ftr_data_frame(s_send_p, frame, data, length);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_send_mesg_ex(s_send_p, length+2, DEFAULT_TIME_OUT);
    if(FP_OK != ret)
        return ret;

    ret = FP_protocol_recv_complete_frame(s_recv_p, &data_field, timeout);
    if(FP_OK != ret)
        return ret;

    *errorCode = FP_action_get_errorCode(s_recv_p->error_code);

    return FP_OK;
}

