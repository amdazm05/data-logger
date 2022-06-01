cmake_minimum_required(VERSION 3.0)
project(datalogger)

add_library(
    LIBRARIES
        STATIC
    ${PROJECT_SOURCE_DIR}/src/datalogger/datalogger.cc
)

target_include_directories(
    LIBRARIES
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include/datalogger
)

add_executable(${PROJECT_NAME} src/main.cc)
target_link_libraries(${PROJECT_NAME}  PUBLIC LIBRARIES)


