#ifndef TASK_MANAGER_TASK_H
#define TASK_MANAGER_TASK_H

#include <stdexcept>
#include <string>

class task
{
public:
    // Can't create an empty 'task'.
    task( ) = delete;
    // Can't copy a 'task' to create another one.
    task( const task & ) = delete;
    task & operator=( const task & ) = delete;

    task( std::string name, std::string description, int id );

    /**
     * @brief To copy a 'task' you need to pass the parameter 'id'.
     * @param t Task to be copied.
     * @param id id that the new task will have.
     */
    task( const task &t, int id );

    task( task && ) noexcept = default;
    task & operator=( task && ) = default;

    [[nodiscard]] std::string get_name( ) const;
    [[nodiscard]] std::string get_description( ) const;
    void change_name( const std::string &new_name );
    void change_description( const std::string &new_description );

    [[nodiscard]] int get_id( ) const;

    [[nodiscard]] bool is_completed( ) const;
    void mark_as_completed( );
    void mark_as_uncompleted( );

private:
    std::string m_str_name;
    std::string m_str_description;
    int m_i_id = 0;
    bool m_b_completed = false;
};

#endif //TASK_MANAGER_TASK_H