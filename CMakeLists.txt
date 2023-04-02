cmake_minimum_required(VERSION 3.5)

project(crontab LANGUAGES CXX C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/usr/include)
include_directories(/usr/local/include)

link_directories(/usr/lib)
link_directories(/usr/lib64)
link_directories(/usr/local/lib)

aux_source_directory(src SOURCES)

function(main)
    add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
endfunction(main)

main()