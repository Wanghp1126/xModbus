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
#ifndef __MB_MASTER_PORT_H__
#define __MB_MASTER_PORT_H__

#include "myBool.h"

enum MB_Master_PortStateEnum{
	MBMPE_IDLE,	
	MBMPE_FRAME_SEND,
	MBMPE_FRAME_RECEIVED,
	MBMPE_FRAME_EXECUTE
};


void MB_MstPortInit(void);
	
void MB_MstPortStateSet(enum MB_Master_PortStateEnum portState);

enum mFlagStatus MB_MstPortStateGet(enum MB_Master_PortStateEnum portState);

#endif
/************************************************************** end of file ***/
