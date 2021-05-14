#include <map>
#include "main_log.h"
#include "file_descriptor_node.h"

using std::map;
using std::pair;

#include <unistd.h>

static map<int, file_descriptor_node *> file_descriptor_map;
static int file_descriptor_allocate;
static int file_descriptor_release;

void file_descriptor_log_init(void)
{
    file_descriptor_allocate = 0;
    file_descriptor_release = 0;
}

void file_descriptor_log_record(int type, int fd, const char *filename)
{
    if (log_enabled())
    {
        log_enable(false);

        if (type)
        {
            file_descriptor_node *&node = file_descriptor_map[fd];
            node = new file_descriptor_node(filename, fd);

            fprintf(log_file, "%s File open  fd %02d name %s\n", node->get_trace()->get_trace_time(), fd, filename);
            fprintf(stdout, "\033[31;1mInfo\033[0m: %s File open  fd %02d name %s\n", node->get_trace()->get_trace_time(), fd, filename);
            file_descriptor_allocate++;
        }
        else
        {
            file_descriptor_map.erase(fd);

            fprintf(log_file, "%s File close fd %02d\n", get_local_time(), fd);
            fprintf(stdout, "\033[31;1mInfo\033[0m: %s File close fd %02d\n", get_local_time(), fd);
            file_descriptor_release++;
        }

        log_enable(true);
    }
}

void file_descriptor_log_finish(void)
{
    fprintf(log_file, "======================================File Descriptor=====================================\n");
    fprintf(log_file, "Summary:\n");
    fprintf(log_file, "    %d node(s) of file descriptor allocated.\n", file_descriptor_allocate);
    fprintf(log_file, "    %d node(s) of file descriptor released.\n", file_descriptor_release);
    fprintf(log_file, "    %d node(s) of file descriptor not released.\n", file_descriptor_allocate - file_descriptor_release);
    if (file_descriptor_allocate - file_descriptor_release)
    {
        fprintf(log_file, "------------------------------------------------------------------------------------------\n\n");
        int index = 0;
        for (pair<int, file_descriptor_node *> p : file_descriptor_map)
        {
            file_descriptor_node *node = p.second;
            trace *tr = node->get_trace();

            fprintf(log_file, "%02d. File fd %02d opened at %s : %s\n", ++index, node->get_fd(), node->get_trace()->get_trace_time(), node->get_name());

            for (int i = 3; i < tr->get_trace_size(); i++)
            {
                fprintf(log_file, "\t\tat %p: %s\n", tr->get_back_trace()[i], tr->get_symbols()[i]);
            }

            close(node->get_fd());
            fprintf(log_file, "\t---- Auto closed fd %d\n\n", node->get_fd());
            fprintf(log_file, "------------------------------------------------------------------------------------------\n\n");
        }
    }
    else
    {
        fprintf(log_file, "\nNo file descriptor leak!\n");
    }
    fprintf(log_file, "===================================File Descriptor Ended==================================\n\n");
}