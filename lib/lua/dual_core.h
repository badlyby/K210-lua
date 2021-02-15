#ifndef _DUAL_CORE_H
#define _DUAL_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <atomic.h>
#include <stdint.h>
#include <encoding.h>

void core_lock(void);
void core_unlock(void);

#ifdef __cplusplus
}
#endif

#endif /* _DUAL_CORE_H */