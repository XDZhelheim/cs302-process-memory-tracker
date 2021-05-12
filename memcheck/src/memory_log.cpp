#include "main_log.h"
#include "memory_node.h"
#include <map>

using std::map;
using std::pair;

static map<void *, memory_node *> mem_map;

void memory_log_record(int type, void *ptr, size_t size, size_t block)
{
    if (log_enabled())
    {
        log_enable(false);

        if (type)
        {
            memory_node *node = new memory_node(ptr, size, block);

            if (!node->valid_memory_allocation())
            {
                delete node;
                log_enable(true);
                return;
            }

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

        log_enable(true);
    }
}

void memory_log_finish(void)
{
    int all = mem_map.size();
    fprintf(stderr, "------------------------------------------Memory------------------------------------------\n");
    fprintf(stderr, "Summary:\n");
    fprintf(stderr, "    %d node(s) of memory not released.\n", all);
    if (all)
    {
        fprintf(stderr, "  --------------------------------------List start--------------------------------------\n\n");
        int index = 0;
        for (pair<void *, memory_node *> p : mem_map)
        {
            memory_node *node = p.second;
            trace *tr = node->get_trace();

            fprintf(stderr, "  %02d. Memory at %p allocated at %s : %ld block(s) with size %ld\n", ++index, node->get_ptr(), node->get_trace()->get_trace_time(), node->get_block(), node->get_size());
            for (int i = 3; i < tr->get_trace_size(); i++)
            {
                fprintf(stderr, "          at %p: %s\n", tr->get_back_trace()[i], tr->get_symbols()[i]);
            }
            free(node->get_ptr());
            fprintf(stderr, "     ---- Auto freed %p\n\n", node->get_ptr());
        }
        fprintf(stderr, "  --------------------------------------List ended--------------------------------------\n\n");
    }
    else
    {
        fprintf(stderr, "\nNo memory leak!\n");
    }
    fprintf(stderr, "---------------------------------------Memory Ended---------------------------------------\n\n");
}