cmake_minimum_required(VERSION 3.10.2)
project(reactorServer)


find_package(Boost COMPONENTS thread)

if(NOT Boost_FOUND)
    message("NOT found Boost")
endif()


include_directories($(Boost_include_dir))
include_directories(
        include
)

set(CMAKE_CXX_FLAGS -g)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(utils)


add_executable(reactorServer
        main.cpp
        include/HEAD.h
        include/DEFINE.h
        src/Acceptor.cpp src/Acceptor.h include/BaseAcceptor.h
        src/MainReactor.cpp src/MainReactor.h
        src/RingBufferSession.cpp src/RingBufferSession.h include/BaseConnection.h
        src/SubReactor.cpp src/SubReactor.h 
        src/SubReactorMgr.cpp src/SubReactorMgr.h
        src/ReactorServer.cpp  src/ReactorServer.h include/BaseServer.h
        src/BaseSession.h
        src/FixedBufferSession.cpp src/FixedBufferSession.h
)

link_directories($(Boost_library_dir))
link_directories( ./utils/lib)

target_link_libraries(reactorServer ${Boost_LIBRARIES})
target_link_libraries(reactorServer asynclogger jsonreader)


