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
#ifndef __MB_MASTER_CALLBACK_H__
#define __MB_MASTER_CALLBACK_H__

#include <stdint.h>
#include "myBool.h"

enum mBool MB_MstIdleCallback(void);
void MB_MstTurnAroundCallBack(void);
enum MBExceptionCode MB_MstExcuteResponseCallBack(uint8_t * pusRepADU, uint16_t usLength);
void MB_MstExcuteError(uint8_t iD, uint8_t funcCode, uint8_t exCode);

#endif
/************************************************************** end of file ***/
