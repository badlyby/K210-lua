#include "dual_core.h"

static corelock_t s_core_sync_locks[2] = { CORELOCK_INIT, CORELOCK_INIT };

void core_lock(void)
{
    uint64_t core = current_coreid();
    corelock_lock(&s_core_sync_locks[core]);
}

void core_unlock(void)
{
    uint64_t core = current_coreid();
    corelock_unlock(&s_core_sync_locks[core]);
}
