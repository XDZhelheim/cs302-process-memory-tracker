#include "file_handler_type.h"

const struct file_handler_type types[] = 
    {
        {"File", "fopen ", "fclose", "filename"}, 
        {"Pipe", "popen ", "pclose", " command"}
    };