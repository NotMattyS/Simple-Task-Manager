#ifndef TASK_MANAGER_CLI_CLEAR_COMPLETED_COMMAND_H
#define TASK_MANAGER_CLI_CLEAR_COMPLETED_COMMAND_H

#include <task_manager.h>
#include <commands/i_command.h>

class clear_completed_command : public i_command
{
    public:
        explicit clear_completed_command( ) = default;
        void execute( task_manager &tm ) override { tm.clear_completed_tasks(); }

};

#endif //TASK_MANAGER_CLI_CLEAR_COMPLETED_COMMAND_H