#! /bin/sh

DIRECTORY=build

# setup build directory
mkdir $DIRECTORY

# perform out of source build
cd $DIRECTORY && cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-gnueabi-BBB.cmake ../
make

# copy files to BBB
if [ $# -gt 0 ]; then 
    sudo scp $DIRECTORY/spi_test root@192.168.7.2:/home/root/project/
fi
