/*******************************************************************************
* @FileName: MB.h
* @Author: WHP
* @Version: 2.20
* @Date:
* @Brief:
* @Others: 要修改此文件时，请谨慎再谨慎
********************************************************************************
* @Function List: // 主要函数及其功能
*  1. Modbus 帧间隔时间计算
*     uint32_t Modbus_T35_50us(uint32_t baudrate)
*  2. 
*
********************************************************************************
* @History:	// 历史修改记录
*  1. Date:  
*  Author:
*  Modification:
*  
*  2. ...
*
*******************************************************************************/

#ifndef _MB_H_
#define _MB_H_

#include <stdint.h>

/****************** Modbus Address ********************/
#define MB_ADDRESS_BROADCAST    ( 0 )   /*! Modbus broadcast address. */
#define MB_ADDRESS_MIN          ( 1 )   /*! Smallest possible slave address. */
#define MB_ADDRESS_MAX          ( 247 ) /*! Biggest possible slave address. */
/****************** End of Modbus Address *************/

/****************** Modbus Function code's ************/
#define MB_FUNC_HANDLERS_MAX                  ( 7 )
#define MB_FUNC_WRITE_SINGLE_REGISTER	   	  ( 0x06 )  /* 写单个寄存器 */
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS   	  ( 0x10 )  /* 写多个寄存器 */
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  ( 0x17 )  /* 读写多个寄存器 */
#define MB_FUNC_READ_HOLDING_REGISTER	   	  ( 0x03 )  /* 读保持寄存器 */
#define MB_FUNC_READ_INPUT_REGISTER           ( 0x04 )  /* 读输入寄存器 */
#define MB_FUNC_REPORT_SLAVEID                ( 0x11 )  /* 报告从机，仅用于串行链路 */
#define MB_FUNC_READ_DEVICE_IDENTIFICATION    ( 0x2B )  /* 读设备识别码 */
#define MB_FUNC_ERROR                         ( 0x80 )  /* 功能错误 */
/****************** End of Modbus Function code's ******/


/****************** Modbus Messgae frame's ************/
/*!
 * Constants which defines the format of a modbus frame. The example is
 * shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
 * dependent on the underlying transport.
 *
 * <code>
 *  |<---------------------- MODBUS SERIAL LINE ADU (1) ------------------>|
 *              |<---------- MODBUS PDU (1') --------------->|
 *  +-----------+---------------+----------------------------+-------------+
 *  | Address   | Function Code | Data                       |  CRC        |
 *  +-----------+---------------+----------------------------+-------------+
 *  |           |               |                                   |
 * (2)        (3/2')           (3')                                (4)
 *
 * (1)  ... MB_SER_ADU_SIZE_MAX    = 256
 * (2)  ... MB_SER_ADU_ADDR_OFFSET = 0
 * (3)  ... MB_SER_ADU_PDU_OFFSET  = 1
 * (4)  ... MB_SER_ADU_CRC_SIZE    = 2
 *
 * (1') ... MB_PDU_SIZE_MAX     = 253
 * (2') ... MB_PDU_FUNC_OFFSET  = 0
 * (3') ... MB_PDU_DATA_OFFSET  = 1
 *
 *
 *
 * </code>
 */
 
#define MB_SER_ADU_SIZE_MAX        ( 256 ) /* 串行链路的ADU的最大字节数 */
#define MB_SER_ADU_SIZE_MIN        ( 7 )   /* 串行链路的ADU的最小字节数 */
#define MB_SER_ADU_ADDR_OFFSET     ( 0 )   /* 串行链路的ADU中地址的偏移量 */
#define MB_SER_ADU_PDU_OFFSET      ( 1 )   /* 串行链路的ADU中PDU的偏移量 */
#define MB_SER_ADU_CRC_SIZE        ( 2 )   /* 串行链路的ADU的CRC校验码长度为2字节 */	
#define MB_PDU_SIZE_MAX            ( 253 ) /* PDU的最大字节数 */
#define MB_PDU_SIZE_MIN            ( 2 )   /* 功能码的字节数 */
#define MB_PDU_FUNC_OFFSET         ( 0 )   /* 功能码在PDU中的偏移地址 */
#define MB_PDU_DATA_OFFSET         ( 1 )   /* 数据在PDU中的偏移地址 */
#define MB_PDU_DATA_MAX		       ( 252 ) /* PDU数据最大字节数 */
#define MB_PDU_REG_MAX             ( 126 ) /* PDU寄存器最多数量 */
#define MB_PDU_FUNC_READ_SIZE      ( 5 )   /* 读操作时，PDU的字节数 */
#define MB_PDU_FUNC_WRITE_SIG_SIZE ( 5 )   /* 写单个寄存器时，PDU的字节数 */
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN ( 6 )   /* 写多个寄存器时，PDU的最小字节数 */
#define MB_PDU_FUNC_READWRITE_SIZE_MIN ( 10 )   /* 读写多个寄存器时，PDU的最小字节数 */
#define MB_PDU_FUNC_WRITE_DVC_SIZE ( 4 )   /* 读取设备识别码时，PDU的字节数 */

/****************** End of Modbus Messgae frame's ************/
enum MBStatus{
	MB_STATUS_RX_IDLE,        /* 接收空闲 */
	MB_STATUS_FRAME_RECEIVED, /* 数据帧完成接收 */
	MB_STATUS_RX_RECV,        /* 数据帧正在接受 */
	
	MB_STATUS_TX_IDLE,        /* 发送空闲 */
	MB_STATUS_TX_XMIT         /* 正在发送 */
};

/* Modbus结构体 数据缓冲 
   RS485的Modbus通讯是单工通讯，所以接收数据和发送数据可以公用同一个缓冲。 
*/
struct MBBufferTypDef{
	uint16_t 	recvcount;				      // 数据的长度
	uint8_t 	membuff[MB_SER_ADU_SIZE_MAX]; // 数据
	enum MBStatus   status;                   // Modbus的状态
};

// Modbus 异常代码
enum MBExceptionCode{
	MB_EX_NONE = 0x00,
    MB_EX_ILLEGAL_FUNCTION = 0x01,     /* The function code received in the query is not an allowable action for the server (or slave). 
	                                      This may be because the function code is only applicable to newer devices, and was not 
	                                      implemented in the unit selected. It could also indicate that the server (or slave) is in the
	                                      wrong state to process a request of this type, for example because it is unconfigured and is 
	                                      being asked to return register values. */
    MB_EX_ILLEGAL_DATA_ADDRESS = 0x02, /* The data address received in the query is not an allowable address for the server (or slave). 
	                                      More specifically, the combination of reference number and transfer length is invalid. For a 
	                                      controller with 100 registers, a request with offset 96 and length 4 would succeed, a request 
	                                      with offset 96 and length 5 will generate exception 02.*/
    MB_EX_ILLEGAL_DATA_VALUE = 0x03,   /* A value contained in the query data field is not an allowable value for server (or slave). This
	                                      indicates a fault in the structure of the remainder of a complex request, such as that the implied
                                          length is incorrect. It specifically does NOT mean that a data item submitted for storage in a 
	                                      register has a value outside the expectation of the application program, since the MODBUS protocol
                                          is unaware of the significance of any particular value of any particular register */
    MB_EX_SLAVE_DEVICE_FAILURE = 0x04, /* An unrecoverable error occurred while the server (or slave) was attempting to perform the requested 
		                                  action */
    MB_EX_ACKNOWLEDGE = 0x05,          /* Specialized use in conjunction with programming commands. The server (or slave) has accepted the 
	                                      request and is processing it, but a long duration of time will be required to do so. This response 
	                                      is returned to prevent a timeout error from occurring in the client (or master). The client (or master)
                                          can next issue a Poll Program Complete message to determine if processing is completed. */ 
    MB_EX_SLAVE_BUSY = 0x06,           /* Specialized use in conjunction with programming commands.
	                                      The server (or slave) is engaged in processing a long–duration program command. The client (or
	                                      master) should retransmit the message later when the server (or slave) is free. */
    MB_EX_MEMORY_PARITY_ERROR = 0x08,  /* Specialized use in conjunction with function codes 20 and 21 and reference type 6, to indicate 
	                                      that the extended file area failed to pass a consistency check.
										  The server (or slave) attempted to read record file, but detected a parity error in the memory.
										  The client (or master) can retry the request, but service may be required on the server (or slave) 
										  device. */
    MB_EX_GATEWAY_PATH_FAILED = 0x0A,  /* Specialized use in conjunction with gateways, indicates that the gateway was unable to allocate
										  an internal communication path from the input port to the output port for processing the request.
										  Usually means that the gateway is misconfigured or overloaded. */
    MB_EX_GATEWAY_TGT_FAILED = 0x0B    /* Specialized use in conjunction with gateways, indicates that no response was obtained from the 
	 									  target device. Usually means that the device is not present on the network. */
};


// Modbus 错误代码
enum MBErrorCode{
	MB_ERR_NONE = 0, // 没有错误
	MB_ERR_ID   = 1, // Modbus的ID失配
	MB_ERR_CRC  = 2, // CRC 校验错误
	MB_ERR_NOREG,    // 无效的寄存器地址
	MB_ERR_INVAL,    // 无效寄存器数据
    MB_ERR_PORTERR,  // porting layer error. 
    MB_ERR_NORES,    // 资源不足
    MB_ERR_IO,       // I/O error. 
    MB_ERR_ILLSTATE, // 协议栈异常 
    MB_ERR_TIMEOUT,  // 发生了超时 
	MB_ERR_BUSY      // 忙
};

enum MBRegisterMode
{
    MB_REG_READ,                /*!< Read register values and pass to protocol stack. */
    MB_REG_WRITE                /*!< Update register values. */
};

// Modbus 帧间隔时间计算，返回值是50us的倍数
uint32_t Modbus_T35_50us(uint32_t Baudrate);	

#endif

/************************************************************** end of file ***/
