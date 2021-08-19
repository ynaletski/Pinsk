
#define Test 0  // 1 - тестовый режим без MVD

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <time.h>
#include <sys\types.h>
#include <sys\timeb.h>

#include "device.h"

#include "PROC.h"
#include "MMI.h"
//#include "ekd.h"

#include "queue.h"
#include "icp.h"
#include "host.h"
#include "7017c.h"
#include "i7060.h"
#include "i7041.h"
#include "ofp.h"
#include "eee.h"
#include "rtu.h"
#include "sk.h"

#if defined(RTU_Delta)
#include "delta.h"
#endif

#if defined(RTU_Delta_C)
#include "deltaC.h"
#endif


#include "MVD.h"
#include "tsd.h"
#include "arch.h"
#include "avt_ctrl.h"
#include "reg.h"
#include "cor_adc.h"
#include "slave.h"
#include "valve.h"
#include "ref_t.h"
#include "md5.h"

