# pbpath

## CMake

Please install protobuf to `pwd`. The `protobuf` directory is empty.

>./clean.sh

>cmake .

>make

## compile

Compile test with `libpbpath.so` and `*.h`. 

>./generate-test.sh

or

>g++ -o test main.cpp family.pb.cc -std=c++11 -Wall -g -I`pwd`/protobuf/include -L`pwd`/protobuf/lib -lprotobuf -I`pwd` -L`pwd` -lpbpath

## execute test

The path of SHARED LIBRARY must be loaded first.

>cd `your root directory`

>cd bin

>export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/../protobuf/lib

>./test

## LOG

We use `LOG.h` for DEBUG. And we cancel it in present version.
