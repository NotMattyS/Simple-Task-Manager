cmake_minimum_required( VERSION 3.20 )
project( task_manager_cli VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

add_library( task_util src/task.cpp src/task_manager.cpp )
target_include_directories( task_util PUBLIC include )

add_library( parse_cli src/parse_cli.cpp )
target_link_libraries( parse_cli PRIVATE task_util )

add_executable( task_manager_cli src/main.cpp )
target_link_libraries( task_manager_cli PRIVATE task_util parse_cli )

if ( CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    target_link_options( task_manager_cli PRIVATE -static -static-libgcc -static-libstdc++ )
endif ( )