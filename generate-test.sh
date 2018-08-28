#!/etc/sh

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/protobuf/lib
export LIBRARY_PATH=$LIBRARY_PATH:`pwd`/protobuf/lib

export PATH=$PATH:`pwd`/protobuf/bin

# 编译子文件夹中proto
protoc -I=./message/ --cpp_out=./message/ ./message/*.proto

cmake . && make

# main, with shared lib
g++ -o ./bin/main ./src/main.cpp ./message/family.pb.cc -std=c++11 -Wall -g -I`pwd`/protobuf/include -L`pwd`/protobuf/lib -lprotobuf -I`pwd` -L`pwd`/bin -lpbpath

# test-efficiency
g++ -o ./bin/t ./src/test-efficiency.cpp ./message/testmsg.pb.cc -Wall -std=c++11 -I./protobuf/include -L./protobuf/lib -lprotobuf