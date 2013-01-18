#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdlib.h>

void *setPointer( void **dest, void *val )
{
	void *ret;
	__asm__ ( "lock; xchg %0,(%1)"
			  : "=r" (ret) : "r" (dest), "0" (val) : "memory" );
	return ret;
}

typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;

static inline int
casXInternal(volatile void** addr,
     unsigned long expected_high, unsigned long expected_low,
     unsigned long new_high, unsigned long new_low)
{
	char success;
    asm volatile("lock; cmpxchg8b (%6);"
                 "setz %7; "
                 : "=a" (expected_low), "=d" (expected_high)
                 : "0" (expected_low), "1" (expected_high),
				   "c" (new_high), "b" (new_low),
				   "r" (addr), "m" (success)
                 : "cc", "memory");
    return success;
}


static inline int casX(volatile void** addr,
                        const void *oldVal,
                        const void *newVal)
{
    unsigned long old_high = (unsigned long long)oldVal >> 32, old_low = (unsigned long)oldVal;
    unsigned long new_high = (unsigned long long)newVal >> 32, new_low = (unsigned long)newVal;
	return casXInternal(addr, old_high, old_low, new_high, new_low);
}

#endif
