#include <string.h>
#include <stdio.h>
#include "fingerprint_action.h"
#include "fingerprint_type.h"
#include "fingerprint_protocol.h"

#include <windows.h>

static U8Bit s_recv_buffer[256];
static U8Bit s_send_buffer[256];
static U8Bit s_ftrDataBuffer[8196];        //���ϴ����ص�ָ������buffer
static U32Bit s_ftrDataLength = 0;          //���ϴ����ص�ָ������buffer����

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
//             //���ݲ�ͬ�Ŀ�������ʵ����ʱ
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

    //��һ��ֱ��enrollStart
    goto EnrollStartLabel;

    //
    //ÿ��enroll����ȷ����ָҪ̧�����Ժ��ٴΰ�ѹ
FingerLeaveLabel:               
    FP_action_check_finger_is_touch(&isTouch, &errorCode);
    if(0 != isTouch || COMP_CODE_OK != errorCode)
    {
        // ��isTouch����0��ʱ����ζ����ָ���ϣ�
        //��ʾ�û���ָ�뿪sensor�ٴΰ�ѹ
        //printf("lift your finger please !");
        //��ʱ 200 ms
		Delay_ms(200);
        goto FingerLeaveLabel;
    }

    //��ʼע��
EnrollStartLabel:
    ret = FP_action_enroll_start(index, &errorCode);
    if(COMP_CODE_OK != errorCode || FP_OK != ret)
    {
	    Delay_ms(100);
		//�����ϵ��ʼ��δ��ɻ�˯�߻��ѳ�ʼ����
		goto EnrollStartLabel;
	}
	else
	{
		//����ʱ100ms���ͻ�ȡע���������ͼ����Ҫ�����ô��ʱ�䴦��
		Delay_ms(100);
	}
    
    //��ȡע����
EnrollResultLabel:
    ret = FP_action_get_enroll_result(&enrollResult, &errorCode);
    if(FP_DEVICE_TIMEOUT_ERROR == ret)
	{
		//�������ݳ�ʱ�����ݽ����������賬ʱʱ���ʵ���ʱ��
		Delay_ms(100);
		goto EnrollResultLabel;
	}
    
    if(COMP_CODE_OK == errorCode)
    {
        //���errorCode ��COMP_CODE_OK,˵������enrollִ����ɣ�
        //��ʱ���Բ鿴enrollResult.progress�Ƿ����ӣ�������ӣ�˵������enroll�ɹ�

        //���progress >= 100 ��˵������ע�����̳ɹ���������ʼ����enrollResult.fingerID����ָ��
        if(enrollResult.progress >= 0x64)
        {
            goto SaveStartLabel;
        }
        else
        {
            //���progress < 100, ��ָ�뿪sensor���ٴΰ�ѹ������ע��
            index++;
            //��ʱ 100 ms
            Delay_ms(100);
            goto FingerLeaveLabel;
        }
    }
    else if(COMP_CODE_CMD_NOT_FINISHED == errorCode)
    {
        //errorCode == COMP_CODE_CMD_NOT_FINISHED��ζ��sensor��û�д�����ָ�����ݻ�û�а�ѹ��ָ��
        //�ʵ���ʱ���ٴλ�ȡ���
        //��ʱ 100 ms
        Delay_ms(100);
        goto EnrollResultLabel;
    }
    else if(COMP_CODE_SAME_ID == errorCode)
    {
        //errorCode == COMP_CODE_SAME_ID��ζ������ע��ָ���ظ����軻��ָע��
        //�ʵ���ʱ���ٴλ�ȡ���
        //��ʱ 100 ms
        Delay_ms(100);
        goto EnrollResultLabel;
    }
	else if(COMP_CODE_NO_FINGER_DETECT == errorCode)
	{
		//errorCode == COMP_CODE_NO_FINGER_DETECT��ζ�ų�ʱ��û��ѹ��ָ
        //����ע��
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
        //ͼ���������ã���ָ���ܰ�ѹ���ء����ᡢ����̫����ʪ�ȣ�����enrollStart���ɣ�Ҳ�ɸ���Elock�û��ֲ�ϸ������
        //��ʱ 200 ms
    	Delay_ms(200);
        goto EnrollStartLabel;
    }

    //����ָ�ƿ�ʼ
    //enrollResult.fingerID�����ģ��ظ����Ƽ�idȥ����,��Ŵ�00��ʼ
SaveStartLabel:
    ret = FP_action_save_start(enrollResult.fingerID, &errorCode);
    if(COMP_CODE_OK != errorCode || FP_OK != ret)
    {
	    Delay_ms(100);
		goto SaveStartLabel;
	}

	Delay_ms(200);
    //��ȡ����ָ�ƽ��
SaveResultLabel://������ָ�ƿ�ʼ������ͺ�ģ�������flash�����ڼ���200ms���͡���ȡ����ָ�ƽ����û�����ݻظ����ɳ�ʱ�ط�
    ret = FP_action_get_save_result(&errorCode, &SaveID);
	if(FP_DEVICE_TIMEOUT_ERROR == ret)
	{
		//�������ݳ�ʱ�����ݽ����������賬ʱʱ���ʵ���ʱ��
		Delay_ms(100);
		goto SaveResultLabel;
	}
	
    if(COMP_CODE_OK == errorCode)
    {
    	//�鿴����ɹ���SaveID
        //�������
    }
    else if(COMP_CODE_CMD_NOT_FINISHED == errorCode)
    {
        //��δ������ɣ���ʱ�ʵ�ʱ�䣬�ٴλ�ȡ���
        //��ʱ 100 ms
        Delay_ms(100);
        goto SaveResultLabel;
    }
    else if(COMP_CODE_STORAGE_IS_FULL == errorCode)
    {
        //flash�洢���������ܱ���ָ��
    }
    else
    {
        //�������󣬱���Ӳ������ȡ�
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
Ϊ�����٣�Ҳ���Բ��ȼ����ָ��λ��
��ͬ��ģ��ظ�ʱ�䲻ͬ(FPM08Xϵ��ģ���Լ��ʱ30ms����)
************************/
checkFingeronLabel:
    FP_action_check_finger_is_touch(&isTouch, &errorCode);
    if((0 != isTouch) && (COMP_CODE_OK == errorCode))
    {
        //��isTouch��1��ʱ����ζ����ָ���ϣ�
        //�����������Ρ�
    }
    else
    {
        //��ʱ 50 ms
        Delay_ms(50);
        goto checkFingeronLabel;
    }
/**********************/    
matchStartLabel:
    //��ʼmatch
    ret = FP_action_match_start(&errorCode);
	if(COMP_CODE_OK != errorCode || FP_OK != ret)
	{
		//��ʱ 50 ms
		//�����ϵ��ʼ��δ��ɻ�˯�߻��ѳ�ʼ����
        Delay_ms(50);
		goto matchStartLabel;
	}

	//ƥ�䴦��̶�����300ms;
    Delay_ms(300);
    //��ȡע����
matchResultLabel:
    ret = FP_action_get_match_result(&matchResult,&errorCode);
    if(FP_DEVICE_TIMEOUT_ERROR == ret)
	{
		//�������ݳ�ʱ�����ݽ����������賬ʱʱ���ʵ���ʱ��
		Delay_ms(100);
		goto matchResultLabel;
	}
    
    if(COMP_CODE_OK == errorCode)
    {
        //match�������

        /*  typedef struct {
         *      U16Bit isPass;
         *      U16Bit matchScore;
         *      U16Bit matchID;
         * } FP_matchResult_t, *FP_matchResult_p;
         */

        // isPass��1�Ļ��������Ѿ�ƥ�䵽��ָ�ƣ���ʱ��matchID��ƥ�䵽��ָ��ID
        // matchScore�Ǿ���ƥ��ķ���

        if(1 == matchResult.isPass)
        {
            //ƥ�䵽ָ��
            printf("match success, matchID = %d, matchScore = %d\n", matchResult.matchID, matchResult.matchScore);
        }
        else
        {
            //δƥ�䵽ָ��
        }

    }
    else if(COMP_CODE_CMD_NOT_FINISHED == errorCode)
    {
        //��δƥ����ɣ���ʱ�ʵ�ʱ�䣬�ٴλ�ȡ���,����ָ����ҪһЩʱ��
        //��ʱ 30 ms
	    Delay_ms(30);
        goto matchResultLabel;
    }
	else if(COMP_CODE_NO_FINGER_DETECT == errorCode)
	{
		//errorCode == COMP_CODE_NO_FINGER_DETECT��ζ�ų�ʱ��û��ѹ��ָ
        //����ƥ��
	}
    else if(COMP_CODE_NO_REQ_CMD == errorCode)
    {
        goto matchStartLabel;
	}
    else if(COMP_CODE_HARDWARE_ERROR == errorCode)
    {
        //��ʱ 200 ms
        Delay_ms(200);
        goto matchStartLabel;
    }
	else
    {
        return;
    }

}


/***ƥ��ɹ���ִ�и���ָ��������ģ����ѧϰ�����������Ż�����*
    ���������ƥ��ɹ��Ժ�ֱ�ӷ��ͣ�ģ��������ж��Ƿ�Ҫ��ѧϰ��
    �������Ҫ�᷵�ش�����
**/
void updateFeature(void)
{
    U32Bit errorCode;
    S32Bit ret;

    S32Bit updateID = 0;//�����ID������ƥ��ɹ����IDһ��

updatestartLabel:
    ret = FP_action_update_start(updateID, &errorCode);
    if(COMP_CODE_OK == errorCode)
    {	
        //��Ҫ���£���ʱ100����
        Delay_ms(100);
    }
    else
    {
       //����Ҫ���£�ֱ�ӽ���
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
    	//���³ɹ���
    } 
    else if(COMP_CODE_CMD_NOT_FINISHED == errorCode)
    {
        //�ʵ���ʱ100ms���ٴλ�ȡ
        Delay_ms(100);
        goto updateResultLabel;
    }
    else
    {
        //����ʧ��,���¸��£������˳�
        goto updatestartLabel;
    }
		
    return;
}


void deleteFp(void)
{
    U32Bit errorCode;
    S32Bit ret;
    S16Bit delete_id = -1;

    //��ʼɾ����delete_id > 0 ��ʱ��ɾ��ָ��id��ָ�ơ�
    //delete_id == -1��ʱ��ɾ������ָ�ƣ�
 deleteStartLabel:
    ret = FP_action_delete_start(delete_id, &errorCode);
	if(COMP_CODE_OK != errorCode || FP_OK != ret)
	{
		//��ʱ 100 ms
		//�����ϵ��ʼ��δ��ɻ�˯�߻��ѳ�ʼ����
		Delay_ms(100);
		goto deleteStartLabel;
	}

    //��ȡɾ�����
deleteResultLabel:
	Delay_ms(100);      //��ʱ 100 ms
    ret = FP_action_get_delete_result(&errorCode);
    if(ret == FP_DEVICE_TIMEOUT_ERROR)
    {
        goto deleteResultLabel;
    }
	
    if(COMP_CODE_OK == errorCode)
    {
        //ɾ���ɹ�
    }
    else if(COMP_CODE_CMD_NOT_FINISHED == errorCode)
    {
        //��δɾ����ɣ��ٴλ�ȡ���
        goto deleteResultLabel;
    }
    else
    {
        //�������󣬱���ָ��ID�����ڵȡ�flashӲ������ȡ�
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
        //���߳ɹ���ģ���������
    }
    else
    {
        //��ʱ 200 ms, ���������������ֱ���ظ����߳ɹ�Ϊֹ
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
    U32Bit timeout = 10000; //���鳬ʱʱ����Ϊ10s
    
	enrollPara.fingerID_high = 0xFF;  //���޸�ID���� ID Ϊ 0xFFFF ����ϵͳ�Զ�����ָ�� ID ��
	enrollPara.fingerID_low = 0xFF;  //���޸�ID���� ID Ϊ 0xFFFF ����ϵͳ�Զ�����ָ�� ID ��
	
	enrollPara.enroll_mode = 0x01; //����Ϊ 1���ʾ��ѹ����Ҫ�ȴ���ָ̧���ٴβſ��Խ����´�ע�ᣬ����Ϊ0����Ҫ��
	enrollPara.times = 3; 		 //���޸�ע���������Χ��1~6��

SendEnrollStartLabel:
	ret = FP_action_auto_enroll_send(enrollPara);
    if(FP_OK != ret)
    {
        goto SendEnrollStartLabel;
    }

GetEnrollResultLabel:
    ret = FP_action_auto_enroll_recv( &EnrollResult , &errorCode, timeout);  //��������
    if(0 != ret)
    {
        //�����ڵȴ���ָ��ѹ���������ڴ���ע�ᣬ�����²�ѯ�Ƿ���յ�
        goto GetEnrollResultLabel;
    }
    
    if(errorCode == COMP_CODE_CMD_DATA_LEN_ERROR || errorCode == COMP_CODE_CMD_DATA_ERROR)
    {
        //��������Ƿ񷢴����¿�ʼע��
        goto SendEnrollStartLabel;
    }

    if(errorCode == COMP_CODE_STORAGE_IS_FULL)
    {
        //ָ�������������˳�
        return;
    }

    if((100 == EnrollResult.progress) && (EnrollResult.state == 0xff))
    {
        if(COMP_CODE_OK == errorCode)
        {
            // ����100��state=0XFFʱ����Ϊע��ɹ�
            // ע��ɹ�
        }
        else
        {
            //�洢�������⣬���¿�ʼע��
            goto SendEnrollStartLabel;
        }
    }
    else
    {
        if(errorCode == COMP_CODE_OK)
        {
            //����ע��ɹ���������ѹ
        }
        else if(errorCode == COMP_CODE_SAME_ID)
        {
            //�ظ�ָ�ƣ�����ʾ�û�����ָ��ѹ
        }
        else if(errorCode == COMP_CODE_UNQUALIFIED_IMAGE_ERROR)
        {
            //��ͼ�������ã�������ѹֱ���ɹ�
        }
        else if(errorCode == COMP_CODE_NO_FINGER_DETECT)
        {
            //8S��ʱ��û��ѹ��ָ��������ѹֱ��ע��ɹ���
        }
        else
        {
            //�������⣬������ѹ
        }
            
        //������ȡ��һ�ν��
        goto GetEnrollResultLabel;
    }
    
    return;
}


void matchSyn(void)
{
    U32Bit errorCode;
	S32Bit ret;
    U32Bit timeout = 10000; //���鳬ʱʱ����Ϊ10s
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
        //�����ڵȴ���ָ��ѹ���������ڴ���ƥ�䣬�����²�ѯ�Ƿ���յ�
        goto getMatchSynResultLabel;
    }
    
    if((FP_OK == ret) && (COMP_CODE_OK == errorCode))
    {
        if(1 == matchResult.isPass)
        {
            //ƥ��ɹ�
            //ͬ��ƥ����Դ���ѧϰ���ܣ����Ժ�ʱ������ڷֲ�ƥ�䳤һ��
        }
        else
        {
            //ƥ��ʧ��
        }
    }
    else if(COMP_CODE_NO_FINGER_DETECT == errorCode)
    {
        //û����ָ��ѹ
    }
    else if(COMP_CODE_UNQUALIFIED_IMAGE_ERROR == errorCode)
    {
        //��ͼ��������
        goto matchSynSendLabel;
    }
    else
    {
        //��������
    }
}


void deleteSyn(void)
{
    U32Bit errorCode;
    S32Bit ret;
    U32Bit timeout = 1000;         //���鳬ʱʱ����Ϊ1s
    S16Bit delete_id = -1;

    //delete_id > 0 ��ʱ��ɾ��ָ��id��ָ�ơ�
    //delete_id == -1��ʱ��ɾ������ָ�ƣ�

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
        //ɾ���ɹ�
    }
    else
    {
        //�������󣬱���ָ��ID�����ڵȡ�flashӲ������ȡ�
    }

    return;
}


/**********************************************************************************************
�ϴ�ָ������:������FPM08Xϵ������汾����118��ģ��(��汾��Ϊ��FPM083-00-118);

һ��ģ���Ϊ����Ӧ�ó�����
    1���ϴ�ģ��������ע���id���������ָ�����0x13��+ ��ѯ���������0x14����
        ֱ�ӽ�ָ�������ϴ�����λ����

    2���ϴ�ģ���ڴ洢��ָ��һ��id���ϴ�֮ǰ����ȷ��id�Ƿ���ڣ��������������ϴ���
        
    3���ϴ�ģ���ڴ洢������id���ϴ�֮ǰ�����ȡָ��ģ��ֲ���Ȼ����ݷֲ�ָ��id�ţ�һ��һ���ϴ���

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
    U8Bit ftrReceiveBuffer[128];        //ÿ֡����ظ���buffer
    U8Bit err_num = 0;

/*
    typedef struct {
        U16Bit id;          //���ϴ���ָ��id
        U16Bit length;      //���ϴ���ָ�����ݳ���
        U8Bit  *data;       
    }FP_ftrData_t, *FP_ftrData_p;
*/

    uploadftrData.data = s_ftrDataBuffer;
    uploadftrData.id = 0x00;

/******�ϴ�ģ��������ע���id*************
    //ע�����100%
    //����ָ�ƿ�ʼ
    uploadftrData.id = 0xFFFF;      //��idΪ0XFFFFʱ����ֱ���ϴ�
    goto UploadFtrGetInfoLabel;
*****************************************/

UploadFtrCheckFtrLabel:
    ret = FP_action_get_ftr_distribution(&distribution, &errorCode);
    if(FP_OK != ret || errorCode != FP_OK)
    {
        //�������·���  
        Delay_ms(100);
        goto UploadFtrCheckFtrLabel;
    }

/*****************�ϴ�ģ���ڴ洢��ָ��һ��id**********/
//  Printf("distribution[] = 0x%x\r\n", distribution.distributon[check_id >> 3]);

    if(0 == CHECK_BIT(distribution.distributon[uploadftrData.id >> 3], (uploadftrData.id & 0x7)))       
    {
        //id�����ڣ����������ȷid��
        Delay_ms(100);
        goto UploadFtrCheckFtrLabel;
    }
/****************************************************/
    
/*********�ϴ�ģ���ڴ洢������id********************
    S16Bit upload_id[100] = 0;  //���ϴ���id,�����ϴ�����ָ��
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
                //���ݽ����������·��͸�֡
                goto UploadFtrGetDataLabel;
            }
            else
            {
                //�������3��,�����˳�
                return;
            }
        }
        else
        {
            err_num = 0;
            memcpy(uploadftrData.data + 128*i, ftrReceiveBuffer, 128);    //ģ��ÿ֡�ظ������ݳ�����ʵ�ǹ̶���128�ֽ�
        }
    }

    s_ftrDataLength = ftrLength;
    
    /*
        FTR�洢����λ�����ݿ�ǰ
        ���ftrData�ĳ���ftrLength���м�¼����Ϊ��ָ������ʱ�ĳ��ȣ�
        �ɰ�ftrData���һ�£���һ��CRC����żУ�飩
    */
}


/*********************************************************************************************
����ָ������:������FPM08Xϵ������汾����118��ģ��(��汾��Ϊ��FPM083-00-118);
��ָ�����ݷַ���ģ����
*/
void downloadFTR(void)
{
    FP_ftrData_t downloadftrData;
    U8Bit ftrSentBuffer[128];          //ÿ֡����͵�buffer
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
        U16Bit id;          //�����ص�ָ��id
        U16Bit length;      //�����ص�ָ�����ݳ���
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

        //��������һ֡������ģ����Ҫ�洢ָ�ƣ����ʱʱ�����һ��, 1S�ȽϺ���
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
                //���ݷ����������·��͸�֡
                Delay_ms(100);
                goto DownloadFtrSentDataLabel;

            }
            else
            {
                //�������3��,�����˳�
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
���� LED ������Ϣ:�����ڴ�LED�Ƶ�ģ��;
*/
void setLed(void)
{
    U32Bit errorCode;
    S32Bit ret;
	FP_LED_CTRL_INFO stLEDCtrlInfo;

setLedStartLabel:
/**�Ƶ�ģʽ����ɫ���������ã�����Ϊ�ο�����1����˸�̵�5��,����ʱ��Ϊ200ms�����ʱ��Ϊ200ms*/
    stLEDCtrlInfo.ucLEDCtrlMode = EM_LED_CTRL_BLINK;
    stLEDCtrlInfo.ucLEDColor = EM_LED_GREEN;
    stLEDCtrlInfo.usLEDpara1 = 0x14;    //����ʱ��
    stLEDCtrlInfo.usLEDpara2 = 0x14;    //Ϩ��ʱ��
    stLEDCtrlInfo.usLEDpara3 = 0x5;     //��˸����
/***/
/**�Ƶ�ģʽ����ɫ���������ã�����Ϊ�ο�����2�����ƺ���,���ռ�ձ�100,��Сռ�ձ�0,ռ�ձ�ÿ��仯50%)*
    stLEDCtrlInfo.ucLEDCtrlMode = EM_LED_CTRL_PWM;
    stLEDCtrlInfo.ucLEDColor = EM_LED_BLUE;
    stLEDCtrlInfo.usLEDpara1 = 0x64;    //���ռ�ձ�
    stLEDCtrlInfo.usLEDpara2 = 0x00;    //Ϩ��ʱ��
    stLEDCtrlInfo.usLEDpara3 = 0x32;     //��˸����
***/
    stLEDCtrlInfo.ucLEDCtrlMode = EM_LED_CTRL_PWM;
    stLEDCtrlInfo.ucLEDColor = EM_LED_RED_WITH_BLUE;
    stLEDCtrlInfo.usLEDpara1 = 0x64;    //���ռ�ձ�
    stLEDCtrlInfo.usLEDpara2 = 0x00;    //Ϩ��ʱ��
    stLEDCtrlInfo.usLEDpara3 = 128;     //��˸����

    ret = FP_action_set_led(&stLEDCtrlInfo, &errorCode);
    if(FP_OK == ret && 0 == errorCode)
    { 
        //��Ƴɹ�
        printf("set LED success\r\n");
    }
    else
    {
        //��ʱ 50 ms, �������͵���������ֱ���˳�
        Delay_ms(50);
        goto setLedStartLabel;
    }
}


/*********************************************************************************************
����ϵͳ����:��������ģ��Ĭ�ϵĹ��ܣ���ر��ظ�ָ�Ƽ�鹦��
*/
void setSysPolicy(void)
{
    U32Bit errorCode;
    S32Bit ret;
    U32Bit SysPolicy_value;
    U32Bit SysPolicy_bit_selflearn = 0x00000002;    //�ظ�ָ�Ƽ�鹦�ܵ�λ
    U32Bit timeout = 1000; //���鳬ʱʱ����Ϊ1s

/**����ϵͳ����֮ǰ���Ȼ�ȡϵͳ����*/
getSysPolicyLabel:

    ret = FP_action_get_SysPolicy(&SysPolicy_value, &errorCode);
    if(FP_OK == ret && 0 == errorCode)
    { 
        //��ȡ���Գɹ�
    }
    else
    {
        //��ʱ 50 ms, �������͵���������ֱ���˳�
        Delay_ms(50);
        goto getSysPolicyLabel;
    }
    
setSysPolicyLabel:
    SysPolicy_value &= ~SysPolicy_bit_selflearn;

    ret = FP_action_set_SysPolicy(&SysPolicy_value, &errorCode, timeout);  //��������
    if(FP_OK == ret && 0 == errorCode)
    { 
        //��ȡ���Գɹ���ģ������и�λ
        Delay_ms(200);
        return;
    }
    else
    {
        //��ʱ 50 ms, ��������
        Delay_ms(50);
        goto setSysPolicyLabel;
    }
}
/*
��ѯ��ָ��λ״̬
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 35 CA

ָ��ƥ��
F1 1F E2 2E B6 6B A8 8A 00 07 86 00 00 00 00 01 21 DE

��ѯƥ����
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





ָ��ע��
F1 1F E2 2E B6 6B A8 8A 00 08 85 00 00 00 00 01 11 01 ED
 * */

void demo_main(U8Bit command)
{
    //����ָ���շ���buffer
    FP_action_set_send_buffer(s_send_buffer);
    FP_action_set_recv_buffer(s_recv_buffer);

    switch(command)
    {
        case 0:
            getID();    // ��ȡģ��ID
            break;
        case 1:
            enroll();   // ע�ᣬ¼��ָ��
            break;
        case 2:
            match();    // ƥ�䣬��ָ֤��
            break;
        case 3:
            deleteFp(); // ɾ��ָ��
            break;
        case 4:
            sleep();    // ����
            break;
        case 5:
            updateFeature();     // ����ָ������
            break;
        case 6:
            autoEnroll();   // �Զ�ע�ᣬ¼��ָ��
            break;
        case 7:
            matchSyn();     // ͬ��ƥ��
            break;
        case 8:
            deleteSyn();    // ɾ��ͬ��ָ��
            break;
        case 9:
            uploadFTR();    // �ϴ�ָ������
            break;
        case 10:
            downloadFTR();  // ����ָ������
            break;
        case 11:
            setLed();       // ���� LED ������Ϣ
            break;
        case 12:
            setSysPolicy(); // ����ϵͳ����
            break;

        default:
            break;
    }

}
