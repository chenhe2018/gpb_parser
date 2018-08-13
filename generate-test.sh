export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/protobuf/lib
export LIBRARY_PATH=$LIBRARY_PATH:`pwd`/protobuf/lib

export PATH=$PATH:`pwd`/protobuf/bin

g++ -o `pwd`/test main.cpp family.pb.cc -std=c++11 -Wall -g -I`pwd`/protobuf/include -L`pwd`/protobuf/lib -lprotobuf -I`pwd`/ -L`pwd`/ -lpbpath 
