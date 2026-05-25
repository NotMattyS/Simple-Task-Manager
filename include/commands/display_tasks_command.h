#ifndef TASK_MANAGER_CLI_DISPLAY_TASKS_COMMAND_H
#define TASK_MANAGER_CLI_DISPLAY_TASKS_COMMAND_H

#include <task_manager.h>
#include <commands/i_command.h>

class display_tasks_command : public  i_command
{
public:
    void execute(task_manager &tm) override { tm.display_tasks( ); }
};

#endif //TASK_MANAGER_CLI_DISPLAY_TASKS_COMMAND_H
