#include <parse_cli.h>

namespace
{
    void notify( std::string message = "generic" ) { std::cout << std::format( "Something went wrong while parsing: {}\n", message ); }

    // Use in commands that requires at least 2 arguments
    [[nodiscard]] bool notify_if_missing( const arguments &args, size_t number_of_args )
    {
        if ( args.size( ) < number_of_args )
        {
            notify( "missing arguments" );
            return true;
        }

        return false;
    }

    void notify_if_exceeds( const arguments &args, size_t number_of_args )
    {
        if ( args.size( ) > number_of_args ) notify( "Warning: some arguments are not being captured by the parser." );
    }

    [[nodiscard]] std::optional< int > try_parse_id( const std::string &str_id )
    {
        try { return std::stoi( str_id ); }
        catch ( const std::invalid_argument & )
        {
            notify( "invalid arguments" );
            return std::nullopt;
        }
    }

    [[nodiscard]] std::string to_lower_case( std::string str )
    {
        if ( str.empty( ) ) return str;

        std::ranges::transform( str, str.begin( ), [ ]( unsigned char c ) { return std::tolower( c ); } );
        return str;
    }
}

command parse_cli_arguments( const arguments &args )
{
    if ( args.empty( ) ) return nullptr;

    const std::string &action = to_lower_case( args[ 0 ] );

    if ( action == "load" )
    {
        if ( notify_if_missing( args, 2u ) ) return nullptr;
        notify_if_exceeds( args, 2u );

        return std::make_unique< load_from_file_command >( args[ 1 ] );
    }


    if ( action == "save" )
    {
        if ( args.size( ) == 1 ) return std::make_unique< save_command >( );

        if ( notify_if_missing( args, 2u ) ) return nullptr;
        notify_if_exceeds( args, 2u );

        return std::make_unique< save_to_file_command >( args[ 1 ] );
    }

    if ( action == "reload" )
    {
        notify_if_exceeds( args, 1u );
        return std::make_unique< reload_command >( );
    }

    if ( action == "file" )
    {
        notify_if_exceeds( args, 1u );
        return std::make_unique< print_file_command >( );
    }

    if ( action == "display" )
    {
        notify_if_exceeds( args, 1u );
        return std::make_unique< display_tasks_command >( );
    }

    if ( action == "check" )
    {
        notify_if_exceeds( args, 1u );
        return std::make_unique< check_task_command >( );
    }
    
    if ( action == "see" )
    {
        if ( notify_if_missing( args, 2u ) ) return nullptr;
        notify_if_exceeds( args, 2u );

        std::optional< int > id = try_parse_id( args[ 1 ] );
        if ( !id.has_value( ) ) return nullptr;

        return std::make_unique< see_task_command >( id.value( ) );
    }


    if ( action == "add" )
    {
        if ( notify_if_missing( args, 3u ) ) return nullptr;
        notify_if_exceeds( args, 3u );

        if ( args[ 1 ].empty( ) )
        {
            notify( "invalid arguments" );
            return nullptr;
        }

        return std::make_unique< add_task_command >( args[ 1 ], args[ 2 ] );
    }

    if ( action == "delete" )
    {
        if ( notify_if_missing( args, 2u ) ) return nullptr;
        notify_if_exceeds( args, 2u );

        std::optional< int > id = try_parse_id( args[ 1 ] );
        if ( !id.has_value( ) ) return nullptr;

        return std::make_unique< delete_task_command >( id.value( ) );
    }

    if ( action == "edit" )
    {
        if ( notify_if_missing( args, 4u ) ) return nullptr;
        notify_if_exceeds( args, 4u );

        const std::string lower_cmd = to_lower_case( args[ 1 ] );

        if ( lower_cmd.empty( ) || ( lower_cmd != "name" && lower_cmd != "desc" && lower_cmd != "description" ) )
        {
            notify( "invalid arguments" );
            return nullptr;
        }

        std::optional< int > id = try_parse_id( args[ 2 ] );
        if ( !id.has_value( ) ) return nullptr;

        if ( lower_cmd == "name" )
        {
            if ( args[ 3 ].empty( ) )
            {
                notify( "invalid arguments" );
                return nullptr;
            }

            return std::make_unique< edit_name_command >( args[ 3 ], id.value( ) );
        }

        // Description can be empty
        return std::make_unique< edit_desc_command >( args[ 3 ], id.value( ) );
    }

    if ( action == "copy" )
    {
        if ( notify_if_missing( args, 2u ) ) return nullptr;
        notify_if_exceeds( args, 2u );

        std::optional< int > id = try_parse_id( args[ 1 ] );
        if ( !id.has_value( ) ) return nullptr;

        return std::make_unique< copy_task_command >( id.value( ) );
    }

    if ( action == "mark" )
    {
        if ( notify_if_missing( args, 2u ) ) return nullptr;
        notify_if_exceeds( args, 2u );

        std::optional< int > id = try_parse_id( args[ 1 ] );
        if ( !id.has_value( ) ) return nullptr;

        return std::make_unique< mark_as_completed_command >( id.value( ) );
    }

    if ( action == "unmark" )
    {
        if ( notify_if_missing( args, 2u ) ) return nullptr;
        notify_if_exceeds( args, 2u );

        std::optional< int > id = try_parse_id( args[ 1 ] );
        if ( !id.has_value( ) ) return nullptr;

        return std::make_unique< mark_as_uncompleted_command >( id.value( ) );
    }


    notify( "unknown command" );
    return nullptr;
}