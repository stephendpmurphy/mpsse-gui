#ifndef _MPSSE_H_
#define _MPSSE_H_

#include "ftd2xx.h"
#include "libMPSSE_spi.h"

int mpsse_init(void);
uint32 mpsse_getChannelCount(void);
FT_DEVICE_LIST_INFO_NODE mpsse_getChannelInfo(int index);

#endif // _MPSSE_H_