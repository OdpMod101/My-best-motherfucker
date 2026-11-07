#include <iostream>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    std::cout << "=== Nexus Device Manager: SERWER (Odbiornik) ===" << std::endl;

    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Słuchaj połączeń z całej sieci lokalnej
    server_addr.sin_port = htons(5252); // Nasz dedykowany port sterowania

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    std::cout << "Serwer uruchomiony. Czekam na polaczenie od administratora..." << std::endl;

    int addr_size = sizeof(server_addr);
    SOCKET client_socket = accept(server_socket, (struct sockaddr*)&server_addr, &addr_size);
    std::cout << "[POŁĄCZENIE] Administrator zalogowany!" << std::endl;

    char buffer[1024];
    while (true) {
        memset(buffer, 0, 1024);
        int bytes_received = recv(client_socket, buffer, 1024, 0);
        
        if (bytes_received <= 0) {
            std::cout << "Rozlaczono z administratorem." << std::endl;
            break;
        }

        std::string komenda(buffer);
        std::cout << "[ROZKAZ] Otrzymano polecenie: " << komenda << std::endl;

        // Legalne wykonywanie komend systemowych
        if (komenda == "PING_TEST") {
            std::string msg = "Serwer działa stabilnie.\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        } 
        else if (komenda == "OPEN_NOTEPAD") {
            system("notepad.exe"); // Uruchamia Notatnik na ekranie sterowanego PC
            std::string msg = "Uruchomiono Notatnik.\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        }
        else if (komenda == "EXIT") {
            break;
        }
        else {
            std::string msg = "Nieznana komenda.\n";
            send(client_socket, msg.c_str(), msg.size(), 0);
        }
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
