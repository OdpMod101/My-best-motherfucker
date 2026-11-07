cmake_minimum_required(VERSION 3.22)
project(NexusNetworkTools)

set(CMAKE_CXX_STANDARD 20)

# Łączymy oba projekty w jeden potężny kombajn
add_subdirectory(01_Network_Scanner)
add_subdirectory(02_Device_Manager)
