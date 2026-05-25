#ifndef TASK_MANAGER_TASK_MANAGER_H
#define TASK_MANAGER_TASK_MANAGER_H

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <task.h>
#include <unordered_set>
#include <vector>

class task_manager
{
public:
    task_manager( ) { m_v_tasks.reserve( 64 ); }

    void load_from_file( std::string path );
    void save_to_file( const std::string& path );

    void save( );
    void reload( );

    void print_file_name( ) const;

    void display_tasks( ) const;
    void see_task( int id ) const;

    void add_task( const std::string &name, const std::string &description );
    void delete_task( int id );

    void edit_task( const std::string &subcmd, int id, const std::string &new_str  );
    void copy_task( int id );

    void mark_task_as_completed( int id );
    void mark_task_as_uncompleted( int id );
    void check_tasks( ) const;

private:
    std::vector< task > m_v_tasks;
    int m_i_next_id = 0;

    std::string m_str_path = "untitled.tsk";

    [[nodiscard]] bool notify_if_empty( ) const;
};

#endif //TASK_MANAGER_TASK_MANAGER_H