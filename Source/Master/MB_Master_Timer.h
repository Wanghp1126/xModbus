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
*  Author:
*  Modification:
*  
*  2. ...
*
*******************************************************************************/

#ifndef __MB_MASTER_TIMER_H__
#define __MB_MASTER_TIMER_H__

#include <stdint.h>

/* 定时间的状态 */
enum TimerStatusEnum{
	TS_Init, /* 初始化 */
	TS_Running,/* 正在运行 */
	TS_Stop,/* 停止 */
	TS_TimeUp/* 时间到了 */
};

extern enum TimerStatusEnum MB_MstTimerStatus;


void MB_MstTimerInit(void);

void MB_MstTimerStart(uint32_t ticks);

void MB_MstTimerStop(void);

#endif


/************************************************************** end of file ***/
