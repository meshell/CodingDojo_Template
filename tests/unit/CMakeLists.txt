# required minimum version of cmake
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# set the path to the additional cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/modules)

# set path to externals
set(CMAKE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../externals/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../externals/lib)

# Check for clang compiler
if(CMAKE_CXX_COMPILER MATCHES ".*clang")
  set(CMAKE_COMPILER_IS_CLANGCXX 1)
endif()

#########################################################################
# Create test executables for every testclass (no coverage measurement) #
#########################################################################

find_package(GMock)
find_package(GTest REQUIRED)

if(NOT TARGET internal::projectlib)
  include(${CMAKE_CURRENT_SOURCE_DIR}/../../project-export.cmake)
endif()

set(UNITTEST_HEADER_FILES
)

source_group("Header Files" FILES ${UNITTEST_HEADER_FILES})

set(UNITTEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/hiker_tests.cpp)

add_executable(unittests
    ${UNITTEST_SOURCES}
    ${UNITTEST_HEADER_FILES}
)
add_executable(tests::unit ALIAS unittests)

target_include_directories(unittests
    SYSTEM
    PRIVATE
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(unittests
    ${GTEST_BOTH_LIBRARIES}
    internal::projectlib
)
if(GMOCK_FOUND)
  target_include_directories(unittests
      SYSTEM
      PRIVATE
      ${GMOCK_INCLUDE_DIRS}
  )
  target_link_libraries(unittests
      ${GMOCK_BOTH_LIBRARIES}
  )
endif(GMOCK_FOUND)

if(CMAKE_COMPILER_IS_CLANGCXX)
  target_compile_options(unittests
      PRIVATE
      -Wno-global-constructors
  )
endif(CMAKE_COMPILER_IS_CLANGCXX)

if(UNIX)
  find_package(Threads) # GTest needs this and it's a static library
  target_link_libraries(unittests
      ${CMAKE_THREAD_LIBS_INIT}
  )
endif(UNIX)
