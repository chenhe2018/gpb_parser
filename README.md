# pbpath

Compile test with `libpbpath.so` and `*.h`. 

>g++ -o test main.cpp family.pb.cc -std=c++11 -Wall -g -I`pwd`/protobuf/include -L`pwd`/protobuf/lib -lprotobuf -I`pwd` -L`pwd` -lpbpath

## cmake

>mkdir build && cd build

>cmake .. && make

## adjust catalog

>protoc -I=./message/ --cpp_out=./message/ ./message/*.proto

## cpack

>cpack --config CPackConfig.cmake

