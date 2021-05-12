#include <map>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"
#include "memnode.h"
#include "fdnode.h"

using std::map;
using std::pair;

static bool started = false;
static bool enabled = true;
static bool finished = false;
static map<void *, memnode *> mem_map;
static map<int, fdnode *> fd_map;
static sem_t sem;

void logger_enable(int en)
{
    enabled = en;
}

void logger_start(void)
{
    fprintf(stderr, "%s Program started\n", get_local_time());
    sem_init(&sem, 0, 1);
    started = true;
}

void logger_mem_record(int type, void *ptr, size_t size, size_t block)
{
    if (started && enabled && !finished)
    {
        sem_wait(&sem);
        logger_enable(false);

        if (type)
        {
            memnode *node = new memnode(ptr, size, block);
            mem_map[ptr] = node;

            if (block == 1)
            {
                fprintf(stderr, "%s Allocate memory at %p size %ld\n", node->get_trace()->get_trace_time(), ptr, size);
            }
            else
            {
                fprintf(stderr, "%s Allocate memory at %p with %ld block(s) of size %ld\n", node->get_trace()->get_trace_time(), ptr, block, size);
            }
        }
        else
        {
            fprintf(stderr, "%s Release  memory at %p\n", get_local_time(), ptr);

            mem_map.erase(ptr);
        }

        logger_enable(true);
        sem_post(&sem);
    }
}

void logger_fd_record(int type, FILE *f, int fd, const char *filename)
{
    if (started && enabled && !finished)
    {
        sem_wait(&sem);
        logger_enable(false);

        if (type)
        {
            fdnode *node = new fdnode(f, filename, fd);
            fd_map[fd] = node;

            if (f)
            {
                fprintf(stderr, "%s FILE open  using  fopen at %p name %s\n", node->get_trace()->get_trace_time(), (void *)f, filename);
            }
            else
            {
                fprintf(stderr, "%s FILE open  using   open fd %02d name %s\n", node->get_trace()->get_trace_time(), fd, filename);
            }

            if (fd == -1)
            {
                fd = fileno(f);
            }
        }
        else
        {
            if (f)
            {
                fprintf(stderr, "%s FILE close using fclose at %p\n", get_local_time(), (void *)f);
            }
            else
            {
                fprintf(stderr, "%s FILE close using  close fd %02d\n", get_local_time(), fd);
            }

            if (fd == -1)
            {
                fd = fileno(f);
            }
            fd_map.erase(fd);
        }

        logger_enable(true);
        sem_post(&sem);
    }
}

void logger_finish(void)
{
    finished = true;

    fprintf(stderr, "%s Program terminated\n\n", get_local_time());

    logger_mem_finish();
    logger_fd_finish();
}

void logger_mem_finish(void)
{
    int all = mem_map.size();
    fprintf(stderr, "%d node(s) of memory not released.\n", all);
    if (all)
    {
        int index = 0;
        for (pair<void *, memnode *> p : mem_map)
        {
            memnode *node = p.second;
            trace *tr = node->get_trace();

            fprintf(stderr, "%02d. Memory at %p allocated at %s : %ld block(s) with size %ld\n", ++index, node->get_ptr(), node->get_trace()->get_trace_time(), node->get_block(), node->get_size());
            for (int i = 3; i < tr->get_trace_size(); i++)
            {
                fprintf(stderr, "        at %p: %s\n", tr->get_back_trace()[i], tr->get_symbols()[i]);
            }
            free(node->get_ptr());
            fprintf(stderr, "    ---- Auto freed %p\n", node->get_ptr());
        }
        fprintf(stderr, "List ended\n\n");
    }
    else
    {
        fprintf(stderr, "No memory leak!\n\n");
    }
}

void logger_fd_finish(void)
{
    int all = fd_map.size();
    fprintf(stderr, "%d node(s) of file not released.\n", all);
    if (all)
    {
        int index = 0;
        for (pair<int, fdnode *> p : fd_map)
        {
            fdnode *node = p.second;
            trace *tr = node->get_trace();

            if (node->get_file())
            {
                fprintf(stderr, "%02d. File at %p opened at %s : %s\n", ++index, (void *)node->get_file(), node->get_trace()->get_trace_time(), node->get_name());
            }
            else
            {
                fprintf(stderr, "%02d. File fd %02d opened at %s : %s\n", ++index, node->get_fd(), node->get_trace()->get_trace_time(), node->get_name());
            }

            for (int i = 3; i < tr->get_trace_size(); i++)
            {
                fprintf(stderr, "        at %p: %s\n", tr->get_back_trace()[i], tr->get_symbols()[i]);
            }

            if (node->get_file())
            {
                fclose(node->get_file());
                fprintf(stderr, "    ---- Auto closed %p\n", (void *)node->get_file());
            }
            else
            {
                close(node->get_fd());
                fprintf(stderr, "    ---- Auto closed fd %d\n", node->get_fd());
            }
        }
        fprintf(stderr, "List ended\n\n");
    }
    else
    {
        fprintf(stderr, "No file handler leak!\n\n");
    }
}