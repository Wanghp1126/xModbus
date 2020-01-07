/*******************************************************************************
* @FileName:
* @Author: Michael
* @Version: V0.0.0
* @Date: 2019/07/01
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
*  Author: Michael
*  Modification:
*  
*  2. ...
*
*******************************************************************************/
#include "MB_Master_Port.h"
#include "MB_Master.h"
#include "RS485.h"
#include "USART.h"

/* USART2 缓存 */
extern struct USART2TypeDef USART2Buf;  

/* 端口的状态 */
static enum MB_Master_PortStateEnum MBM_PortState = MBMPE_IDLE;

/**
* @Function: void MB_MstPortInit(void)
* @Brief: Modbus主站端口的初始化
* @Calls: None
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: 必须对MB_MstPara中的各超时时间、MB_MstRespADU、MB_MstRespADULength进
*          行适当的配置。同时，对Modbus主站使用的端口进行初始化。
**/
void MB_MstPortInit(void)
{
	/* Modbus 主站的超时时间：写超时 */
	MB_MstPara.MBM_WriteTimeOut = 1000;
	/* Modbus 主站的超时时间：读超时 */
	MB_MstPara.MBM_ReadTimeOut = 500;
	/* Modbus 主站的超时时间：广播回转超时 */
	MB_MstPara.MBM_TurnAroundTime = 1000;	
	/* 接收数据的首地址 */
	MB_MstRespADU = &(USART2Buf.RxBuf[0]);
	/* 数据长度地址 */
	MB_MstRespADULength = &(USART2Buf.RxLength);
	
	/* 端口初始化 */
	RS485_Master_Init();
}

/**
* @Function: void MB_MstPortStateSet(enum MB_Master_PortEventEnum event)
* @Brief: Modbus主站端口状态的设置
* @Calls: None
* @Called By: None
* @Input: portState 端口状态
* @Output: None
* @Return: None
* @Others: 必须给MBM_PortState赋值
**/
void MB_MstPortStateSet(enum MB_Master_PortStateEnum portState)
{
	MBM_PortState = portState;
	
	
	switch(portState)
	{
		case MBMPE_IDLE:
			USART2Buf.PortRxState = PORT_STATE_RX_IDLE;
		break;
		
		case MBMPE_FRAME_RECEIVED:
			USART2Buf.PortRxState = PORT_STATE_RX_RECEIVED;
		break;
		
		case MBMPE_FRAME_SEND:
			USART2Buf.PortTxState = PORT_STATE_TX_TRANSMITTED;
		break;
		
		case MBMPE_FRAME_EXECUTE:
			USART2Buf.PortRxState = PORT_STATE_RX_DATA_HANDLING;
		break;
	}
}

/**
* @Function: enum mFlagStatus MB_MstPortStateGet(enum MB_Master_PortStateEnum portState)
* @Brief: Modbus主站端口状态的获取
* @Calls: None
* @Called By: None
* @Input: portState 端口状态
* @Output: None
* @Return: 相同返回mSET，不相同返回mRESET
* @Others: None
**/
enum mFlagStatus MB_MstPortStateGet(enum MB_Master_PortStateEnum portState)
{
	enum mFlagStatus flag;
	
	
	switch(USART2Buf.PortRxState)
	{
		case PORT_STATE_RX_IDLE :
			MBM_PortState = MBMPE_IDLE;
		break;
		
		case PORT_STATE_RX_RECEIVED:
			MBM_PortState = MBMPE_FRAME_RECEIVED;
		break;
		
		case PORT_STATE_RX_DATA_HANDLING:
			MBM_PortState = MBMPE_FRAME_EXECUTE;
		break;
		
		default:break;
	}
	
	
	if(MBM_PortState == portState)
	{
		flag = mSET;
	}
	else
	{
		flag = mRESET;
	}
	
	return flag;
}




/************************************************************** end of file ***/
