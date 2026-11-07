#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    std::cout << "=== Nexus Device Manager: SERWER (Android/Linux) ===" << std::endl;

    // Tworzenie gniazda sieciowego (Socket) pod Linuxa
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Blad tworzenia socketu." << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Słuchaj z całej sieci Wi-Fi
    server_addr.sin_port = htons(5252);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Blad bindowania portu. Moze jest zajety?" << std::endl;
        return 1;
    }

    listen(server_socket, 3);
    std::cout << "Odbiornik uruchomiony. Czekam na polaczenie..." << std::endl;

    socklen_t addr_size = sizeof(server_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&server_addr, &addr_size);
    if (client_socket < 0) {
        std::cerr << "Blad akceptowania polaczenia." << std::endl;
        return 1;
    }
    std::cout << "[POŁĄCZENIE] Administrator sie zalogowal!" << std::endl;

    char buffer[1024];
    while (true) {
        std::memset(buffer, 0, 1024);
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        
        if (bytes_received <= 0) {
            std::cout << "Rozlaczono z administratorem." << std::endl;
            break;
        }

        std::string komenda(buffer);
        std::cout << "[ROZKAZ] Otrzymano: " << komenda << std::endl;

        if (komenda == "PING_TEST") {
            std::string msg = "Serwer Androida dziala i odpowiada.\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        } 
        else if (komenda == "LOG_TEST") {
            std::string msg = "[LOG] Wszystkie systemy stabilne w pamięci RAM.\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        }
        else if (komenda == "EXIT") {
            break;
        }
        else {
            std::string msg = "Nieznane polecenie.\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        }
    }

    close(client_socket);
    close(server_socket);
    return 0;
}
