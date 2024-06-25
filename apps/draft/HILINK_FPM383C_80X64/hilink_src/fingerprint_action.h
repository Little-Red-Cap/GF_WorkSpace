
#ifndef FINGERPRINT_ACTION_H
#define FINGERPRINT_ACTION_H

#include "fingerprint_type.h"
#include "fingerprint_protocol.h"

#ifdef __cplusplus   
extern "C" {   
#endif

#define DEFAULT_TIME_OUT   (100) //超时时间根据不同的命令去设定

void FP_action_set_send_buffer(U8Bit *buffer);
void FP_action_set_recv_buffer(U8Bit *buffer);

/* 
 * module ID
 */
S32Bit FP_action_getID(FP_moduleID_p moduleID, U32Bit * errorCode);

/*
 * finger touch
 */
S32Bit FP_action_check_finger_is_touch(U8Bit * isTouch, U32Bit * errorCode);

/*
 * enroll
 */
S32Bit FP_action_enroll_start(U8Bit index, U32Bit * errorCode);
S32Bit FP_action_get_enroll_result(FP_enrollResult_p enrollResult, U32Bit * errorCode);

/*
 * save
 */
S32Bit FP_action_save_start(S16Bit id, U32Bit * errorCode);

S32Bit FP_action_get_save_result(U32Bit * errorCode, U16Bit *SaveID);


/*
 * delete
 */
S32Bit FP_action_delete_start(S16Bit id, U32Bit * errorCode);
S32Bit FP_action_get_delete_result(U32Bit * errorCode);

/*
 * match
 */
S32Bit FP_action_match_start(U32Bit * errorCode);
S32Bit FP_action_get_match_result(FP_matchResult_p matchResult, U32Bit * errorCode);


/*
 * sleep
 */
S32Bit FP_action_sleep(FP_sleep_type type, U32Bit * errorCode);


/*
 * reset
 */
S32Bit FP_action_reset(U32Bit * errorCode);

/*
 * update
 */
S32Bit FP_action_update_start(S16Bit updateID, U32Bit * errorCode);
S32Bit FP_action_get_update_result(U32Bit * errorCode);

/*
 * auto enroll
 */
S32Bit FP_action_auto_enroll_send(FP_auto_enroll_t enrollPara);
S32Bit FP_action_auto_enroll_recv(FP_enrollResult_p enrollResult, U32Bit * errorCode, U32Bit timeout);

/*
 * match syn
 */
S32Bit FP_action_match_syn_send(void);
S32Bit FP_action_match_syn_recv(FP_matchResult_p matchResult, U32Bit * errorCode, U32Bit timeout);


/*
 * delete syn
 */
S32Bit FP_action_delete_syn_send(S16Bit id);
S32Bit FP_action_delete_syn_recv(U32Bit * errorCode, U32Bit timeout);

/*
 * uploadFTR
 */

S32Bit FP_action_get_ftr_distribution(FP_ftrDistribution_p distribution, U32Bit * errorCode);
S32Bit FP_action_get_ftr_info(FP_ftrData_p uploadftrData_p, U32Bit * errorCode);
S32Bit FP_action_get_ftr_data(U16Bit frame, U8Bit *data, U32Bit * errorCode);
S32Bit FP_action_write_ftr_info(FP_ftrData_p downloadftrData_p, U32Bit * errorCode);
S32Bit FP_action_write_ftr_data(U16Bit frame, U8Bit * data, U32Bit length, U32Bit *errorCode, U32Bit timeout);

/*
 * setled
 */
S32Bit FP_action_set_led(PFP_LED_CTRL_INFO p_stLEDCtrlInfo, U32Bit * errorCode);


S32Bit FP_action_get_SysPolicy(U32Bit *p_SysPolicy, U32Bit * errorCode);
S32Bit FP_action_set_SysPolicy(U32Bit *p_SysPolicy, U32Bit * errorCode, U32Bit timeout);


#ifdef __cplusplus   
}   
#endif


#endif //FINGERPRINT_ACTION_H

