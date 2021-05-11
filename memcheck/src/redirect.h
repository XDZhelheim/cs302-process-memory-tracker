#ifndef REDIRECT_H
#define REDIRECT_H

#include <stdlib.h>
#include "logger.h"

void __attribute__((constructor)) nu_init(void);
void __attribute__((destructor)) nu_fini(void);

#endif