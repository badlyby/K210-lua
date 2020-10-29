/*
The MIT License (MIT)

Copyright (c) 2017 badlyby

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//SPE通讯协议数据链路层
#include "SPE_DLL.h"

#ifndef SPE_RAMLessMode
volatile static uint32_t SPE_Scount;
static uint8_t SPE_Sbuf[((((SPE_MAX_SendPacketLength+2)*8)-6)/7)+2];
#endif

static uint8_t SPE_Rbuf[SPE_MAX_RecvPacketLength+2];
volatile static uint8_t SPE_TmpBits = 0;
volatile static uint8_t SPE_LBits = 0;
volatile static uint8_t SPE_Begin = 0;
static const uint8_t SPE_BitMasks[]={0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};

/**********************************************************************
//		函数名:		SPE_Start_Send
//		功能:			发送开始
//		参数个数:	无
//		返回值:		无
**********************************************************************/
void SPE_Start_Send(void)//数据包开始
{
	SPE_Begin = 1;
#ifndef SPE_RAMLessMode
	SPE_Scount = 0;
#endif
}

/**********************************************************************
//		函数名:		SPE_Byte_Send
//		功能:			发送数据字节
//		参数个数:	1, bits: 要发送的字节
//		返回值:		无
**********************************************************************/
void SPE_Byte_Send(uint8_t bits)//发送一个字节
{
	if(SPE_Begin)
	{
		SPE_Begin = 0;
	#ifdef SPE_RAMLessMode
		SendByte((bits >> 2) | 0x80);
	#else
		if(SPE_Scount >= sizeof(SPE_Sbuf)) return;
		SPE_Sbuf[SPE_Scount++] = (bits >> 2) | 0x80;
	#endif
		SPE_TmpBits = bits & 0x03;
		SPE_LBits = 2;
	}
	else
	{
	#ifdef SPE_RAMLessMode
		SendByte((SPE_TmpBits << (7 - SPE_LBits)) | (bits >> (SPE_LBits + 1)));
	#else
		if(SPE_Scount >= sizeof(SPE_Sbuf)) return;
		SPE_Sbuf[SPE_Scount++] = (SPE_TmpBits << (7 - SPE_LBits)) | (bits >> (SPE_LBits + 1));
	#endif
		SPE_LBits ++;
		SPE_TmpBits = bits & SPE_BitMasks[SPE_LBits];
		if(SPE_LBits == 7)
		{
		#ifdef SPE_RAMLessMode
			SendByte(SPE_TmpBits);
		#else
			if(SPE_Scount >= sizeof(SPE_Sbuf)) return;
			SPE_Sbuf[SPE_Scount++] = SPE_TmpBits;
		#endif
			SPE_LBits = 0;
			SPE_TmpBits = 0;
		}
	}
}

/**********************************************************************
//		函数名:		SPE_End_Send
//		功能:			发送结束
//		参数个数:	无
//		返回值:		无
**********************************************************************/
void SPE_End_Send(void)//数据包结束
{
	if(SPE_Begin)
	{
		SPE_Begin = 0;
	#ifdef SPE_RAMLessMode
		SendByte(0xC0);
	#else
		SPE_Sbuf[0] = 0xC0;
		SendBytes(SPE_Sbuf, 1);
	#endif
	}
	else
	{
	#ifdef SPE_RAMLessMode
		SendByte((SPE_TmpBits << (6 - SPE_LBits)) | 0xC0);
	#else
		if(SPE_Scount >= sizeof(SPE_Sbuf)) return;
		SPE_Sbuf[SPE_Scount++] = (SPE_TmpBits << (6 - SPE_LBits)) | 0xC0;
		SendBytes(SPE_Sbuf, SPE_Scount);
	#endif
	}
}

/**********************************************************************
//		函数名:		SPE_Receive_Byte
//		功能:			接受字节
//		参数个数:	1, byte: 接收字节
//		返回值:		无
**********************************************************************/
uint8_t SPE_Receive_Byte(uint8_t byte)//串口接收一个字节调用一次本函数
{
	static volatile uint32_t i,rcount = 0;
	static volatile uint8_t rbits = 0;
	static volatile uint8_t frame_start = 0;
	uint8_t hit = 0;
	if((byte & 0xC0) == 0x80)
	{
		frame_start = 1;
		rcount = 0;
		SPE_Rbuf[rcount] = (byte & 0x3F) << 2;
		rbits = 6;
	}
	else if((byte & 0xC0) == 0xC0)
	{
		if(frame_start == 0) return hit;
		if(rbits != 8)
		{
			if(rcount >= SPE_MAX_RecvPacketLength) return hit;
			SPE_Rbuf[rcount] |= ((byte & 0x3F) >> (rbits - 2));
			rcount ++;
		}
		SPE_ProcessData(SPE_Rbuf, rcount);
		frame_start = 0;
		rcount = 0;
		rbits = 8;
		hit = 1;
	}
	else
	{
		if(frame_start == 0) return hit;
		if(rbits == 1)
		{
			SPE_Rbuf[rcount] |= byte & 0x7F;
			rcount ++;
			rbits = 8;
		}
		else
		{
			if(rbits != 8)
			{
				SPE_Rbuf[rcount] |= ((byte & 0x7F) >> (rbits - 1));
				rcount ++;
			}
			if(rcount >= SPE_MAX_RecvPacketLength) return hit;
			SPE_Rbuf[rcount] = (byte << (9 - rbits));
			rbits --;
		}
	}
	return hit;
}
