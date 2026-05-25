#include <task.h>

task::task( std::string name, std::string description, int id ) : m_str_name( std::move( name ) ),
                                                                  m_str_description( std::move( description ) ),
                                                                  m_i_id( id ),
                                                                  m_b_completed( false )
{
    if ( m_str_name.empty( ) ) throw std::invalid_argument( "The name of a task can't be empty.\n" );
}

task::task( const task &t, int id ) : m_str_name( t.m_str_name ),
                                      m_str_description( t.m_str_description ),
                                      m_i_id( id ),
                                      m_b_completed( false )
{
    if ( m_i_id == t.get_id( ) ) throw std::invalid_argument( "The new cloned task can't have the same id as the original.\n" );
}

std::string task::get_name( ) const { return m_str_name; }

void task::change_name( const std::string &new_name )
{
    if ( new_name.empty( ) ) return;
    if ( new_name == m_str_name ) return;
    m_str_name = new_name;
}

std::string task::get_description( ) const { return m_str_description; }

void task::change_description( const std::string &new_description )
{
    if ( new_description == m_str_description ) return;
    m_str_description = new_description;
}

int task::get_id( ) const { return m_i_id; }

bool task::is_completed( ) const { return m_b_completed; }

void task::mark_as_completed( ) { m_b_completed = true; }

void task::mark_as_uncompleted( ) { m_b_completed = false; }