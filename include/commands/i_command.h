#ifndef TASK_MANAGER_CLI_COMMAND_H
#define TASK_MANAGER_CLI_COMMAND_H

class task_manager;

class i_command
{
public:
    virtual ~i_command( ) = default;
    virtual void execute( task_manager &tm ) = 0;
};

#endif //TASK_MANAGER_CLI_COMMAND_H