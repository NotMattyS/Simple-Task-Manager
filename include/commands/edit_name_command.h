#ifndef TASK_MANAGER_CLI_EDIT_NAME_COMMAND_H
#define TASK_MANAGER_CLI_EDIT_NAME_COMMAND_H

#include <string>
#include <task_manager.h>
#include <commands/i_command.h>

class edit_name_command : public i_command
{
public:
    explicit edit_name_command( std::string name, int id ) : m_str_name( std::move( name ) ),
                                                             m_i_id( id ) { }

    void execute( task_manager &tm ) override { tm.edit_task( "name", m_i_id, std::move( m_str_name ) ); }

private:
    std::string m_str_name;
    int m_i_id;
};

#endif //TASK_MANAGER_CLI_EDIT_NAME_COMMAND_H