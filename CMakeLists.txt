cmake_minimum_required(VERSION 3.5)

project(gpacman_server VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp gpm_signals.cpp gpm_game.cpp gpm_auth.cpp gpm_config.cpp gamesrvmsg.cpp gamers.cpp)
set(HEADERS gpm_signals.hpp gpm_logging.hpp gpm_game.hpp gpm_auth.hpp gpm_config.hpp gpm_tcp_server.hpp gpm_session.hpp gpm_message.hpp gamesrvmsg.hpp gamers.hpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -D LOGGING_LEVEL_DEBUG -pthread")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)

# find_package(Boost COMPONENTS program_options thread REQUIRED)

find_package(Boost 1.74)

include_directories(${Boost_INCLUDE_DIRS})



list(APPEND CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules")

find_package(SFML 2.4 COMPONENTS system window graphics audio REQUIRED)
# find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

add_executable(gpacman_server ${SOURCES} ${HEADERS})

configure_file(gpacman_server.conf gpacman_server.conf COPYONLY)
configure_file(README.md README.md COPYONLY)

#target_include_directories (gpacman_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#target_link_libraries(gpacman_server  stdc++fs sfml-graphics ${Boost_LIBRARIES})

target_include_directories (gpacman_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gpacman_server ${SFML_LIBRARIES} ${Boost_LIBRARIES})
