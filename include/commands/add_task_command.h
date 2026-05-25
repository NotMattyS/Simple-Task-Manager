#ifndef TASK_MANAGER_CLI_ADD_TASK_COMMAND_H
#define TASK_MANAGER_CLI_ADD_TASK_COMMAND_H

#include <string>
#include <task_manager.h>
#include <commands/i_command.h>

class add_task_command : public i_command
{
public:
    add_task_command( std::string name, std::string description ) : m_str_name( std::move( name ) ),
                                                                    m_str_description( std::move( description ) ) { }

    void execute( task_manager &tm ) override { tm.add_task( std::move( m_str_name ), std::move( m_str_description ) ); }

private:
    std::string m_str_name, m_str_description;
};

#endif //TASK_MANAGER_CLI_ADD_TASK_COMMAND_H