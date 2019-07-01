/*******************************************************************************
* @FileName:
* @Author:
* @Version:
* @Date:
* @Brief:
* @Others:
********************************************************************************
* @Function List:
*  1. -------
*  2. -------
*
********************************************************************************
* @History:
*  1. Date:  
*  Author:
*  Modification:
*  
*  2. ...
*
*******************************************************************************/

#include "MB_Master.h"
#include "MB_Master_Timer.h"
#include "MB_Master_Callback.h"
#include "mb_crc16.h"
#include "MB_Master_Port.h"

/* 指向接收到的数据 */
uint8_t *MB_MstRespADU;

/* 接收到的数据长度 */
uint16_t *MB_MstRespADULength;

/* 主Modbus的属性 */
struct MB_MasterParaTypeDef MB_MstPara;

/* 主Modbus的状态 */
enum MBErrorCode MB_MstErrStatus = MB_ERR_NONE;

/* FSM */
static enum MB_Master_FSMStateEnum MB_MstFSM = MBMS_Init;


/**
* @Function: void Modbus_Master_Init(void)
* @Brief: Modbus主站的初始化
* @Calls: MB_MstPortInit() 端口初始化
*         MB_MstTimerInit() 超时定时器初始化
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: None
**/
void Modbus_Master_Init(void)
{
	MB_MstFSM = MBMS_Init;
	
	MB_MstPortInit();
	MB_MstTimerInit();

	MB_MstFSM = MBMS_Idle;
}
/**
* @Function: void Modbus_Master_Poll(void)
* @Brief: Modbus主站状态机，可在任务中循环调用
* @Calls: None
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: None
**/
void Modbus_Master_Poll(void)
{
	switch (MB_MstFSM)
	{
		case MBMS_Idle:
		{
			/* 只能在空闲状态的回调中发送指令 */
			if(MB_MstIdleCallback() == mTRUE)	
			{
				/* 如果是广播 */ 
				if (MB_MstPara.MBM_ID == MB_ADDRESS_BROADCAST)
				{				
					/* 启动回转定时器 */
					MB_MstTimerStart(MB_MstPara.MBM_TurnAroundTime);
					/* 状态转化 */
					MB_MstFSM = MBMS_WaitAround; 
				}
				/* 如果不是广播 */
				else
				{
					
					if((MB_MstPara.MBM_Func == MB_FUNC_WRITE_MULTIPLE_REGISTERS)
						||(MB_MstPara.MBM_Func == MB_FUNC_WRITE_SINGLE_REGISTER))
					{
						/* 启动写等待超时 */
						MB_MstTimerStart(MB_MstPara.MBM_WriteTimeOut);
					}
					else
					{
						/* 启动读等待超时 */
						MB_MstTimerStart(MB_MstPara.MBM_ReadTimeOut);
					}
					
					/* 状态转化 */
					MB_MstFSM = MBMS_WaitResponse;
				}
				
				MB_MstErrStatus = MB_ERR_NONE;
				*MB_MstRespADULength = 0;
				MB_MstPortStateSet(MBMPE_IDLE);
			}
			
			break;
		}

		case MBMS_WaitAround:
		{
			/* 定时时间到 */
			if (MB_MstTimerStatus == TS_TimeUp)
			{
				MB_MstTurnAroundCallBack();
				MB_MstFSM = MBMS_Idle;
			}
			
			break;
		}

		case MBMS_WaitResponse:
		{
			if((MB_MstPortStateGet(MBMPE_FRAME_RECEIVED) == mSET) /* 收到一帧数据*/
			&&(MB_MstRespADU[0] == MB_MstPara.MBM_ID)) /* 期望的从站*/
			{
				/* 停止等待应答超时定时器 */
				MB_MstTimerStop();
				/* 状态转换 */
				MB_MstFSM = MBMS_ExecResponse;
			}
			/* 定时时间到 */
			else if (MB_MstTimerStatus == TS_TimeUp)				
			{
				/* 发生了超时 */
				MB_MstErrStatus = MB_ERR_TIMEOUT;
				/* 状态转换 */
				MB_MstFSM = MBMS_ExecError;
			}
			
			break;
		}

		case MBMS_ExecResponse:
		{
			/* CRC 校验 */
			if(usMBCRC16(MB_MstRespADU, *MB_MstRespADULength) != 0)
			{
				MB_MstErrStatus = MB_ERR_CRC;
				MB_MstFSM = MBMS_ExecError;
			}
			/* 从机应答了错误功能码 */
			else if ((MB_MstRespADU[1] & MB_FUNC_ERROR) == MB_FUNC_ERROR)
			{
				MB_MstFSM = MBMS_ExecError;
			}
			/* 正常应答 */
			else
			{
				if (MB_EX_NONE == 
					MB_MstExcuteResponseCallBack(MB_MstRespADU, 
												 *MB_MstRespADULength))				
				{
					MB_MstFSM = MBMS_Idle;
				}
				else
				{
					MB_MstFSM = MBMS_ExecError;
				}
			}
			
			break;
		}

		case MBMS_ExecError:
		{
			MB_MstExcuteError(MB_MstRespADU[0], /* ID */
			                  MB_MstRespADU[1] & 0x7F, /* 功能码 */
			                  MB_MstRespADU[2]);/* 错误码 */
			
			MB_MstFSM = MBMS_Idle;
			break;
		}

		default:
		{
			Modbus_Master_Init();
			break;
		}
	}/*lint !e788*/
}

/************************************************************** end of file ***/
