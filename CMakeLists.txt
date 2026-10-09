cmake_minimum_required(VERSION 3.20)

project(PigeiteCompute
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(boost_asio REQUIRED CONFIG)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(pigeite-server
    server/src/main.cpp
)

target_link_libraries(pigeite-server
    PRIVATE
    Boost::asio
     nlohmann_json::nlohmann_json
)

add_executable(pigeite-node
    node/src/main.cpp
)

target_link_libraries(pigeite-node
    PRIVATE
    Boost::asio
)

if(WIN32)
    target_link_libraries(pigeite-server PRIVATE ws2_32)
    target_link_libraries(pigeite-node PRIVATE ws2_32)
endif()