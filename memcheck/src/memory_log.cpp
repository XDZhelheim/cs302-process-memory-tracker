#include <map>
#include "main_log.h"
#include "memory_node.h"

using std::map;
using std::pair;

static map<void *, memory_node *> memory_map;
static int memory_allocate;
static int memory_release;
static size_t current_memory_size;
static size_t max_memory_size;
static size_t memory_size_allocate;
static size_t memory_size_release;

void memory_log_init()
{
    memory_allocate = 0;
    memory_release = 0;
    current_memory_size = 0;
    max_memory_size = 0;
    memory_size_allocate = 0;
    memory_size_release = 0;
}

void memory_log_record(int type, void *ptr, size_t size, size_t block)
{
    if (log_enabled())
    {
        log_enable(false);

        if (type)
        {
            size_t s = size * block;
            memory_node *node = new memory_node(ptr, size, block);

            if (!node->valid_memory_allocation())
            {
                delete node;
                log_enable(true);
                return;
            }

            memory_map[ptr] = node;

            if (block == 1)
            {
                fprintf(log_file, "%s Allocate memory at %p size %ld\n", node->get_trace()->get_trace_time(), ptr, size);
                fprintf(stdout, "\033[31;1mInfo\033[0m: %s Allocate memory at %p size %ld\n", node->get_trace()->get_trace_time(), ptr, size);
            }
            else
            {
                fprintf(log_file, "%s Allocate memory at %p with %ld block(s) of size %ld\n", node->get_trace()->get_trace_time(), ptr, block, size);
                fprintf(stdout, "\033[31;1mInfo\033[0m: %s Allocate memory at %p with %ld block(s) of size %ld\n", node->get_trace()->get_trace_time(), ptr, block, size);
            }
            memory_allocate++;

            memory_size_allocate += s;
            current_memory_size += s;
            if (current_memory_size > max_memory_size)
            {
                max_memory_size = current_memory_size;
            }
        }
        else
        {
            size_t s = 0;
            memory_node *node = memory_map[ptr];

            memory_map.erase(ptr);

            if (node != NULL)
            {
                s = node->get_block() * node->get_size();
            }
            else
            {
                log_enable(true);
                return;
            }

            fprintf(log_file, "%s Release  memory at %p\n", get_local_time(), ptr);
            fprintf(stdout, "\033[31;1mInfo\033[0m: %s Release  memory at %p\n", get_local_time(), ptr);
            memory_release++;

            memory_size_release += s;
            current_memory_size -= s;
        }

        log_enable(true);
    }
}

void memory_log_finish(void)
{
    fprintf(log_file, "==========================================Memory==========================================\n");
    fprintf(log_file, "Summary:\n");
    fprintf(log_file, "    %d node(s) of memory request allocated.\n", memory_allocate);
    fprintf(log_file, "    %d node(s) of memory request released.\n", memory_release);
    fprintf(log_file, "    %d node(s) of memory request not released.\n", memory_allocate - memory_release);
    fprintf(log_file, "    Total memory size allocated    in bytes : %ld\n", memory_size_allocate);
    fprintf(log_file, "    Total memory size released     in bytes : %ld\n", memory_size_release);
    fprintf(log_file, "    Total memory size not released in bytes : %ld\n", memory_size_allocate - memory_size_release);
    fprintf(log_file, "    Max   memory size allocated    in bytes : %ld\n", max_memory_size);
    if (memory_allocate - memory_release)
    {
        fprintf(log_file, "------------------------------------------------------------------------------------------\n\n");
        int index = 0;
        for (pair<void *, memory_node *> p : memory_map)
        {
            memory_node *node = p.second;
            trace *tr = node->get_trace();

            fprintf(log_file, "%02d. Memory at %p allocated at %s : %ld block(s) with size %ld\n", ++index, node->get_ptr(), node->get_trace()->get_trace_time(), node->get_block(), node->get_size());
            for (int i = 3; i < tr->get_trace_size(); i++)
            {
                fprintf(log_file, "\t\tat %p: %s\n", tr->get_back_trace()[i], tr->get_symbols()[i]);
            }
            
            fprintf(log_file, "\n------------------------------------------------------------------------------------------\n\n");
        }
    }
    else
    {
        fprintf(log_file, "\nNo memory leak!\n");
    }
    fprintf(log_file, "=======================================Memory Ended=======================================\n\n");
}
