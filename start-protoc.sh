export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/protobuf/lib
export LIBRARY_PATH=$LIBRARY_PATH:`pwd`/protobuf/lib

export PATH=$PATH:`pwd`/protobuf/bin

# protoc -I=./ --cpp_out=./ *.proto
# 编译子文件夹中proto
protoc -I=./message/ --cpp_out=./message/ ./message/*.proto
