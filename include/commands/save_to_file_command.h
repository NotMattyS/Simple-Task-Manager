#ifndef TASK_MANAGER_CLI_SAVE_TO_FILE_COMMAND_H
#define TASK_MANAGER_CLI_SAVE_TO_FILE_COMMAND_H

#include <string>
#include <task_manager.h>
#include <commands/i_command.h>

class save_to_file_command : public i_command
{
public:
    explicit save_to_file_command( std::string path ) : m_str_path( std::move( path ) ) { }

    void execute( task_manager &tm ) override { tm.save_to_file( m_str_path ); }

private:
    std::string m_str_path;
};

#endif //TASK_MANAGER_CLI_SAVE_TO_FILE_COMMAND_H