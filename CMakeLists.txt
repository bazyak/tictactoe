CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(tictactoe)
set(CMAKE_CXX_STANDARD 17)

file(GLOB EX src/*.cc)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(tictactoe ${EX})

target_include_directories(tictactoe PRIVATE src)
target_link_libraries(tictactoe ${CURSES_LIBRARIES})
