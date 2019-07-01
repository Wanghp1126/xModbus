/*******************************************************************************
* @FileName:
* @Author: Michael
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
*  Author: Michael
*  Modification:
*  
*  2. ...
*
*******************************************************************************/

#ifndef __MB_MASTER_H__
#define __MB_MASTER_H__

#include "string.h"
#include "MB.h"

/* 主动Modbus属性 */
struct MB_MasterParaTypeDef{
	uint32_t MBM_TurnAroundTime; /* 广播读操作回转延时时间，单位：ms */ 
	uint32_t MBM_ReadTimeOut;    /* 读操作超时时间，单位：ms */ 
	uint32_t MBM_WriteTimeOut;   /* 写操作超时时间，单位：ms */ 
	
	uint8_t MBM_ID;  /* 当前从站ID */ 
	uint8_t MBM_Func;/* 当前执行的功能码 */
};

/* Modbus Master FSM 状态列表*/
enum MB_Master_FSMStateEnum{
	MBMS_Init ,        /* 初始化 */
	MBMS_Idle,         /* 空闲状态，空闲状态可以发送请求 */
	MBMS_WaitAround,   /* 广播时，等待回转状态 */
	MBMS_WaitResponse, /* 等待应答 */
	MBMS_ExecResponse, /* 处理应答 */
	MBMS_ExecError     /* 处理异常/故障 */
};

/* 主Modbus的属性 */
extern struct MB_MasterParaTypeDef MB_MstPara;

/* 主Modbus的状态 */
extern enum MBErrorCode MB_MstErrStatus;

/* 指向接收到的数据 */
extern uint8_t *MB_MstRespADU;

/* 接收到的数据长度 */
extern uint16_t *MB_MstRespADULength;

void Modbus_Master_Init(void);
/* 主动式 Modbus 进程 */
void Modbus_Master_Poll(void);	

#endif
/************************************************************** end of file ***/
