add_executable(bro)
target_sources(bro PRIVATE shell.c shell_ui.c)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(bro ${CURSES_LIBRARIES})