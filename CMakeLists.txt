cmake_minimum_required(VERSION 3.0)
project(RunnersDB)


set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
find_package(Boost 1.40.0 REQUIRED system)
add_subdirectory(Parts)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/runners.csv
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

# Добавляем поддиректорию Parts, где находится наша библиотека

# Указываем путь к заголовочным файлам DB и Generator
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Parts/Parser)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Parts/Parser)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Patrs/DB)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Patrs/DB)


# Добавляем исполняемый файл generator и линкуем его с библиотекой db_parts
add_executable(generator generator.cpp)
add_executable(parser justParser.cpp)

target_link_libraries(parser db_part parser_part)
target_link_libraries(generator db_part)