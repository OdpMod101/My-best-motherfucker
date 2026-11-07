#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    std::cout << "=== Nexus Device Manager: KLIENT (Android/Linux) ===" << std::endl;

    std::string ip_celu;
    std::cout << "Wpisz IP telefonu/urzadzenia docelowego: ";
    std::cin >> ip_celu;

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Blad tworzenia socketu." << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_celu.c_str());
    server_addr.sin_port = htons(5252);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Blad polaczenia! Sprawdz IP i czy serwer działa." << std::endl;
        return 1;
    }

    std::cout << "Polaczono z sukcesem! Komendy: PING_TEST, LOG_TEST, EXIT" << std::endl;

    std::string komenda;
    char odpowiedz[1024];

    while (true) {
        std::cout << "\nNexusAndroid_Remote> ";
        std::cin >> komenda;

        send(client_socket, komenda.c_str(), komenda.size(), 0);

        if (komenda == "EXIT") break;

        std::memset(odpowiedz, 0, 1024);
        recv(client_socket, odpowiedz, 1024, 0);
        std::cout << "[ODPOWIEDŹ]: " << odpowiedz;
    }

    close(client_socket);
    return 0;
}
