cmake_minimum_required(VERSION 3.1)
project(gpb_parser)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -g -std=c++11")
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

# ILl
execute_process(
        COMMAND sh start-protoc.sh
)

# extra command
include_directories(${CMAKE_SOURCE_DIR}/protobuf/include)
link_directories(${CMAKE_SOURCE_DIR}/protobuf/lib)
include_directories(${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR})

set(_TARGETNAME pbpath)
set(_SRCS PbPath.cpp)
add_library(${_TARGETNAME} SHARED ${_SRCS})
target_link_libraries(${_TARGETNAME} -lprotobuf)

#add_executable(main main.cpp family.pb.cc)
#target_link_libraries(main -lprotobuf -lpbpath)
