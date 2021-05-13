#ifndef MAIN_REDIRECT_H
#define MAIN_REDIRECT_H

#include "main_log.h"

void __attribute__((constructor)) nu_init(void);
void __attribute__((destructor)) nu_fini(void);

#endif