#ifndef TASK_MANAGER_CLI_EXPORT_TO_JSON_COMMAND_H
#define TASK_MANAGER_CLI_EXPORT_TO_JSON_COMMAND_H

#include <string>
#include <task_manager.h>
#include <commands/i_command.h>

class export_to_json_command : public i_command
{
public:
    explicit export_to_json_command( std::string path ) : m_str_path( std::move( path ) ) { }

    void execute( task_manager &tm ) override { tm.export_to_json( m_str_path ); }

private:
    std::string m_str_path;
};

#endif //TASK_MANAGER_CLI_EXPORT_TO_JSON_COMMAND_H