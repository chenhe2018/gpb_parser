cmake_minimum_required(VERSION 3.1)
project(gpb_parser)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -g -std=c++11")
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

execute_process(
#        COMMAND sh ${CMAKE_SOURCE_DIR}/start-protoc.sh
		COMMAND mkdir ${CMAKE_SOURCE_DIR}/bin -p
		COMMAND cp ${CMAKE_SOURCE_DIR}/src/PbPath.h ${CMAKE_SOURCE_DIR}/bin/
)

include_directories(${CMAKE_SOURCE_DIR}/protobuf/include)
link_directories(${CMAKE_SOURCE_DIR}/protobuf/lib)

set(_TARGETNAME PbPath)
set(_SRCS src/PbPath.cpp)
add_library(${_TARGETNAME} SHARED ${_SRCS})
target_link_libraries(${_TARGETNAME} -lprotobuf)

# first executable declaration
add_executable(main test/main.cpp message/family.pb.cc src/PbPath.cpp)
target_link_libraries(main -lprotobuf)

# not work
#add_executable(t test/test-efficiency.cpp message/testmsg.pb.cc)
#target_link_libraries(${_TARGETNAME_T} -lprotobuf -lPbPath)

# second executalbe declaration
set(_TARGETNAME_T test-efficiency)
set(_SRCS_T test/test-efficiency.cpp src/PbPath.cpp message/testmsg.pb.cc)
add_executable(${_TARGETNAME_T} ${_SRCS_T})
target_link_libraries(${_TARGETNAME_T} -lprotobuf)


# CPack
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_DIRECTORY "${PROJECT_SOURCE_DIR}/pack")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "a")
set(PROJECT_VERSION_FULL ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(CPACK_SOURCE_IGNORE_FILES
        ${PROJECT_SOURCE_DIR}/build
        ${PROJECT_SOURCE_DIR}/cmake-build-debug
        ${PROJECT_SOURCE_DIR}/CMakeCache.txt
        ${PROJECT_SOURCE_DIR}/.git
        ${PROJECT_SOURCE_DIR}/.idea
        ${PROJECT_SOURCE_DIR}/pack
        )
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME PbPath-${PROJECT_VERSION_FULL})

include(CPack)