# Примеры запуска:
# ./cross-build.sh - сборка для Ubuntu
# WINDOWS=1 ./cross-build.sh - сборка для Windows

BUILD=$PWD/.build
WIN=$PWD/toolchain-win.cmake

rm -rf $BUILD
mkdir -p $BUILD
cd $BUILD

cmake ..
cmake --build .

mkdir Ubuntu
cp ../server.json ./Ubuntu
mv server ./Ubuntu
cd Ubuntu 
      
./server