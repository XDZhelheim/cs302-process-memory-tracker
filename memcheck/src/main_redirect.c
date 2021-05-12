#include "main_redirect.h"

void __attribute__((constructor)) nu_init(void)
{
    log_start();
}

void __attribute__((destructor)) nu_fini(void)
{
    log_finish();
}