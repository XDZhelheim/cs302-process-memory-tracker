#include <map>
#include "main_log.h"
#include "file_handler_node.h"

using std::map;
using std::pair;

#include <unistd.h>

static map<FILE *, file_handler_node *> file_handler_map;
static int file_handler_allocate;
static int file_handler_release;

const char *types[2][4] = {{"File", "fopen ", "fclose", "filename"}, {"Pipe", "popen ", "pclose", " command"}};

void file_handler_log_init(void)
{
    file_handler_allocate = 0;
    file_handler_release = 0;
}

void file_handler_log_record(int type, FILE *f, const char *fname, int ftype)
{
    if (log_enabled())
    {
        log_enable(false);

        if (type)
        {
            file_handler_node *&node = file_handler_map[f];
            node = new file_handler_node(fname, f, ftype);

            fprintf(log_file, "%s %s %s at %p %s : %s\n", node->get_trace()->get_trace_time(), types[ftype][0], types[ftype][1], (void *)f, types[ftype][3], fname);
            fprintf(stdout, "\033[31;1mInfo\033[0m: %s %s %s at %p %s : %s\n", node->get_trace()->get_trace_time(), types[ftype][0], types[ftype][1], (void *)f, types[ftype][3], fname);
            file_handler_allocate++;
        }
        else
        {
            file_handler_map.erase(f);

            fprintf(log_file, "%s %s %s at %p\n", get_local_time(), types[ftype][0], types[ftype][2], (void *)f);
            fprintf(stdout, "\033[31;1mInfo\033[0m: %s %s %s at %p\n", get_local_time(), types[ftype][0], types[ftype][2], (void *)f);
            file_handler_release++;
        }

        log_enable(true);
    }
}

void file_handler_log_finish(void)
{
    fprintf(log_file, "=======================================File Handler=======================================\n");
    fprintf(log_file, "Summary:\n");
    fprintf(log_file, "    %d node(s) of file handler allocated.\n", file_handler_allocate);
    fprintf(log_file, "    %d node(s) of file handler released.\n", file_handler_release);
    fprintf(log_file, "    %d node(s) of file handler not released.\n", file_handler_allocate - file_handler_release);
    if (file_handler_allocate - file_handler_release)
    {
        fprintf(log_file, "------------------------------------------------------------------------------------------\n\n");
        int index = 0;
        for (pair<FILE *, file_handler_node *> p : file_handler_map)
        {
            file_handler_node *node = p.second;
            trace *tr = node->get_trace();

            fprintf(log_file, "%02d. %s at %p opened at %s %s : %s\n", ++index, types[node->get_ftype()][0], (void *)node->get_fh(), node->get_trace()->get_trace_time(), types[node->get_ftype()][3], node->get_name());

            for (int i = 3; i < tr->get_trace_size(); i++)
            {
                fprintf(log_file, "\t\tat %p: %s\n", tr->get_back_trace()[i], tr->get_symbols()[i]);
            }

            fprintf(log_file, "------------------------------------------------------------------------------------------\n\n");
        }
    }
    else
    {
        fprintf(log_file, "\nNo file handler leak!\n");
    }
    fprintf(log_file, "====================================File Handler Ended====================================\n\n");
}