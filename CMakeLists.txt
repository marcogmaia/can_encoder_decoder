cmake_minimum_required(VERSION 3.2)

project(can)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(CAN_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(CAN_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(CAN_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

# set(CAN_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/division)

include_directories(${CAN_INCLUDE_DIR})
# include_directories(${CAN_HEADERS_DIR})

add_subdirectory(src)
add_executable(can src/main.c)
target_link_libraries(can canlib)
# add_subdirectory(test)