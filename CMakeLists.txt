cmake_minimum_required(VERSION 3.6)
project(fzy)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

configure_file(
    src/config.def.h
    ${CMAKE_BINARY_DIR}/config.h
    )

add_library(fzy
    src/match.c
    src/match.h
    src/bonus.h
    )
target_include_directories(fzy 
    PUBLIC src ${CMAKE_BINARY_DIR}
    )
target_compile_definitions(fzy 
    PUBLIC VERSION="1.0"
    )

add_executable(efzy
    src/fzy.c 
    src/tty.c 
    src/tty.h
    src/choices.c 
    src/choices.h
    src/options.c 
    src/options.h
    src/tty_interface.c
    src/tty_interface.h
    )
target_compile_options(efzy PUBLIC -pthread)
target_link_libraries(efzy fzy pthread)

