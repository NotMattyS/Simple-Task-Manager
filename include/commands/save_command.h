#ifndef TASK_MANAGER_CLI_SAVE_COMMAND_H
#define TASK_MANAGER_CLI_SAVE_COMMAND_H

#include <task_manager.h>
#include <commands/i_command.h>

class save_command : public i_command
{
public:
    void execute( task_manager &tm ) override { tm.save( ); }
};

#endif //TASK_MANAGER_CLI_SAVE_COMMAND_H