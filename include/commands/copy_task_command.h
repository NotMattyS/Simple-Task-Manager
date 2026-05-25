#ifndef TASK_MANAGER_CLI_COPY_TASK_COMMAND_H
#define TASK_MANAGER_CLI_COPY_TASK_COMMAND_H

#include <task_manager.h>
#include <commands/i_command.h>

class copy_task_command : public i_command
{
public:
    explicit copy_task_command( int id ) : m_i_id( id ) { }

    void execute( task_manager &tm ) override { tm.copy_task( m_i_id ); }

private:
    int m_i_id;
};

#endif //TASK_MANAGER_CLI_COPY_TASK_COMMAND_H