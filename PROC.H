
//#include"7188.h"
//#define I7188D

#include"7188xa.h"
//#include"x607.h"

#if defined(I7188D)
 #if !defined(TIM_TT$)
   unsigned long far *TimeTicks=(unsigned long far *)0x40006c;
   #define TIM_TT$
 #endif
#endif

