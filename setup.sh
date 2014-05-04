#! /bin/sh

DIRECTORY=build

# setup build directory
if [ ! -d $DIRECTORY ]; then 
    mkdir $DIRECTORY
else
    rm -rf $DIRECTORY
    mkdir $DIRECTORY
fi

# perform out of source build
cd $DIRECTORY && cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-gnueabi-BBB.cmake ../
make

# copy files to BBB
if [ $# -gt 0 ]; then 
    sudo scp ~/projects/src/caddy/$DIRECTORY/* root@192.168.7.2:/home/root/project/
fi
