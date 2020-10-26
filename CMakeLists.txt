cmake_minimum_required(VERSION 3.5)

project(gpacman_server VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp gpm_signals.cpp gpm_game.cpp gpm_auth.cpp gpm_config.cpp)
set(HEADERS gpm_signals.hpp gpm_logging.hpp gpm_game.hpp gpm_auth.hpp gpm_config.hpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -D LOGGING_LEVEL_DEBUG")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost COMPONENTS program_options REQUIRED)


include_directories(${Boost_INCLUDE_DIRS})

add_executable(gpacman_server ${SOURCES} ${HEADERS})

target_include_directories (gpacman_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gpacman_server ${Boost_LIBRARIES})