#include <map>
#include "main_log.h"
#include "file_handler_node.h"

using std::map;
using std::pair;

#include <unistd.h>

static map<FILE *, file_handler_node *> file_handler_map;
static int file_handler_allocate;
static int file_handler_release;

void file_handler_log_init(void)
{
    file_handler_allocate = 0;
    file_handler_release = 0;
}

void file_handler_log_record(int type, FILE *f, const char *filename)
{
    if (log_enabled())
    {
        log_enable(false);

        if (type)
        {
            file_handler_node *&node = file_handler_map[f];
            node = new file_handler_node(filename, f);

            fprintf(log_file, "%s File fopen  at %p name %s\n", node->get_trace()->get_trace_time(), (void *)f, filename);
            fprintf(stdout, "\033[31;1mInfo\033[0m: %s File fopen  at %p name %s\n", node->get_trace()->get_trace_time(), (void *)f, filename);
            file_handler_allocate++;
        }
        else
        {
            file_handler_map.erase(f);

            fprintf(log_file, "%s File fclose at %p\n", get_local_time(), (void *)f);
            fprintf(stdout, "\033[31;1mInfo\033[0m: %s File fclose at %p\n", get_local_time(), (void *)f);
            file_handler_release++;
        }

        log_enable(true);
    }
}

void file_handler_log_finish(void)
{
    fprintf(log_file, "---------------------------------------File Handler---------------------------------------\n");
    fprintf(log_file, "Summary:\n");
    fprintf(log_file, "    %d node(s) of file handler allocated.\n", file_handler_allocate);
    fprintf(log_file, "    %d node(s) of file handler released.\n", file_handler_release);
    fprintf(log_file, "    %d node(s) of file handler not released.\n", file_handler_allocate - file_handler_release);
    if (file_handler_allocate - file_handler_release)
    {
        fprintf(log_file, "  --------------------------------------List start--------------------------------------\n\n");
        int index = 0;
        for (pair<FILE *, file_handler_node *> p : file_handler_map)
        {
            file_handler_node *node = p.second;
            trace *tr = node->get_trace();

            fprintf(log_file, "  %02d. File at %p opened at %s : %s\n", ++index, (void *)node->get_fh(), node->get_trace()->get_trace_time(), node->get_name());

            for (int i = 3; i < tr->get_trace_size(); i++)
            {
                fprintf(log_file, "          at %p: %s\n", tr->get_back_trace()[i], tr->get_symbols()[i]);
            }

            fclose(node->get_fh());
            fprintf(log_file, "     ---- Auto closed %p\n\n", (void *)node->get_fh());
        }
        fprintf(log_file, "  --------------------------------------List ended--------------------------------------\n\n");
    }
    else
    {
        fprintf(log_file, "\nNo file handler leak!\n");
    }
    fprintf(log_file, "------------------------------------File Handler Ended------------------------------------\n\n");
}