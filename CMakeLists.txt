cmake_minimum_required(VERSION 3.10)
project(SingleInstanceGuard    
    VERSION 1.0.0
    LANGUAGES CXX)

add_library(${PROJECT_NAME}
    src/single_instance_guard.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)