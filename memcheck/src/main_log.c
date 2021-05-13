#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "main_log.h"

static bool started = false;
static bool enabled = true;
static bool finished = false;

static int pipe_fd[2];

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
    stdout_init();
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
    close(1);

    fprintf(stderr, "%s Program terminated\n\n", get_local_time());

    stdout_finish();
    memory_log_finish();
    file_handler_log_finish();
    file_descriptor_log_finish();
}

void stdout_init(void)
{
    pipe(pipe_fd);
    dup2(pipe_fd[1], 1);
}

void stdout_finish(void)
{
    close(pipe_fd[1]);
    fprintf(stderr, "------------------------------------------stdout------------------------------------------\n");

    const int buffer_size = 1024;
    char buffer[1024];

    int size = buffer_size - 1;
    while (size == buffer_size - 1)
    {
        size = read(pipe_fd[0], buffer, buffer_size - 1);
        buffer[size] = '\0';
        fprintf(stderr, "%s", buffer);
    }
    fprintf(stderr, "\n");

    fprintf(stderr, "---------------------------------------stdout ended---------------------------------------\n\n");
    close(pipe_fd[0]);
}