cmake_minimum_required(VERSION 3.16)
project(Testlog)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)  #设置编译条件c++ 14

find_package(Threads)
set(CMAKE_BUILD_TYPE "RELEASE")
#set(CMAKE_BUILD_TYPE "DEBUG")
 #set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
 #set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")  #性能分析工具pg
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg") #性能分析工具pg
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg") #性能分析工具pg

#link_directories(..)
aux_source_directory(. SRC1)
add_executable(Testlog ${SRC1})
#add_executable(TestLog ${SRC})

#target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT}) #引用thread
target_link_libraries(Testlog server_Log pthread)
#add_subdirectory(Test)
