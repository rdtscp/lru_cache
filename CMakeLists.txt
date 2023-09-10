cmake_minimum_required(VERSION 3.6.2)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-function -Werror")

project(lru_cache)

IF (CMAKE_BUILD_TYPE MATCHES Debug)
  IF (NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message("Enabling LeakSanitizer")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=leak")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=leak")
  ENDIF (NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

# Code Coverage Configuration
option(CODE_COVERAGE "Enable coverage reporting" OFF)
IF (CODE_COVERAGE)
  message("Enabling Code Coverage")
  add_library(coverage_config INTERFACE)

  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g --coverage")
  set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -O0 -g --coverage")
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
ENDIF (CODE_COVERAGE)

# Don't Install Google Test if its already installed.
IF(NOT GTEST_INSTALLED MATCHES TRUE)
  include(GoogleTest)
  
  find_package(Threads REQUIRED)
  
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
ENDIF(NOT GTEST_INSTALLED MATCHES TRUE)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

enable_testing()
add_subdirectory(test)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include
  DESTINATION include
)
