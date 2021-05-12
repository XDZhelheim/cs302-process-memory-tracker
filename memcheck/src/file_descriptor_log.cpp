#include <map>
#include "main_log.h"
#include "file_descriptor_node.h"

using std::map;
using std::pair;

#include <unistd.h>

static map<int, fdnode *> fd_map;

void file_descriptor_log_record(int type, int fd, const char *filename)
{
    if (log_enabled())
    {
        log_enable(false);

        if (type)
        {
            fdnode *&node = fd_map[fd];
            node = new fdnode(filename, fd);

            fprintf(stderr, "%s FILE open  fd %02d name %s\n", node->get_trace()->get_trace_time(), fd, filename);
        }
        else
        {
            fprintf(stderr, "%s FILE close fd %02d\n", get_local_time(), fd);

            fd_map.erase(fd);
        }

        log_enable(true);
    }
}

void file_descriptor_log_finish(void)
{
    int all = fd_map.size();
    fprintf(stderr, "--------------------------------------File Descriptor-------------------------------------\n");
    fprintf(stderr, "Summary:\n");
    fprintf(stderr, "    %d node(s) of file not released.\n", all);
    if (all)
    {
        fprintf(stderr, "  --------------------------------------List start--------------------------------------\n\n");
        int index = 0;
        for (pair<int, fdnode *> p : fd_map)
        {
            fdnode *node = p.second;
            trace *tr = node->get_trace();

            fprintf(stderr, "  %02d. File fd %02d opened at %s : %s\n", ++index, node->get_fd(), node->get_trace()->get_trace_time(), node->get_name());

            for (int i = 3; i < tr->get_trace_size(); i++)
            {
                fprintf(stderr, "          at %p: %s\n", tr->get_back_trace()[i], tr->get_symbols()[i]);
            }

            close(node->get_fd());
            fprintf(stderr, "     ---- Auto closed fd %d\n\n", node->get_fd());
        }
        fprintf(stderr, "  --------------------------------------List ended--------------------------------------\n\n");
    }
    else
    {
        fprintf(stderr, "\nNo file handler leak!\n");
    }
    fprintf(stderr, "-----------------------------------File Descriptor Ended----------------------------------\n\n");
}