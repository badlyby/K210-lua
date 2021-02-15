#ifndef __WZL_FIFO_DEFINE_H__
	#define __WZL_FIFO_DEFINE_H__
	#include <stdint.h>
	#ifdef __cplusplus
	extern "C"
	{
	#endif
	typedef struct {
	volatile uint16_t inIndex;
	volatile uint16_t outIndex;
	volatile uint8_t inited;
	uint16_t size;
	uint8_t *buf;
	} WZL_FIFO;

	#define FIFO_Init(x,y) do{(x) -> inIndex = 0;(x) -> outIndex = 0;(x) -> size = sizeof(y);(x) -> buf = y;(x) -> inited = 1;} while(0)
	#define FIFO_getSize(x) ((((x) -> inIndex >= (x) -> outIndex) ? ((x) -> inIndex - (x) -> outIndex) : ((x) -> size + (x) -> inIndex - (x) -> outIndex)))
	#define FIFO_getMAXSize(x) ((x) -> size)
	#define FIFO_push(x,y) do{if(FIFO_getSize(x) < ((x) -> size -1)){(x) -> buf[(x) -> inIndex] = y;(x) -> inIndex = ((x) -> inIndex + 1) % (x) -> size;}} while(0)
	#define FIFO_get(x,y) do{if(FIFO_getSize(x) > 0){(*y) = (x) -> buf[(x) -> outIndex];(x) -> outIndex = ((x) -> outIndex + 1) % (x) -> size;}} while(0)

//	static inline void FIFO_push(WZL_FIFO *fifo, uint8_t ch)
//	{
//		if(fifo->inited == 0)
//			return;
//		if(FIFO_getSize(fifo) < (fifo->size-1))
//		{
//			fifo->buf[fifo->inIndex++] = ch;
//			if(fifo->inIndex >= fifo->size)
//				fifo->inIndex = 0;
//		}
//	}
//	
//	static inline void FIFO_get(WZL_FIFO *fifo, uint8_t *ch)
//	{
//		if(fifo->inited == 0)
//			return;
//		if(FIFO_getSize(fifo))
//		{
//			*ch = fifo->buf[fifo->outIndex++];
//			if(fifo->outIndex >= fifo->size)
//				fifo->outIndex = 0;
//		}
//	}
		
	#ifdef __cplusplus
	}
	#endif

#endif //__WZL_FIFO_DEFINE_H__
