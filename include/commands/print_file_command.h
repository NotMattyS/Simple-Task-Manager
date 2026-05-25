#ifndef TASK_MANAGER_CLI_PRINT_FILE_COMMAND_H
#define TASK_MANAGER_CLI_PRINT_FILE_COMMAND_H

#include <task_manager.h>
#include <commands/i_command.h>

class print_file_command : public i_command
{
public:
    void execute( task_manager &tm ) override { tm.print_file_name( ); }
};

#endif //TASK_MANAGER_CLI_PRINT_FILE_COMMAND_H