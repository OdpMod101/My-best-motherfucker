#include <iostream>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    std::cout << "=== Nexus Device Manager: KLIENT (Konsola Sterujaca) ===" << std::endl;

    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);

    std::string ip_celu;
    std::cout << "Wpisz adres IP urzadzenia z zainstalowanym odbiornikiem: ";
    std::cin >> ip_celu;

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_celu.c_str());
    server_addr.sin_port = htons(5252);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Blad polaczenia! Upewnij sie, ze serwer na drugim PC jest wlaczony." << std::endl;
        std::cin.get();
        return 1;
    }

    std::cout << "Polaczono pomyślnie! Dostepne komendy: PING_TEST, OPEN_NOTEPAD, EXIT" << std::endl;

    std::string komenda;
    char odpowiedz[1024];

    while (true) {
        std::cout << "\nNexusSDK_Remote> ";
        std::cin >> komenda;

        send(client_socket, komenda.c_str(), komenda.size(), 0);

        if (komenda == "EXIT") break;

        memset(odpowiedz, 0, 1024);
        recv(client_socket, odpowiedz, 1024, 0);
        std::cout << "[ODPOWIEDŹ Z SERWERA]: " << odpowiedz;
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
