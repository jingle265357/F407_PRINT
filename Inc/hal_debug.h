#ifndef  __HAL_DEBUG_H

#define  __HAL_DEBUG_H

extern int sendchar(int ch);
void UartInit(void);

//#define M_DEBUG

//#define debug(format, ...) fprintf (stderr, format, __VA_ARGS__)

//不知道为什么下面的宏定义编译不通过
//#ifdef 	__DEBUG_
//	#define DEBUGOUT(format, ...)  int printf(const char *format, __VA_ARGS__)
////	#define DEBUGOUT(A,B)  printf(A,B)
//#else 
//	#define DEBUGOUT(format, ...)
//#endif


//extern int getKey(void);

#endif

