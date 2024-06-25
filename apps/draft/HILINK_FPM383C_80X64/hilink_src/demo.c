#include <string.h>
#include <stdio.h>
#include "fingerprint_action.h"
#include "fingerprint_type.h"
#include "fingerprint_protocol.h"

#include <windows.h>

static U8Bit s_recv_buffer[256];
static U8Bit s_send_buffer[256];
static U8Bit s_ftrDataBuffer[8196];        //待上传下载的指纹数据buffer
static U32Bit s_ftrDataLength = 0;          //待上传下载的指纹数据buffer长度

#define CHECK_BIT(value, x) ((value) & (1 << (x)))

void Delay_ms(U32Bit delaytime)
{
//     U32Bit i;
//     U32Bit j;
//
//     for (j = 0; j < delaytime; j++)
//     {
//         for (i = 0; i < 10000; i++)
//         {
//             //根据不同的开发环境实现延时
//         }
//     }
//     i=j;
    Sleep(delaytime);
}


void getID(void)
{
    U32Bit errorCode;
	S32Bit ret;

    FP_moduleID_t moduleID;

    ret = FP_action_getID(&moduleID, &errorCode);
    printf("getID ret = %d, errorCode = %d\n", ret, errorCode);
    if(FP_OK == ret && 0 == errorCode)
    {
        //get ID ok
        printf("OK\n");
        printf("moduleID = %08X\n", moduleID);
    }
}

void enroll(void)
{
    U32Bit errorCode;
	S32Bit ret;
    U8Bit index = 1;
    U8Bit isTouch;
	U16Bit SaveID;

    FP_enrollResult_t enrollResult;
    enrollResult.fingerID = 0;
    enrollResult.progress = 0;

    //第一次直接enrollStart
    goto EnrollStartLabel;

    //
    //每次enroll过后，确保手指要抬起来以后，再次按压
FingerLeaveLabel:               
    FP_action_check_finger_is_touch(&isTouch, &errorCode);
    if(0 != isTouch || COMP_CODE_OK != errorCode)
    {
        // 当isTouch不是0的时候，意味着手指在上，
        //提示用户手指离开sensor再次按压
        //printf("lift your finger please !");
        //延时 200 ms
		Delay_ms(200);
        goto FingerLeaveLabel;
    }

    //开始注册
EnrollStartLabel:
    ret = FP_action_enroll_start(index, &errorCode);
    if(COMP_CODE_OK != errorCode || FP_OK != ret)
    {
	    Delay_ms(100);
		//可能上电初始化未完成或睡眠唤醒初始化中
		goto EnrollStartLabel;
	}
	else
	{
		//可延时100ms后发送获取注册结果命令，采图后需要大概这么长时间处理
		Delay_ms(100);
	}
    
    //获取注册结果
EnrollResultLabel:
    ret = FP_action_get_enroll_result(&enrollResult, &errorCode);
    if(FP_DEVICE_TIMEOUT_ERROR == ret)
	{
		//接受数据超时，根据接受数据所设超时时间适当延时。
		Delay_ms(100);
		goto EnrollResultLabel;
	}
    
    if(COMP_CODE_OK == errorCode)
    {
        //如果errorCode 是COMP_CODE_OK,说明本次enroll执行完成，
        //此时可以查看enrollResult.progress是否增加，如果增加，说明本次enroll成功

        //如果progress >= 100 ，说明整个注册流程成功结束，开始根据enrollResult.fingerID保存指纹
        if(enrollResult.progress >= 0x64)
        {
            goto SaveStartLabel;
        }
        else
        {
            //如果progress < 100, 手指离开sensor，再次按压，继续注册
            index++;
            //延时 100 ms
            Delay_ms(100);
            goto FingerLeaveLabel;
        }
    }
    else if(COMP_CODE_CMD_NOT_FINISHED == errorCode)
    {
        //errorCode == COMP_CODE_CMD_NOT_FINISHED意味着sensor还没有处理完指纹数据或没有按压手指，
        //适当延时，再次获取结果
        //延时 100 ms
        Delay_ms(100);
        goto EnrollResultLabel;
    }
    else if(COMP_CODE_SAME_ID == errorCode)
    {
        //errorCode == COMP_CODE_SAME_ID意味着与已注册指纹重复，需换手指注册
        //适当延时，再次获取结果
        //延时 100 ms
        Delay_ms(100);
        goto EnrollResultLabel;
    }
	else if(COMP_CODE_NO_FINGER_DETECT == errorCode)
	{
		//errorCode == COMP_CODE_NO_FINGER_DETECT意味着超时还没按压手指
        //重新注册
	}
	else if(COMP_CODE_OTHER_ERROR == errorCode)
	{
         goto EnrollStartLabel;
	}
	else if(COMP_CODE_NO_REQ_CMD == errorCode)
	{
		goto EnrollStartLabel;
	}
	else
    {
        //图像质量不好，手指可能按压过重、过轻、或者太过潮湿等，继续enrollStart即可，也可根据Elock用户手册细化处理
        //延时 200 ms
    	Delay_ms(200);
        goto EnrollStartLabel;
    }

    //保存指纹开始
    //enrollResult.fingerID会根据模组回复的推荐id去保存,编号从00开始
SaveStartLabel:
    ret = FP_action_save_start(enrollResult.fingerID, &errorCode);
    if(COMP_CODE_OK != errorCode || FP_OK != ret)
    {
	    Delay_ms(100);
		goto SaveStartLabel;
	}

	Delay_ms(200);
    //获取保存指纹结果
SaveResultLabel://【保存指纹开始命令】发送后，模组需操作flash，这期间大概200ms发送【获取保存指纹结果】没有数据回复，可超时重发
    ret = FP_action_get_save_result(&errorCode, &SaveID);
	if(FP_DEVICE_TIMEOUT_ERROR == ret)
	{
		//接受数据超时，根据接受数据所设超时时间适当延时。
		Delay_ms(100);
		goto SaveResultLabel;
	}
	
    if(COMP_CODE_OK == errorCode)
    {
    	//查看保存成功的SaveID
        //保存完成
    }
    else if(COMP_CODE_CMD_NOT_FINISHED == errorCode)
    {
        //还未保存完成，延时适当时间，再次获取结果
        //延时 100 ms
        Delay_ms(100);
        goto SaveResultLabel;
    }
    else if(COMP_CODE_STORAGE_IS_FULL == errorCode)
    {
        //flash存储已满，不能保存指纹
    }
    else
    {
        //其他错误，比如硬件错误等。
    }

    return;
}

void match(void)
{
    U32Bit errorCode;
    S32Bit ret;
    U8Bit isTouch;
    FP_matchResult_t matchResult;

/***********************
为了提速，也可以不先检查手指在位，
不同的模组回复时间不同(FPM08X系列模组大约耗时30ms左右)
************************/
checkFingeronLabel:
    FP_action_check_finger_is_touch(&isTouch, &errorCode);
    if((0 != isTouch) && (COMP_CODE_OK == errorCode))
    {
        //当isTouch是1的时候，意味着手指在上，
        //播放语音“滴”
    }
    else
    {
        //延时 50 ms
        Delay_ms(50);
        goto checkFingeronLabel;
    }
/**********************/    
matchStartLabel:
    //开始match
    ret = FP_action_match_start(&errorCode);
	if(COMP_CODE_OK != errorCode || FP_OK != ret)
	{
		//延时 50 ms
		//可能上电初始化未完成或睡眠唤醒初始化中
        Delay_ms(50);
		goto matchStartLabel;
	}

	//匹配处理固定大于300ms;
    Delay_ms(300);
    //获取注册结果
matchResultLabel:
    ret = FP_action_get_match_result(&matchResult,&errorCode);
    if(FP_DEVICE_TIMEOUT_ERROR == ret)
	{
		//接受数据超时，根据接受数据所设超时时间适当延时。
		Delay_ms(100);
		goto matchResultLabel;
	}
    
    if(COMP_CODE_OK == errorCode)
    {
        //match动作完成

        /*  typedef struct {
         *      U16Bit isPass;
         *      U16Bit matchScore;
         *      U16Bit matchID;
         * } FP_matchResult_t, *FP_matchResult_p;
         */

        // isPass是1的话，代表已经匹配到了指纹，这时，matchID是匹配到的指纹ID
        // matchScore是具体匹配的分数

        if(1 == matchResult.isPass)
        {
            //匹配到指纹
            printf("match success, matchID = %d, matchScore = %d\n", matchResult.matchID, matchResult.matchScore);
        }
        else
        {
            //未匹配到指纹
        }

    }
    else if(COMP_CODE_CMD_NOT_FINISHED == errorCode)
    {
        //还未匹配完成，延时适当时间，再次获取结果,处理指纹需要一些时间
        //延时 30 ms
	    Delay_ms(30);
        goto matchResultLabel;
    }
	else if(COMP_CODE_NO_FINGER_DETECT == errorCode)
	{
		//errorCode == COMP_CODE_NO_FINGER_DETECT意味着超时还没按压手指
        //重新匹配
	}
    else if(COMP_CODE_NO_REQ_CMD == errorCode)
    {
        goto matchStartLabel;
	}
    else if(COMP_CODE_HARDWARE_ERROR == errorCode)
    {
        //延时 200 ms
        Delay_ms(200);
        goto matchStartLabel;
    }
	else
    {
        return;
    }

}


/***匹配成功后执行更新指纹特征（模组自学习），有利于优化体验*
    该命令可在匹配成功以后直接发送，模组自身会判断是否要自学习，
    如果不需要会返回错误码
**/
void updateFeature(void)
{
    U32Bit errorCode;
    S32Bit ret;

    S32Bit updateID = 0;//传入的ID必须与匹配成功后的ID一致

updatestartLabel:
    ret = FP_action_update_start(updateID, &errorCode);
    if(COMP_CODE_OK == errorCode)
    {	
        //需要更新，延时100毫秒
        Delay_ms(100);
    }
    else
    {
       //不需要更新，直接结束
        return;
    }

updateResultLabel:
    ret = FP_action_get_update_result(&errorCode);
    if(ret == FP_DEVICE_TIMEOUT_ERROR)
    {
        Delay_ms(100);
        goto updateResultLabel;
    }
    
    if(COMP_CODE_OK == errorCode)
    {
    	//更新成功；
    } 
    else if(COMP_CODE_CMD_NOT_FINISHED == errorCode)
    {
        //适当延时100ms，再次获取
        Delay_ms(100);
        goto updateResultLabel;
    }
    else
    {
        //更新失败,重新更新，或者退出
        goto updatestartLabel;
    }
		
    return;
}


void deleteFp(void)
{
    U32Bit errorCode;
    S32Bit ret;
    S16Bit delete_id = -1;

    //开始删除，delete_id > 0 的时候，删除指定id的指纹。
    //delete_id == -1的时候，删除所有指纹，
 deleteStartLabel:
    ret = FP_action_delete_start(delete_id, &errorCode);
	if(COMP_CODE_OK != errorCode || FP_OK != ret)
	{
		//延时 100 ms
		//可能上电初始化未完成或睡眠唤醒初始化中
		Delay_ms(100);
		goto deleteStartLabel;
	}

    //获取删除结果
deleteResultLabel:
	Delay_ms(100);      //延时 100 ms
    ret = FP_action_get_delete_result(&errorCode);
    if(ret == FP_DEVICE_TIMEOUT_ERROR)
    {
        goto deleteResultLabel;
    }
	
    if(COMP_CODE_OK == errorCode)
    {
        //删除成功
    }
    else if(COMP_CODE_CMD_NOT_FINISHED == errorCode)
    {
        //还未删除完成，再次获取结果
        goto deleteResultLabel;
    }
    else
    {
        //其他错误，比如指纹ID不存在等、flash硬件错误等。
    }

    return;
}

void sleep(void)
{
	 U32Bit errorCode;
     S32Bit ret;

SleepStartLabel:
    ret = FP_action_sleep(sleep_type_normal, &errorCode);
    if(FP_OK == ret && 0 == errorCode)
    { 
        //休眠成功，模组进入休眠
    }
    else
    {
        //延时 200 ms, 继续发送休眠命令，直到回复休眠成功为止
		Delay_ms(200);
        goto SleepStartLabel;
    }
}


void autoEnroll(void)
{
    U32Bit errorCode;
	S32Bit ret;
    FP_auto_enroll_t enrollPara;
	FP_enrollResult_t EnrollResult;
    U32Bit timeout = 10000; //建议超时时间设为10s
    
	enrollPara.fingerID_high = 0xFF;  //可修改ID，若 ID 为 0xFFFF 则由系统自动分配指纹 ID 。
	enrollPara.fingerID_low = 0xFF;  //可修改ID，若 ID 为 0xFFFF 则由系统自动分配指纹 ID 。
	
	enrollPara.enroll_mode = 0x01; //设置为 1则表示按压后需要等待手指抬起再次才可以进行下次注册，设置为0则不需要。
	enrollPara.times = 3; 		 //可修改注册次数，范围：1~6次

SendEnrollStartLabel:
	ret = FP_action_auto_enroll_send(enrollPara);
    if(FP_OK != ret)
    {
        goto SendEnrollStartLabel;
    }

GetEnrollResultLabel:
    ret = FP_action_auto_enroll_recv( &EnrollResult , &errorCode, timeout);  //处理返回码
    if(0 != ret)
    {
        //可能在等待手指按压，或者正在处理注册，再重新查询是否接收到
        goto GetEnrollResultLabel;
    }
    
    if(errorCode == COMP_CODE_CMD_DATA_LEN_ERROR || errorCode == COMP_CODE_CMD_DATA_ERROR)
    {
        //检查命令是否发错，重新开始注册
        goto SendEnrollStartLabel;
    }

    if(errorCode == COMP_CODE_STORAGE_IS_FULL)
    {
        //指纹容量已满，退出
        return;
    }

    if((100 == EnrollResult.progress) && (EnrollResult.state == 0xff))
    {
        if(COMP_CODE_OK == errorCode)
        {
            // 进度100，state=0XFF时才认为注册成功
            // 注册成功
        }
        else
        {
            //存储出了问题，重新开始注册
            goto SendEnrollStartLabel;
        }
    }
    else
    {
        if(errorCode == COMP_CODE_OK)
        {
            //单次注册成功，继续按压
        }
        else if(errorCode == COMP_CODE_SAME_ID)
        {
            //重复指纹，可提示用户换手指按压
        }
        else if(errorCode == COMP_CODE_UNQUALIFIED_IMAGE_ERROR)
        {
            //采图质量不好，继续按压直到成功
        }
        else if(errorCode == COMP_CODE_NO_FINGER_DETECT)
        {
            //8S超时还没按压手指，继续按压直到注册成功；
        }
        else
        {
            //其他问题，继续按压
        }
            
        //继续获取下一次结果
        goto GetEnrollResultLabel;
    }
    
    return;
}


void matchSyn(void)
{
    U32Bit errorCode;
	S32Bit ret;
    U32Bit timeout = 10000; //建议超时时间设为10s
    FP_matchResult_t matchResult;
    
matchSynSendLabel:
    ret = FP_action_match_syn_send();
    if(FP_OK != ret)
    {
        goto matchSynSendLabel;
    }

getMatchSynResultLabel:
    ret = FP_action_match_syn_recv(&matchResult, &errorCode, timeout);
    if(FP_DEVICE_TIMEOUT_ERROR == ret)
    {
        //可能在等待手指按压，或者正在处理匹配，再重新查询是否接收到
        goto getMatchSynResultLabel;
    }
    
    if((FP_OK == ret) && (COMP_CODE_OK == errorCode))
    {
        if(1 == matchResult.isPass)
        {
            //匹配成功
            //同步匹配会自带自学习功能，所以耗时会相对于分步匹配长一点
        }
        else
        {
            //匹配失败
        }
    }
    else if(COMP_CODE_NO_FINGER_DETECT == errorCode)
    {
        //没有手指按压
    }
    else if(COMP_CODE_UNQUALIFIED_IMAGE_ERROR == errorCode)
    {
        //采图质量不佳
        goto matchSynSendLabel;
    }
    else
    {
        //其他错误
    }
}


void deleteSyn(void)
{
    U32Bit errorCode;
    S32Bit ret;
    U32Bit timeout = 1000;         //建议超时时间设为1s
    S16Bit delete_id = -1;

    //delete_id > 0 的时候，删除指定id的指纹。
    //delete_id == -1的时候，删除所有指纹，

 SendDeleteStartLabel:
    ret = FP_action_delete_syn_send(delete_id);
    if(FP_OK != ret)
    {
        goto SendDeleteStartLabel;
    }

GetDeleteResultLabel:
    ret = FP_action_delete_syn_recv(&errorCode, timeout);
    if(ret == FP_DEVICE_TIMEOUT_ERROR)
    {
        goto GetDeleteResultLabel;
    }
    
    if((FP_OK == ret) && (COMP_CODE_OK == errorCode))
    {
        //删除成功
    }
    else
    {
        //其他错误，比如指纹ID不存在等、flash硬件错误等。
    }

    return;
}


/**********************************************************************************************
上传指纹特征:适用于FPM08X系列软件版本大于118的模组(如版本号为：FPM083-00-118);

一般的，分为三种应用场景：
    1、上传模组内正在注册的id：替代保存指纹命令（0x13）+ 查询保存结果命令（0x14），
        直接将指纹数据上传至上位机；

    2、上传模组内存储的指定一个id：上传之前，需确认id是否存在，如果存在则继续上传；
        
    3、上传模组内存储的所有id；上传之前，需获取指纹模板分布，然后根据分布指定id号，一个一个上传；

*********************************************************************************************/

void uploadFTR(void)
{
    FP_ftrData_t uploadftrData;
    FP_ftrDistribution_t distribution;
    U32Bit errorCode;
    S32Bit ret;
    U16Bit ftrLength;
    S32Bit i = 0;
    U16Bit frameCount; 
    U8Bit ftrReceiveBuffer[128];        //每帧命令回复的buffer
    U8Bit err_num = 0;

/*
    typedef struct {
        U16Bit id;          //待上传的指纹id
        U16Bit length;      //待上传的指纹数据长度
        U8Bit  *data;       
    }FP_ftrData_t, *FP_ftrData_p;
*/

    uploadftrData.data = s_ftrDataBuffer;
    uploadftrData.id = 0x00;

/******上传模组内正在注册的id*************
    //注册进度100%
    //保存指纹开始
    uploadftrData.id = 0xFFFF;      //当id为0XFFFF时，可直接上传
    goto UploadFtrGetInfoLabel;
*****************************************/

UploadFtrCheckFtrLabel:
    ret = FP_action_get_ftr_distribution(&distribution, &errorCode);
    if(FP_OK != ret || errorCode != FP_OK)
    {
        //错误，重新发送  
        Delay_ms(100);
        goto UploadFtrCheckFtrLabel;
    }

/*****************上传模组内存储的指定一个id**********/
//  Printf("distribution[] = 0x%x\r\n", distribution.distributon[check_id >> 3]);

    if(0 == CHECK_BIT(distribution.distributon[uploadftrData.id >> 3], (uploadftrData.id & 0x7)))       
    {
        //id不存在，重新输出正确id号
        Delay_ms(100);
        goto UploadFtrCheckFtrLabel;
    }
/****************************************************/
    
/*********上传模组内存储的所有id********************
    S16Bit upload_id[100] = 0;  //待上传的id,用于上传所有指纹
    U8Bit currentByte = 0;
    S16Bit *p = &upload_id[0];
    
    for(int i = 0; i <= (distribution.ftrMaxCount >> 3); i++)
    {
        currentByte = distribution.distributon[i];
        for(int j = 0; j < 8; j++)
        {
            if(CHECK_BIT(currentByte,j))
            {
                *p = i*8 + j;     
                p++;
            }
        }
    }
**************************************************/
UploadFtrGetInfoLabel:
    ret= FP_action_get_ftr_info(&uploadftrData, &errorCode);
    if( FP_OK != ret)
    {
        Delay_ms(100);
        goto UploadFtrGetInfoLabel;
    }
    
    ftrLength = uploadftrData.length; 
    frameCount = ((ftrLength - 1) >> 7) + 1;   
    
UploadFtrGetDataLabel:
    for(; i < frameCount; i++)
    {
        ret = FP_action_get_ftr_data(i, ftrReceiveBuffer, &errorCode);
        if(ret != FP_OK || errorCode != FP_OK)
        {
            err_num++;
            if(err_num < 3)
            {
                //数据接收有误重新发送该帧
                goto UploadFtrGetDataLabel;
            }
            else
            {
                //如果超过3次,报错退出
                return;
            }
        }
        else
        {
            err_num = 0;
            memcpy(uploadftrData.data + 128*i, ftrReceiveBuffer, 128);    //模组每帧回复的数据长度其实是固定的128字节
        }
    }

    s_ftrDataLength = ftrLength;
    
    /*
        FTR存储在上位机数据库前
        需对ftrData的长度ftrLength进行记录，作为该指纹下载时的长度；
        可把ftrData打包一下（加一个CRC或奇偶校验）
    */
}


/*********************************************************************************************
下载指纹特征:适用于FPM08X系列软件版本大于118的模组(如版本号为：FPM083-00-118);
将指纹数据分发到模组中
*/
void downloadFTR(void)
{
    FP_ftrData_t downloadftrData;
    U8Bit ftrSentBuffer[128];          //每帧命令发送的buffer
    U16Bit i = 0;
    U16Bit remainLength;
    U16Bit currentLength;
    U16Bit frameCount;
    U32Bit errorCode;
    U32Bit ret;
    U32Bit timeout = DEFAULT_TIME_OUT;
    U8Bit err_num = 0;

/*
    typedef struct {
        U16Bit id;          //待下载的指纹id
        U16Bit length;      //待下载的指纹数据长度
        U8Bit  *data;       
    }FP_ftrData_t, *FP_ftrData_p;
*/
    downloadftrData.data = s_ftrDataBuffer;
    downloadftrData.length = s_ftrDataLength;
    downloadftrData.id = 0;
    
DownloadFtrSentInfoLabel:
    ret = FP_action_write_ftr_info(&downloadftrData, &errorCode);
    if(FP_OK != ret || errorCode != FP_OK)
    {
        Delay_ms(100);
        goto DownloadFtrSentInfoLabel;
    }

    remainLength = downloadftrData.length;
    currentLength = 128;
    frameCount = ((downloadftrData.length - 1) >> 7) + 1;

DownloadFtrSentDataLabel:
    for(; i < frameCount; i++)
    {
        memcpy(ftrSentBuffer, downloadftrData.data + 128*i, currentLength);

        //如果发最后一帧，由于模组需要存储指纹，命令超时时间需久一点, 1S比较合适
        if (i == frameCount - 1)
        {
            timeout = 1000;
        }
        ret = FP_action_write_ftr_data(i, ftrSentBuffer, currentLength, &errorCode, timeout);
        if(FP_OK != ret || errorCode != FP_OK)
        {
            err_num++;
            if(err_num < 3)
            {
                //数据发送有误，重新发送该帧
                Delay_ms(100);
                goto DownloadFtrSentDataLabel;

            }
            else
            {
                //如果超过3次,报错退出
                return;
            }
        }
        else
        {
            err_num = 0;
            remainLength -= currentLength;
            if(remainLength < 128)
            {
                currentLength = remainLength;
            }
        }
    }
}

/*********************************************************************************************
设置 LED 控制信息:适用于带LED灯的模组;
*/
void setLed(void)
{
    U32Bit errorCode;
    S32Bit ret;
	FP_LED_CTRL_INFO stLEDCtrlInfo;

setLedStartLabel:
/**灯的模式与颜色按需求设置，以下为参考范例1：闪烁绿灯5次,亮的时间为200ms，灭的时间为200ms*/
    stLEDCtrlInfo.ucLEDCtrlMode = EM_LED_CTRL_BLINK;
    stLEDCtrlInfo.ucLEDColor = EM_LED_GREEN;
    stLEDCtrlInfo.usLEDpara1 = 0x14;    //点亮时长
    stLEDCtrlInfo.usLEDpara2 = 0x14;    //熄灭时长
    stLEDCtrlInfo.usLEDpara3 = 0x5;     //闪烁次数
/***/
/**灯的模式与颜色按需求设置，以下为参考范例2：蓝灯呼吸,最大占空比100,最小占空比0,占空比每秒变化50%)*
    stLEDCtrlInfo.ucLEDCtrlMode = EM_LED_CTRL_PWM;
    stLEDCtrlInfo.ucLEDColor = EM_LED_BLUE;
    stLEDCtrlInfo.usLEDpara1 = 0x64;    //最大占空比
    stLEDCtrlInfo.usLEDpara2 = 0x00;    //熄灭时长
    stLEDCtrlInfo.usLEDpara3 = 0x32;     //闪烁次数
***/
    stLEDCtrlInfo.ucLEDCtrlMode = EM_LED_CTRL_PWM;
    stLEDCtrlInfo.ucLEDColor = EM_LED_RED_WITH_BLUE;
    stLEDCtrlInfo.usLEDpara1 = 0x64;    //最大占空比
    stLEDCtrlInfo.usLEDpara2 = 0x00;    //熄灭时长
    stLEDCtrlInfo.usLEDpara3 = 128;     //闪烁次数

    ret = FP_action_set_led(&stLEDCtrlInfo, &errorCode);
    if(FP_OK == ret && 0 == errorCode)
    { 
        //点灯成功
        printf("set LED success\r\n");
    }
    else
    {
        //延时 50 ms, 继续发送点灯命令，或者直接退出
        Delay_ms(50);
        goto setLedStartLabel;
    }
}


/*********************************************************************************************
设置系统策略:用于设置模组默认的功能；如关闭重复指纹检查功能
*/
void setSysPolicy(void)
{
    U32Bit errorCode;
    S32Bit ret;
    U32Bit SysPolicy_value;
    U32Bit SysPolicy_bit_selflearn = 0x00000002;    //重复指纹检查功能的位
    U32Bit timeout = 1000; //建议超时时间设为1s

/**设置系统策略之前，先获取系统策略*/
getSysPolicyLabel:

    ret = FP_action_get_SysPolicy(&SysPolicy_value, &errorCode);
    if(FP_OK == ret && 0 == errorCode)
    { 
        //获取策略成功
    }
    else
    {
        //延时 50 ms, 继续发送点灯命令，或者直接退出
        Delay_ms(50);
        goto getSysPolicyLabel;
    }
    
setSysPolicyLabel:
    SysPolicy_value &= ~SysPolicy_bit_selflearn;

    ret = FP_action_set_SysPolicy(&SysPolicy_value, &errorCode, timeout);  //处理返回码
    if(FP_OK == ret && 0 == errorCode)
    { 
        //获取策略成功后，模组会自行复位
        Delay_ms(200);
        return;
    }
    else
    {
        //延时 50 ms, 继续发送
        Delay_ms(50);
        goto setSysPolicyLabel;
    }
}
/*
查询手指在位状态
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 35 CA

指纹匹配
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 21 DE

查询匹配结果
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 22 DD





指纹注册
F1 1F E2 2E B6 6B A8 8A 00 08 85 00 00 00 00 01 11 01 ED
 * */

void demo_main(U8Bit command)
{
    //设置指纹收发的buffer
    FP_action_set_send_buffer(s_send_buffer);
    FP_action_set_recv_buffer(s_recv_buffer);

    switch(command)
    {
        case 0:
            getID();    // 获取模组ID
            break;
        case 1:
            enroll();   // 注册，录入指纹
            break;
        case 2:
            match();    // 匹配，验证指纹
            break;
        case 3:
            deleteFp(); // 删除指纹
            break;
        case 4:
            sleep();    // 休眠
            break;
        case 5:
            updateFeature();     // 更新指纹特征
            break;
        case 6:
            autoEnroll();   // 自动注册，录入指纹
            break;
        case 7:
            matchSyn();     // 同步匹配
            break;
        case 8:
            deleteSyn();    // 删除同步指纹
            break;
        case 9:
            uploadFTR();    // 上传指纹特征
            break;
        case 10:
            downloadFTR();  // 下载指纹特征
            break;
        case 11:
            setLed();       // 设置 LED 控制信息
            break;
        case 12:
            setSysPolicy(); // 设置系统策略
            break;

        default:
            break;
    }

}
