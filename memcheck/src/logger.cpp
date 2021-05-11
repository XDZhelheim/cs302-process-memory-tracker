#include "logger.h"
#include "memnode.h"
#include <map>
#include <execinfo.h>
#include <semaphore.h>

using std::map;
using std::pair;

static bool enabled = false;
static map<void *, memnode *> stack_trace;
static sem_t sem;

static void logger_enable(int en)
{
    enabled = en;
}

void logger_start(void)
{
    sem_init(&sem, 0, 1);
    logger_enable(true);
}

void logger_record(int type, void *ptr, size_t size, size_t block)
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
            int s = backtrace(node->get_back_trace(), MAX_STACK_TRACE);
            node->set_back_trace(backtrace_symbols(node->get_back_trace(), s), s);
            stack_trace[ptr] = node;

            logger_enable(true);
        }
        else
        {
            fprintf(stderr, "Release  memory at %p\n", ptr);

            logger_enable(false);

            stack_trace.erase(ptr);

            logger_enable(true);
        }
        sem_post(&sem);
    }
}

void logger_finish(void)
{
    enabled = false;
    int all = stack_trace.size();
    fprintf(stderr, "%d node(s) of memory not released.\n", all);
    if (all)
    {
        int index = 0;
        for (pair<void *, memnode *> p : stack_trace)
        {
            memnode *node = p.second;
            fprintf(stderr, "%02d. at %p: %ld block(s) with size %ld\n", ++index, node->get_ptr(), node->get_block(), node->get_size());
            for (int i = 3; i < node->get_trace_size(); i++)
            {
                fprintf(stderr, "        at %p: %s\n", node->get_back_trace()[i], node->get_symbols()[i]);
            }
        }
    }
    else
    {
        fprintf(stderr, "No memory leak!\n");
    }
}