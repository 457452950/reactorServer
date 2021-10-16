cmake_minimum_required(VERSION 3.10.2)
project(reactorServer)


find_package(Boost COMPONENTS thread)

if(NOT Boost_FOUND)
    message("NOT found Boost")
endif()


include_directories($(Boost_include_dir))
include_directories(include)

set(CMAKE_CXX_STANDARD 17)

add_executable(reactorServer
        src/main.cpp
        include/HEAD.h
        src/Acceptor.cpp src/Acceptor.h include/BaseAcceptor.h
        src/MainReactor.cpp src/MainReactor.h
        src/DEFINE.h
        include/Logger.cpp include/Logger.h
        include/JsonFileReader.cpp include/JsonFileReader.h
        src/Connection.cpp src/Connection.h src/SubReactor.cpp src/SubReactor.h)


link_directories($(Boost_library_dir))
target_link_libraries(reactorServer ${Boost_LIBRARIES})
