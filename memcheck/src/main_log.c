#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "main_log.h"

static bool started = false;
static bool enabled = true;
static bool finished = false;

FILE *log_file;

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
    log_file = tmpfile();
    memory_log_init();
    file_handler_log_init();

    fprintf(log_file, "========================================Running Log=======================================\n\n");
    fprintf(log_file, "%s Program Designed by MasterWanna\n", get_local_time());
    fprintf(stdout, "\033[31;1mInfo\033[0m: %s Program Designed by MasterWanna\n", get_local_time());
    fprintf(log_file, "%s Program started pid = %d\n", get_local_time(), getpid());
    fprintf(stdout, "\033[31;1mInfo\033[0m: %s Program started pid = %d\n", get_local_time(), getpid());
    started = true;
}

void log_finish(void)
{
    finished = true;

    fprintf(log_file, "%s Program terminated\n\n", get_local_time());
    fprintf(stdout, "\033[31;1mInfo\033[0m: %s Program terminated\n\n", get_local_time());
    fprintf(log_file, "=====================================Running Log Ended====================================\n\n");

    memory_log_finish();
    file_handler_log_finish();

    fflush(log_file);
    fseek(log_file, 0, SEEK_SET);

    const size_t buffer_size = 1023;
    char buffer[buffer_size + 1];

    size_t size = buffer_size;
    while (size == buffer_size)
    {
        size = fread(buffer, 1, buffer_size, log_file);

        fwrite(buffer, 1, size, stdout);
    }
    fflush(stdout);

    fclose(log_file);
}
