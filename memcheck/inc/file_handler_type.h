#ifndef FILE_HANDLER_TYPE
#define FILE_HANDLER_TYPE

#define _FILE_ 0
#define _PIPE_ 1

struct file_handler_type
{
    const char *type;
    const char *open_command;
    const char *close_command;
    const char *name_type;
};

extern const struct file_handler_type types[];

#endif