cmake_minimum_required(VERSION 3.17)
project(project)

set(CMAKE_CXX_STANDARD 14)

add_executable(DataBaseLab main.cpp src/BufferManager.cpp src/BufferManager.h src/BCB.cpp src/BCB.h src/DataStorageManager.cpp src/DataStorageManager.h src/StorageStructure.h src/LRU.cpp src/LRU.h main.cpp)