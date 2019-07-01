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
#include "MB_Master_Callback.h"
#include "MB_Master.h"
#include "Thread.h"
#include "Device02.h"
#include "Device01.h"

#define MBRequestADU_LENGTH_MIN  ( 4 )
#define MBRequestADU_LENGTH_MAX  ( 8 )

#define SlD_MaxIndex       ( 1 )
#define SlD_Device01       ( 0 )
#define SlD_Device02       ( 1 )

uint16_t SalveDeviceIndex = 0;

/* 主Modbus请求ADU */
static uint8_t MB_MstReqADU[MBRequestADU_LENGTH_MAX];

void GotoNextSlaveDevice(void)
{
	/* 调整到访问的下一个从站 */
	SalveDeviceIndex ++;
	if(SalveDeviceIndex > SlD_MaxIndex)
	{
		SalveDeviceIndex = 0;
	}
}
/**
* @Function: MB_MstIdleCallback
* @Brief: 主动Modbus，空闲时的回调函数
* @Calls: None
* @Called By: void Modbus_Master_Poll(void)
* @Input: None
* @Output: None
* @Return: 正常返回mTRUE，否则返回mFALSE
* @Others: 只能在这个函数里面发送请求
**/
enum mBool MB_MstIdleCallback(void)
{
	enum mBool cmdSend = mFALSE;
	uint16_t pusLength = 0;

	/* 创建请求ADU */
	switch(SalveDeviceIndex)
	{
		case SlD_Device01:
		{
			pusLength = Device01_ReadSpeedCmd(MB_MstReqADU);
			break;
		}
		
		case SlD_Device02:
		{
			pusLength = Device02_ReadAngleCmd(MB_MstReqADU);
			break;
		}
		
		default:
			pusLength = 0;
			break;
	}
	
	
	if( pusLength > 0)
	{
		/* 必须对本次执行的从站ID和功能码赋值 */
		MB_MstPara.MBM_ID = MB_MstReqADU[0];
		MB_MstPara.MBM_Func = MB_MstReqADU[1];
		
		/* 发送请求 */
		RS485_Master_Send(MB_MstReqADU, pusLength); /*lint !e534 Ignoring return value*/
		
		cmdSend = mTRUE;
	}
	else
	{
		cmdSend = mFALSE;
	}
	
	return cmdSend;
}
/**
* @Function: MB_MstTurnAroundCallBack()
* @Brief: 发送广播指令，并且等待时间到后，执行改回调函数
* @Calls: None
* @Called By: void Modbus_Master_Poll(void)
* @Input: None
* @Output: None
* @Return: None
* @Others: None
**/
void MB_MstTurnAroundCallBack(void)
{
	MB_MstErrStatus = MB_ERR_NONE;
	GotoNextSlaveDevice();
}

/**
* @Function: MB_MstExcuteResponseCallBack
* @Brief: 主动Modbus，收到正确应答后的回调函数，用于处理应答
* @Calls: None
* @Called By: void Modbus_Master_Poll(void)
* @Input: pusRepADU 应答ADU
*         usLength AUD的字节数
* @Output: None
* @Return: MBExceptionCode
* @Others: 
**/
enum MBExceptionCode MB_MstExcuteResponseCallBack(uint8_t * pusRepADU, uint16_t usLength)
{
	uint8_t *pucPDU;
	uint8_t repID;
	enum MBExceptionCode exStatus;
	
	usLength = (uint16_t)(usLength - (MB_SER_ADU_PDU_OFFSET + MB_SER_ADU_CRC_SIZE)); // PDU的字节数
	repID = pusRepADU[0];
	pucPDU = &pusRepADU[MB_SER_ADU_PDU_OFFSET]; // 指向PDU
	
	if (repID == Device01.Addr)
	{
		exStatus = Device01_Process(pucPDU, usLength); // 风速仪的处理
	}
	else if(repID == Device02.ID)
	{
		exStatus = Device02_Process(pucPDU, usLength); // 倾角仪的处理
	}
	
	GotoNextSlaveDevice();
	return exStatus;
}

/**
* @Function: MB_MstExcuteError
* @Brief: 主动Modbus，异常处理函数
* @Calls: None
* @Called By: void Modbus_Master_Poll(void)
* @Input: iD 从站ID
*         funcCode Modbus功能码
*         exCode Modbus异常码
* @Output: None
* @Return: None
* @Others: 需要先处理 MB_MstErrStatus
**/
void MB_MstExcuteError(uint8_t iD, uint8_t funcCode, uint8_t exCode)
{
	if(MB_MstErrStatus != MB_ERR_NONE)
	{
		switch(MB_MstErrStatus)
		{
			case MB_ERR_TIMEOUT: /* 发生了等待应答超时 */
				break;
			case MB_ERR_CRC: /* 接收帧CRC错误 */
				break;
			default:
				break;
		}
	}
	else
	{
		switch((enum MBExceptionCode)exCode)
		{
			case MB_EX_ILLEGAL_FUNCTION:
				break;
			case MB_EX_ILLEGAL_DATA_ADDRESS:
				break;
			case MB_EX_ILLEGAL_DATA_VALUE:
				break;
			case MB_EX_SLAVE_DEVICE_FAILURE:
				break;
			default:
				break;
		}
	}
	
	GotoNextSlaveDevice();
	MB_MstErrStatus = MB_ERR_NONE;
}


/************************************************************** end of file ***/

