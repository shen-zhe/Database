cmake_minimum_required(VERSION 3.0)
project(RunnersDB)


set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_subdirectory(Parts)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/runners.csv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)


include_directories(${CMAKE_CURRENT_BINARY_DIR}/Parts/Parser)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Parts/Parser)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Patrs/DB)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Patrs/DB)


add_executable(generator generator.cpp)
add_executable(parser justParser.cpp)
add_executable(client client.cpp)
add_executable(server server.cpp)
add_executable(http_client http_client.cpp)


target_link_libraries(client db_part parser_part)
target_link_libraries(server db_part parser_part server_part)
target_link_libraries(parser db_part parser_part)
target_link_libraries(generator db_part)
target_link_libraries(http_client db_part parser_part)