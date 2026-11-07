cmake_minimum_required(VERSION 3.22)
project("NexusNetworkTools")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Budowanie Twojego silnika C++ z folderu src
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    add_library(nexus_core SHARED src/main.cpp)
    target_link_libraries(nexus_core log)
endif()

# 2. Automatyczne wejście do folderu z aplikacjami sieciowymi na Androida
add_subdirectory(02_Device_Manager)
