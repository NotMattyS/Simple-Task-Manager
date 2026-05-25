#ifndef TASK_MANAGER_CLI_PARSE_CLI_H
#define TASK_MANAGER_CLI_PARSE_CLI_H

#include <algorithm>
#include <cctype>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <commands/i_command.h>

#include <commands/load_from_file_command.h>
#include <commands/reload_command.h>
#include <commands/save_command.h>
#include <commands/save_to_file_command.h>

#include <commands/print_file_command.h>

#include <commands/display_tasks_command.h>
#include <commands/see_task_command.h>

#include <commands/add_task_command.h>
#include <commands/delete_task_command.h>

#include <commands/copy_task_command.h>
#include <commands/edit_desc_command.h>
#include <commands/edit_name_command.h>

#include <commands/check_task_command.h>
#include <commands/mark_as_completed_command.h>
#include <commands/mark_as_uncompleted_command.h>

using command = std::unique_ptr< i_command >;
using arguments = std::vector< std::string >;

[[nodiscard]] command parse_cli_arguments( const arguments &args );

#endif //TASK_MANAGER_CLI_PARSE_CLI_H