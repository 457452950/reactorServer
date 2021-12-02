cmake_minimum_required(VERSION 3.10.2)
project(reactorServer)

include_directories(
        utils/include
)

set(CMAKE_CXX_STANDARD 17)
set(CXX_FLAGS -g)

add_subdirectory(utils)


add_executable(reactorServer
        main.cpp
        include/HEAD.h
        src/Acceptor.cpp src/Acceptor.h include/BaseAcceptor.h
        src/MainReactor.cpp src/MainReactor.h
        include/DEFINE.h
        src/Connection.cpp src/Connection.h 
        src/SubReactor.cpp src/SubReactor.h
)

link_directories( ./utils/lib)
target_link_libraries(reactorServer logger jsonreader)


