cmake_minimum_required(VERSION 3.1)
project(gpb_parser)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -g -std=c++11")

execute_process(
        COMMAND sh start-protoc.sh
)

include_directories(${CMAKE_SOURCE_DIR}/protobuf/include)
link_directories(${CMAKE_SOURCE_DIR}/protobuf/lib)

set(_TARGETNAME pbpath)
set(_SRCS gpb_msg_parser.cpp)
add_library(${_TARGETNAME} SHARED ${_SRCS})
target_link_libraries(${_TARGETNAME} -lprotobuf)

add_executable(main main.cpp family.pb.cc gpb_msg_parser.cpp gpb_msg_parser.h msg_parser.h LOG.h)
target_link_libraries(main -lprotobuf)
