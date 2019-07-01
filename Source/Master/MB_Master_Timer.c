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

#include "MB_Master_Timer.h"
#include "mb.h"
#include "cmsis_os2.h"

/* 定时器的状态 */
enum TimerStatusEnum MB_MstTimerStatus;

osTimerId_t MB_MstTimerID; /* timer id */

/**
* @Function: void MB_MstTimerCallback(void *arg)
* @Brief: 定时器时间到，调用
* @Calls: None
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: 必须修改定时器的状态为TS_TimeUp
**/
void MB_MstTimerCallback(void *arg)
{
	arg = arg;
	
	/* 必须将MB_MstTimerStatus设置为TS_TimeUp */
	MB_MstTimerStatus = TS_TimeUp;
}/*lint !e438 Last value assigned to variable 'arg' not used */

/**
* @Function: void MB_MstTimerInit(void)
* @Brief: 定时器的初始化
* @Calls: None
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: 必须修改定时器的状态为TS_Init
**/
void MB_MstTimerInit(void)
{
	MB_MstTimerID = osTimerNew(MB_MstTimerCallback, osTimerOnce, NULL, NULL);
	if (MB_MstTimerID == NULL)
	{
		;
	}
	else
	{
		/* 必须将MB_MstTimerStatus设置为TS_Init */
		MB_MstTimerStatus = TS_Init;
	}
}

/**
* @Function: void MB_MstTimerStart(uint32_t ticks)
* @Brief: 启动定时器
* @Calls: None
* @Called By: None
* @Input: ticks 定时时间
* @Output: None
* @Return: None
* @Others: 必须修改定时器的状态为TS_Running
**/
void MB_MstTimerStart(uint32_t ticks)
{
	if (MB_MstTimerID != NULL)
	{
		if (osOK != osTimerStart(MB_MstTimerID, ticks))
		{
			;
		}
		else
		{
			/* 必须将MB_MstTimerStatus设置为TS_Running */
			MB_MstTimerStatus = TS_Running;
		}
	}
}

/**
* @Function: void MB_MstTimerStop(void)
* @Brief: 停止定时器
* @Calls: None
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: 必须修改定时器的状态为TS_Stop
**/
void MB_MstTimerStop(void)
{
	if (osOK != osTimerStop(MB_MstTimerID))
	{
		;
	}
	else
	{
		/* 必须将MB_MstTimerStatus设置为TS_Stop */
		MB_MstTimerStatus = TS_Stop;
	}
}

/************************************************************** end of file ***/
