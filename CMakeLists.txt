cmake_minimum_required(VERSION 3.16.0)
project(palin VERSION 0.1.0)

if(MSVC)
    # MSVC users should update the line below with the correct path to the
    # boost libraries.
    set(BOOST_ROOT D:/Dev/boost_1_78_0)
    add_compile_options(/permissive- /W4 /w14640)
elseif(UNIX)
    add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Werror)
endif()

find_package(Threads REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)
message(STATUS "Boost version: ${Boost_VERSION}")

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

file(GLOB SRCS "src/*.cpp")

add_executable(palin ${SRCS})
target_compile_features(palin PRIVATE cxx_std_17)
target_link_libraries(palin "${CMAKE_THREAD_LIBS_INIT}")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
