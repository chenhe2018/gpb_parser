#!/etc/sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/protobuf/lib
export LIBRARY_PATH=$LIBRARY_PATH:`pwd`/protobuf/lib

export PATH=$PATH:`pwd`/protobuf/bin

cur_dir=`pwd`

# protoc -I=./ --cpp_out=./ *.proto
# 编译子文件夹中proto
protoc -I=${cur_dir}/message/ --cpp_out=${cur_dir}/message/ ${cur_dir}/message/*.proto