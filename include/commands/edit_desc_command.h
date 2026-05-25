#ifndef TASK_MANAGER_CLI_EDIT_DESC_COMMAND_H
#define TASK_MANAGER_CLI_EDIT_DESC_COMMAND_H

#include <string>
#include <task_manager.h>
#include <commands/i_command.h>

class edit_desc_command : public i_command
{
public:
    explicit edit_desc_command( std::string desc, int id ) : m_str_desc( std::move( desc ) ),
                                                             m_i_id( id ) { }

    void execute( task_manager &tm ) override { tm.edit_task( "desc", m_i_id, std::move( m_str_desc ) ); }

private:
    std::string m_str_desc;
    int m_i_id;
};

#endif //TASK_MANAGER_CLI_EDIT_DESC_COMMAND_H