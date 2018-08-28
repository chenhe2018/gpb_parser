cmake_minimum_required(VERSION 3.1)
project(gpb_parser)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -g -std=c++11")
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

execute_process(
#        COMMAND sh start-protoc.sh
		COMMAND mkdir ${CMAKE_SOURCE_DIR}/bin -p
		COMMAND cp ${CMAKE_SOURCE_DIR}/src/PbPath.h ${CMAKE_SOURCE_DIR}/bin/
)

include_directories(${CMAKE_SOURCE_DIR}/protobuf/include)
link_directories(${CMAKE_SOURCE_DIR}/protobuf/lib)

set(_TARGETNAME pbpath)
set(_SRCS src/PbPath.cpp)
add_library(${_TARGETNAME} SHARED ${_SRCS})
target_link_libraries(${_TARGETNAME} -lprotobuf)

#add_executable(main main.cpp family.pb.cc PbPath.cpp)
#target_link_libraries(main -lprotobuf)
