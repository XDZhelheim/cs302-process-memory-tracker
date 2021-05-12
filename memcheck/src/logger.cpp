#include <map>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"
#include "memnode.h"
#include "fdnode.h"

using std::map;
using std::pair;

static bool enabled = false;
static map<void *, memnode *> mem_map;
static map<int, fdnode *> fd_map;
static sem_t sem;

void logger_enable(int en)
{
    enabled = en;
}

void logger_start(void)
{
    sem_init(&sem, 0, 1);
    logger_enable(true);
}

void logger_mem_record(int type, void *ptr, size_t size, size_t block)
{
    if (enabled)
    {
        sem_wait(&sem);
        if (type)
        {
            if (block == 1)
            {
                fprintf(stderr, "Allocate memory at %p size %ld\n", ptr, size);
            }
            else
            {
                fprintf(stderr, "Allocate memory at %p with %ld block(s) of size %ld\n", ptr, block, size);
            }

            logger_enable(false);

            memnode *node = new memnode(ptr, size, block);
            mem_map[ptr] = node;

            logger_enable(true);
        }
        else
        {
            fprintf(stderr, "Release  memory at %p\n", ptr);

            logger_enable(false);

            mem_map.erase(ptr);

            logger_enable(true);
        }
        sem_post(&sem);
    }
}

void logger_fd_record(int type, FILE *f, int fd, const char *filename)
{
    if (enabled)
    {
        if (type)
        {
            if (f)
            {
                fprintf(stderr, "FILE open  using  fopen at %p name %s\n", (void *)f, filename);
            }
            else
            {
                fprintf(stderr, "FILE open  using   open fd %02d name %s\n", fd, filename);
            }

            logger_enable(false);

            if (fd == -1)
            {
                fd = fileno(f);
            }

            fdnode *node = new fdnode(f, filename, fd);
            fd_map[fd] = node;

            logger_enable(true);
        }
        else
        {
            if (f)
            {
                fprintf(stderr, "FILE close using fclose at %p\n", (void *)f);
            }
            else
            {
                fprintf(stderr, "FILE close using  close fd %02d\n", fd);
            }

            logger_enable(false);

            if (fd == -1)
            {
                fd = fileno(f);
            }
            fd_map.erase(fd);

            logger_enable(true);
        }
    }
}

void logger_finish(void)
{
    enabled = false;

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

            fprintf(stderr, "%02d. Memory at %p: %ld block(s) with size %ld\n", ++index, node->get_ptr(), node->get_block(), node->get_size());
            for (int i = 3; i < tr->get_trace_size(); i++)
            {
                fprintf(stderr, "        at %p: %s\n", tr->get_back_trace()[i], tr->get_symbols()[i]);
            }
            free(node->get_ptr());
            fprintf(stderr, "    ---- Auto freed %p\n", node->get_ptr());
        }
    }
    else
    {
        fprintf(stderr, "No memory leak!\n");
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
                fprintf(stderr, "%02d. File at %p name %s\n", ++index, (void *)node->get_file(), node->get_name());
            }
            else
            {
                fprintf(stderr, "%02d. File fd %02d name %s\n", ++index, node->get_fd(), node->get_name());
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
    }
    else
    {
        fprintf(stderr, "No file handler leak!\n");
    }
}