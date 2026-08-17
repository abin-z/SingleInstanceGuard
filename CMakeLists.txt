cmake_minimum_required(VERSION 3.15...3.31)
project(SingleInstanceGuard    
    VERSION 1.0.0
    LANGUAGES CXX)

# ---------------------------------------------------------------------------------------
# Set default build to Release for single-config generators (Makefile/Ninja)
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  if((DEFINED PROJECT_IS_TOP_LEVEL AND PROJECT_IS_TOP_LEVEL)
    OR (NOT DEFINED PROJECT_IS_TOP_LEVEL AND CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR))
      set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
  endif()
endif()

add_library(${PROJECT_NAME}
    src/single_instance_guard.cpp
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ----------------------------------------------------------
# Detect master (top-level) project
# ----------------------------------------------------------
if(NOT DEFINED SINGLE_INSTANCE_GURARD_MASTER_PROJECT)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(SINGLE_INSTANCE_GURARD_MASTER_PROJECT ON)
  else()
    set(SINGLE_INSTANCE_GURARD_MASTER_PROJECT OFF)
  endif()
endif()

if(SINGLE_INSTANCE_GURARD_MASTER_PROJECT)
  message(STATUS "[SingleInstanceGuard] Master project: ${SINGLE_INSTANCE_GURARD_MASTER_PROJECT}")
  # Generate compile_commands.json for clangd/clang-tidy
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  # Enable PIC for all targets (safest for static libs)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

option(SINGLE_INSTANCE_GURARD_BUILD_EXAMPLES  "Build examples" ${SINGLE_INSTANCE_GURARD_MASTER_PROJECT})

if(SINGLE_INSTANCE_GURARD_BUILD_EXAMPLES)
  message(STATUS "[SingleInstanceGuard] Build examples: ${SINGLE_INSTANCE_GURARD_BUILD_EXAMPLES}")
  add_subdirectory(example)
endif()