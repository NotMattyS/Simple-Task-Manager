#ifndef TASK_MANAGER_CLI_CHECK_TASK_COMMAND_H
#define TASK_MANAGER_CLI_CHECK_TASK_COMMAND_H

#include <task_manager.h>
#include <commands/i_command.h>

class check_task_command : public i_command
{
public:
    void execute( task_manager &tm ) override { tm.check_tasks( ); }
};

#endif //TASK_MANAGER_CLI_CHECK_TASK_COMMAND_H