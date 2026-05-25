#include <iostream>
#include <parse_cli.h>
#include <task_manager.h>

void print_commands( );

void trim_left( std::string &str );
void trim_right( std::string &str );

int main( )
{
    task_manager tm;
    bool running = true;

    std::cout << "-- Matt's task manager --\n";

    while ( running )
    {
        std::string line;
        std::getline( std::cin, line );

        if ( line.empty( ) ) continue;

        std::string arg;

        std::stringstream s_stream( line );

        std::getline( s_stream, arg, ' ' );

        if ( arg.empty( ) ) continue;

        if ( arg == "exit" )
        {
            running = false;
            std::cout << "Exiting program...\n";
            continue;
        }

        if ( arg == "help" )
        {
            print_commands( );
            continue;
        }

        arguments args;
        args.reserve( 4 );
        args.emplace_back( arg );

        while ( std::getline( s_stream, arg, ';' ) )
        {
            trim_left( arg );
            trim_right( arg );
            args.emplace_back( arg );
        }

        const command cmd = parse_cli_arguments( args );

        if ( !cmd ) continue;

        cmd->execute( tm );
    }

    return 0;
}

void print_commands( )
{
    std::cout << "help                          -- Display command matrix.\n";
    std::cout << "exit                          -- Terminate the session.\n\n";
    std::cout << "load <path>                   -- Load a specific database (.tsk).\n";
    std::cout << "reload                        -- Reload the active file descriptor.\n";
    std::cout << "save <path>                   -- Save changes to the specified path.\n";
    std::cout << "save                          -- Save changes to the current file.\n";
    std::cout << "file                          -- Print current working file name.\n\n";
    std::cout << "display                       -- Render all tasks in a 4-column matrix.\n";
    std::cout << "check                         -- Compute metrics (completion rate, pending count).\n";
    std::cout << "see <id>                      -- Inspect a specific task's metadata.\n";
    std::cout << "add <name>; <description>     -- Create a new task (supports spaces).\n";
    std::cout << "delete <id>                   -- Erase a task.\n";
    std::cout << "edit <field>; <id>; <new>     -- Change the specific field of a task to a new value.\n";
    std::cout << "copy <id>                     -- Clone an existing task with a new auto-incremented ID.\n";
    std::cout << "mark <id>                     -- Set task state to completed.\n";
    std::cout << "unmark <id>                   -- Set task state to pending.\n";
}

void trim_left( std::string &str )
{
    size_t first = str.find_first_not_of( " \t\n\r\f\v" );

    if ( first == std::string::npos )
    {
        str.clear( );
        return;
    }

    str = str.substr( first );
}

void trim_right( std::string &str )
{
    size_t last = str.find_last_not_of( " \t\n\r\f\v" );

    if ( last == std::string::npos ) return;

    str = str.substr( 0, last + 1 );
}