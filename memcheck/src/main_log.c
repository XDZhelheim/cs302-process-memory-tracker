#include <stdbool.h>
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
    fprintf(stderr, "%s Program started\n", get_local_time());
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