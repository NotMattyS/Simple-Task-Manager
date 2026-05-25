#ifndef TASK_MANAGER_CLI_MARK_AS_COMPLETED_COMMAND_H
#define TASK_MANAGER_CLI_MARK_AS_COMPLETED_COMMAND_H

#include <task_manager.h>
#include <commands/i_command.h>

class mark_as_completed_command : public i_command
{
public:
    explicit mark_as_completed_command( int id ) : m_i_id( id ) { }

    void execute( task_manager &tm ) override { tm.mark_task_as_completed( m_i_id ); }

private:
    int m_i_id;
};

#endif //TASK_MANAGER_CLI_MARK_AS_COMPLETED_COMMAND_H