# Budujemy skaner jako plik wykonywalny
add_executable(nexus_scanner main.cpp)

# Na Windowsie dołączamy wymagane biblioteki sieciowe
if(WIN32)
    target_link_libraries(nexus_scanner ws2_32 iphlpapi)
endif()
