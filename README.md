# pbpath

Compile test with `libpbpath.so` and `*.h`. 

>g++ -o test main.cpp family.pb.cc -std=c++11 -Wall -g -I`pwd`/protobuf/include -L`pwd`/protobuf/lib -lprotobuf -I`pwd` -L`pwd` -lpbpath

## 1.cmake

Compile project with these two lines.

>mkdir build && cd build

>cmake .. && make

## 2.adjust catalog

Subdirectory added in protoc compile command as follow.

>protoc -I=./message/ --cpp_out=./message/ ./message/*.proto

## 3.cpack

CPack after cmake.

>cpack --config CPackConfig.cmake

