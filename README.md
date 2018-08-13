# pbpath

## CMake

Please install protobuf to `pwd`. The `protobuf` directory is empty.

## compile

Compile test with `libpbpath.so` and `*.h`. 

>g++ -o test main.cpp family.pb.cc -std=c++11 -Wall -g -I`pwd`/protobuf/include -L`pwd`/protobuf/lib -lprotobuf -I`pwd` -L`pwd` -lpbpath

