#include <map>
#include "main_log.h"
#include "file_handler_node.h"

using std::map;
using std::pair;

#include <unistd.h>

static map<FILE *, file_handler_node *> fh_map;

void file_handler_log_record(int type, FILE *f, const char *filename)
{
    if (log_enabled())
    {
        log_enable(false);

        if (type)
        {
            file_handler_node *&node = fh_map[f];
            node = new file_handler_node(filename, f);

            fprintf(stderr, "%s FILE open  at %p name %s\n", node->get_trace()->get_trace_time(), (void *)f, filename);
        }
        else
        {
            fprintf(stderr, "%s FILE close at %p\n", get_local_time(), (void *)f);

            fh_map.erase(f);
        }

        log_enable(true);
    }
}

void file_handler_log_finish(void)
{
    int all = fh_map.size();
    fprintf(stderr, "---------------------------------------File Handler---------------------------------------\n");
    fprintf(stderr, "Summary:\n");
    fprintf(stderr, "    %d node(s) of file not released.\n", all);
    if (all)
    {
        fprintf(stderr, "  --------------------------------------List start--------------------------------------\n\n");
        int index = 0;
        for (pair<FILE *, file_handler_node *> p : fh_map)
        {
            file_handler_node *node = p.second;
            trace *tr = node->get_trace();

            fprintf(stderr, "  %02d. File at %p opened at %s : %s\n", ++index, (void *)node->get_fh(), node->get_trace()->get_trace_time(), node->get_name());

            for (int i = 3; i < tr->get_trace_size(); i++)
            {
                fprintf(stderr, "          at %p: %s\n", tr->get_back_trace()[i], tr->get_symbols()[i]);
            }

            fclose(node->get_fh());
            fprintf(stderr, "     ---- Auto closed %p\n\n", (void *)node->get_fh());
        }
        fprintf(stderr, "  --------------------------------------List ended--------------------------------------\n\n");
    }
    else
    {
        fprintf(stderr, "\nNo file handler leak!\n");
    }
    fprintf(stderr, "------------------------------------File Handler Ended------------------------------------\n\n");
}