#include <stdbool.h>
#include <unistd.h>
#include "main_log.h"

static bool started = false;
static bool enabled = true;
static bool finished = false;

int log_enabled(void)
{
    return started && enabled && !finished;
}

void log_enable(int en)
{
    enabled = en;
}

void log_start(void)
{
    memory_log_init();
    file_handler_log_init();
    file_descriptor_log_init();
    
    fprintf(stderr, "%s Program Designed by MasterWanna\n", get_local_time());
    fprintf(stderr, "%s Program started pid = %d\n", get_local_time(), getpid());
    started = true;
}

void log_finish(void)
{
    finished = true;

    fprintf(stderr, "%s Program terminated\n\n", get_local_time());

    memory_log_finish();
    file_handler_log_finish();
    file_descriptor_log_finish();
}