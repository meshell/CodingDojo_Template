# CMake file to build the external dependencies
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# set the path to the additional cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules)

# set default built type to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

include(ExternalProject)

# create output directories
file(MAKE_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR})
if(MSVC)
  set(SCRIPT_SUFFIX .bat)
  set(LIB_DIR ${CMAKE_BUILD_TYPE})
else(MSVC)
  set(SCRIPT_SUFFIX .sh)
  set(LIB_DIR "")
endif(MSVC)

# Add an target to build all external dependencies
add_custom_target(external_dependencies)

add_custom_command(TARGET external_dependencies
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} ..
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

####################
# Google Mock
####################
find_package(GMock)
find_package(GTest)
if(NOT GMOCK_FOUND OR NOT GTEST_FOUND)
  ExternalProject_Add(googlemock
      INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}
      URL http://googlemock.googlecode.com/files/gmock-1.7.0.zip
      URL_MD5 073b984d8798ea1594f5e44d85b20d66
      UPDATE_COMMAND ""
      CMAKE_ARGS
        -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
        -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -Dgtest_force_shared_crt=${MSVC}
        -Dgtest_disable_pthreads=${MINGW}
        -DBUILD_SHARED_LIBS=OFF
      INSTALL_COMMAND
        ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/gtest/include <INSTALL_DIR>/include &&
        ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include <INSTALL_DIR>/include &&
        ${CMAKE_COMMAND} -E copy <BINARY_DIR>/${LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX} <INSTALL_DIR>/lib/ &&
        ${CMAKE_COMMAND} -E copy <BINARY_DIR>/gtest/${LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX} <INSTALL_DIR>/lib/ &&
        ${CMAKE_COMMAND} -E copy <BINARY_DIR>/${LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gmock_main${CMAKE_STATIC_LIBRARY_SUFFIX} <INSTALL_DIR>/lib/  &&
        ${CMAKE_COMMAND} -E copy <BINARY_DIR>/gtest/${LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX} <INSTALL_DIR>/lib/
  )

  # override GMock_LIBRARY_DIRS such that only the ones from external are used
  ExternalProject_Get_Property(googlemock INSTALL_DIR)
  set(GMOCK_LIBRARY_DIRS ${INSTALL_DIR})
  add_dependencies(external_dependencies googlemock)
endif(NOT GMOCK_FOUND OR NOT GTEST_FOUND)
