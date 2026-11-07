# Budujemy dwa osobne pliki wykonywalne: dla odbiornika i nadajnika
add_executable(nexus_server main_server.cpp)
add_executable(nexus_client main_client.cpp)

if(WIN32)
    target_link_libraries(nexus_server ws2_32)
    target_link_libraries(nexus_client ws2_32)
endif()
