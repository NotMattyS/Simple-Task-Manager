#include <task_manager.h>

bool task_manager::notify_if_empty( ) const
{
    if ( m_v_tasks.empty( ) )
    {
        std::cout << "There is no task available. Add a new task with the command `add`.\n";
        return true;
    }

    return false;
}

void task_manager::load_from_file( std::string path )
{
    if ( path == "backup" )
    {
        std::cout << "Trying to load a backup...\n";
        load_from_file( m_str_path + ".back" );
        return;
    }

    if ( path.empty( ) || ( !path.ends_with( ".tsk" ) && !path.ends_with( ".tsk.back" ) ) )
    {
        std::cout << std::format( "Invalid path: {}\n", path );
        return;
    }

    std::ifstream file( path );

    if ( !file.is_open( ) )
    {
        std::cout << std::format( "Error: The file at the following path could not be opened: `{}`.\n", path );
        if ( path.ends_with( ".tsk" ) )
        {
            std::cout << "Attempting to load backup instead...\n";
            load_from_file( path + ".back" );
        }

        return;
    }

    std::vector< task > backup_tasks = std::move( m_v_tasks );
    const int backup_id = m_i_next_id;
    bool has_errors = false;
    bool is_fatal = false;

    m_v_tasks.clear( );
    std::string line;
    std::unordered_set< int > seen_ids;
    std::vector< std::string > error_messages;
    while ( std::getline( file, line ) )
    {
        if ( line.empty( ) ) continue;

        if ( line.starts_with( "lid:" ) )
        {
            try { m_i_next_id = std::stoi( line.substr( 4 ) ); }
            catch ( const std::invalid_argument & )
            {
                error_messages.emplace_back( "Error parsing next ID\n" );
                has_errors = true;
            }

            continue;
        }

        std::string id = line;
        std::string name, description, completed;

        // Try to open a backup
        if ( !std::getline( file, name ) || !std::getline( file, description ) || !std::getline( file, completed ) )
        {
            std::cout << "Corrupted file structure. Missing fields for a task.\n";
            m_v_tasks = std::move( backup_tasks );
            m_i_next_id = backup_id;
            is_fatal = true;
            break;
        }

        try
        {
            int parsed_id = std::stoi( id );

            if ( seen_ids.contains( parsed_id ) )
            {
                has_errors = true;
                error_messages.emplace_back( std::format( "Duplicated id: {}", parsed_id ) );
                continue;
            }

            int parsed_completion = std::stoi( completed );
            m_v_tasks.emplace_back( name, description, parsed_id );
            if ( parsed_completion == 1 ) m_v_tasks.back( ).mark_as_completed( );
            seen_ids.insert( parsed_id );

            std::cout << std::format( "Task has been created successfully with id `{}`.\n", parsed_id );
        }
        catch ( const std::invalid_argument &e )
        {
            error_messages.emplace_back( std::format( "Error while creating task: {}", e.what( ) ) );
            has_errors = true;
            continue;
        }
    }

    file.close( );

    std::cout << "\n";

    if ( is_fatal )
    {
        std::cout << std::format( "Fatal error found while loading {}.\n", path );
        if ( has_errors )
        {
            std::cout << "Some additional errors were found:\n";
            for ( const std::string &msg: error_messages ) std::cout << std::format( "{}\n", msg );
        }

        if ( path.ends_with( ".tsk" ) )
        {
            std::string backup_path = path + ".back";
            std::cout << std::format( "Attempting to recover by loading backup file: `{}`...\n", backup_path );

            load_from_file( backup_path );
            return;
        }

        std::cout << "Fatal error also occurred in the backup file. Recovery failed.\n";
        return;
    }

    if ( has_errors )
    {
        std::cout << "Tasks loaded, but some errors were found.\n";
        std::cout << "Not saving real path for security reasons.\n";
        m_str_path = path + ".temp";
        std::cout << std::format( "New file name: {}\n", m_str_path );
        std::cout << "Errors:\n";
        for ( const std::string &msg: error_messages ) std::cout << std::format( "{}\n", msg );
        return;
    }

    m_str_path = std::move( path );
    std::cout << "Tasks loaded successfully.\n";
}

void task_manager::save_to_file( const std::string &path )
{
    if ( !path.ends_with( ".tsk" ) && !path.ends_with( ".tsk.back" ) )
    {
        std::cerr << std::format( "Invalid path to save: {}\n", path );
        return;
    }

    if ( path.ends_with( ".tsk" ) )
    {
        const std::string backup_path = path + ".back";

        try
        {
            if ( std::filesystem::exists( backup_path ) ) std::filesystem::remove( backup_path );

            if ( std::filesystem::exists( path ) )
            {
                std::filesystem::copy_file( path, backup_path, std::filesystem::copy_options::overwrite_existing );

                std::cout << std::format( "Backup created successfully: {}\n", backup_path );
            }
        }
        catch ( const std::filesystem::filesystem_error &e ) { std::cerr << std::format( "Failed to create backup: {}\n", e.what( ) ); }
    }

    std::ofstream file( path );
    if ( file.is_open( ) )
    {
        file << std::format( "lid:{}\n", m_i_next_id );

        if ( !m_v_tasks.empty( ) )
        {
            for ( const task &t: m_v_tasks ) { file << std::format( "{}\n{}\n{}\n{}\n", t.get_id( ), t.get_name( ), t.get_description( ), t.is_completed( ) ? 1 : 0 ); }
        }

        file.close( );

        std::cout << "File saved correctly.\n";
        return;
    }

    std::cerr << std::format( "Couldn't open file for writing: {}\n", path );
}

void task_manager::save( )
{
    if ( m_str_path.ends_with( ".temp" ) )
    {
        std::cout << "Warning: Tried to save a temporary security file. Use command `save {path}` instead.\n";
        return;
    }

    std::cout << "Saving current file...\n";
    save_to_file( m_str_path );
}

void task_manager::reload( )
{
    std::cout << "Reloading current file...\n";
    load_from_file( m_str_path );
}

void task_manager::print_file_name( ) const { std::cout << std::format( "{}\n", m_str_path ); }

void task_manager::display_tasks( ) const
{
    constexpr auto RESET   = "\033[0m";
    constexpr auto BLUE    = "\033[94m";
    constexpr auto GREEN   = "\033[32m";
    constexpr auto YELLOW  = "\033[33m";

    if ( notify_if_empty( ) ) return;

    for ( size_t i = 0; i < m_v_tasks.size( ); i++ )
    {
        if ( i % 4 == 0 ) std::cout << "\n";
        const task &t = m_v_tasks[ i ];
        std::cout << std::format( "{}{}{}- {} - {}{}{}  |   ",BLUE, t.get_id( ), RESET, t.get_name( ),t.is_completed( ) ? GREEN : YELLOW, t.is_completed( ) ? "completed" : "pending", RESET);
    }

    std::cout << "\n";
}

void task_manager::clear_completed_tasks( )
{
    if ( notify_if_empty( ) ) return;

    const size_t removed = std::erase_if( m_v_tasks, [](const task &t ){ return t.is_completed( ); } );

    if ( removed == 0 )
    {
        std::cout << "There are no completed tasks to clear.\n";
        return;
    }
    std::cout << std::format( "{} completed task(s) cleared successfully.\n", removed );
}

void task_manager::see_task( int id ) const
{
    if ( notify_if_empty( ) ) return;

    for ( const task &t: m_v_tasks )
    {
        if ( t.get_id( ) == id )
        {
            std::cout << std::format( "{}. {} -- {}\n", t.get_id( ), t.get_name( ), t.is_completed( ) ? "completed" : "pending" );
            std::cout << std::format( "    {}\n", t.get_description( ) );

            return;
        }
    }

    std::cout << std::format( "There is no task with id `{}`.\n", id );
}

void task_manager::add_task( const std::string &name, const std::string &description )
{
    try
    {
        m_v_tasks.emplace_back( name, description, m_i_next_id );
        std::cout << std::format( "Task has been created successfully with id `{}`.\n", m_i_next_id );
        m_i_next_id++;
    }
    catch ( const std::invalid_argument &e )
    {
        std::cout << std::format( "Error while creating task: {}\n", e.what( ) );
    }
}

void task_manager::delete_task( int id )
{
    if ( notify_if_empty( ) ) return;

    if ( std::erase_if( m_v_tasks, [ id ]( const task &t ) { return t.get_id( ) == id; } ) == 0 )
    {
        std::cout << std::format( "Task with id `{}` doesn't exist.\n", id );
        return;
    }

    std::cout << "Task deleted successfully\n";
}

void task_manager::edit_task( const std::string &subcmd, int id, const std::string &new_str )
{
    if ( subcmd.empty( ) || ( subcmd != "name" && subcmd != "desc" ) )
    {
        std::cout << std::format( "Wrong indication of what to edit in task [ {} ], use `edit name` or `edit desc`.\n", id );
        return;
    }

    if ( notify_if_empty( ) ) return;

    task *target = nullptr;
    for ( auto &t: m_v_tasks )
    {
        if ( t.get_id( ) == id )
        {
            target = &t;
            break;
        }
    }

    if ( !target )
    {
        std::cout << std::format( "Task with id `{}` doesn't exist.\n", id );
        return;
    }

    if ( subcmd == "name" )
    {
        if ( new_str.empty( ) )
        {
            std::cout << "New name is empty.\n";
            return;
        }
        if ( new_str == target->get_name( ) )
        {
            std::cout << std::format( "New name is the same as old name: {}\n", target->get_name( ) );
            return;
        }

        target->change_name( new_str );

        std::cout << std::format( "Name changed successfully for task [ {} ].\n", id );
        return;
    }

    if ( subcmd == "desc" )
    {
        if ( new_str == target->get_description( ) )
        {
            std::cout << std::format( "New description is the same as old description: {}\n", target->get_description( ) );
            return;
        }

        target->change_description( new_str );
        std::cout << std::format( "Description changed successfully for task [ {} ].\n", id );
    }
}

void task_manager::copy_task( int id )
{
    if ( notify_if_empty( ) ) return;

    const task *original = nullptr;

    for ( const auto &t: m_v_tasks )
    {
        if ( t.get_id( ) == id )
        {
            original = &t;
            break;
        }
    }

    if ( !original )
    {
        std::cout << std::format( "Task with id `{}` doesn't exist.\n", id );
        return;
    }

    try
    {
        m_v_tasks.emplace_back( *original, m_i_next_id );
        std::cout << std::format( "Task has been created successfully with id `{}`.\n", m_i_next_id - 1 );
        m_i_next_id++;
    }
    catch ( const std::invalid_argument &e )
    {
        std::cout << std::format( "Error while creating task: {}\n", e.what( ) );
    }
}

void task_manager::mark_task_as_completed( int id )
{
    if ( notify_if_empty( ) ) return;

    for ( auto &t: m_v_tasks )
    {
        if ( t.get_id( ) == id )
        {
            if ( t.is_completed( ) )
            {
                std::cout << "The task is already completed.\nUse command `unmark` to mark it as uncompleted.\n";
                return;
            }

            t.mark_as_completed( );
            std::cout << std::format( "Task [ {} ] marked as completed.\n", id );
            return;
        }
    }

    std::cout << std::format( "Task with id `{}` doesn't exist.\n", id );
}

void task_manager::mark_task_as_uncompleted( int id )
{
    if ( notify_if_empty( ) ) return;

    for ( auto &t: m_v_tasks )
    {
        if ( t.get_id( ) == id )
        {
            if ( !t.is_completed( ) )
            {
                std::cout << "The task is already uncompleted.\nUse command `mark` to mark it as completed.\n";
                return;
            }

            t.mark_as_uncompleted( );
            std::cout << std::format( "Task [ {} ] marked as uncompleted.\n", id );
            return;
        }
    }

    std::cout << std::format( "Task with id `{}` doesn't exist.\n", id );
}

void task_manager::check_tasks( ) const
{
    const size_t total_tasks = m_v_tasks.size( );

    if ( total_tasks == 0 )
    {
        std::cout << "There is no task to analyze. Add one with the command `add`.\n";
        return;
    }

    size_t completed_tasks = 0;

    for ( const auto &t: m_v_tasks ) if ( t.is_completed( ) ) completed_tasks++;

    const size_t pending_tasks = total_tasks - completed_tasks;

    float completion_rate = static_cast< float >( completed_tasks ) / static_cast< float >( total_tasks ) * 100.0f;

    std::cout << "==============================================\n";
    std::cout << "             TASK MANAGER METRICS             \n";
    std::cout << "==============================================\n";
    std::cout << std::format( "Total of tasks   : {}\n", total_tasks );
    std::cout << std::format( "Completed tasks  : {}\n", completed_tasks );
    std::cout << std::format( "Pending tasks    : {}\n", pending_tasks );
    std::cout << std::format( "Efficiency rate  : {}%\n", completion_rate );
    std::cout << "==============================================\n";
}
