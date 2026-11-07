#include <iostream>
#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <string>

// Musimy dołączyć biblioteki sieciowe Windowsa
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

void skanuj_adres_ip(const std::string& ip_str) {
    // 1. Inicjalizacja narzędzia ICMP (do wysyłania pingów)
    HANDLE icmp_handle = IcmpCreateFile();
    if (icmp_handle == INVALID_HANDLE_VALUE) {
        std::cerr << "Blad inicjalizacji ICMP." << std::endl;
        return;
    }

    // Convert string IP do formatu sieciowego
    unsigned long ip_addr = inet_addr(ip_str.c_str());
    
    // Przygotowanie bufora na odpowiedź od urządzenia
    char send_data[] = "NexusScanData";
    DWORD reply_size = sizeof(ICMP_ECHO_REPLY) + sizeof(send_data);
    char* reply_buffer = new char[reply_size];

    // 2. WYSYŁANIE SYGNAŁU (Ping) z timeoutem 100ms (żeby skanowało szybko)
    DWORD reply_count = IcmpSendEcho(
        icmp_handle, ip_addr, send_data, sizeof(send_data),
        NULL, reply_buffer, reply_size, 100
    );

    // 3. SPRAWDZANIE ODPOWIEDZI
    if (reply_count > 0) {
        PICMP_ECHO_REPLY echo_reply = (PICMP_ECHO_REPLY)reply_buffer;
        if (echo_reply->Status == IP_SUCCESS) {
            // Urządzenie odpowiedziało! Jest aktywne w sieci.
            std::cout << "[AKTOWNE URZADZENIE] Znaleziono: " << ip_str 
                      << " (Czas odpowiedzi: " << echo_reply->RoundTripTime << "ms)" << std::endl;
        }
    }

    // Czyszczenie pamięci
    delete[] reply_buffer;
    IcmpCloseHandle(icmp_handle);
}

int main() {
    std::cout << "=== NexusSDK Network Scanner v1.0 ===" << std::endl;
    std::cout << "Skanowanie sieci lokalnej w toku..." << std::endl;

    // Zakładamy, że Twoja sieć domowa to np. 192.168.1.X
    // Program sprawdzi wszystkie końcówki od 1 do 50 (możesz zwiększyć do 254)
    std::string baza_sieci = "192.168.1.";

    // Inicjalizacja sieci Windows (WSA)
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);

    for (int i = 1; i <= 50; i++) {
        std::string aktualne_ip = baza_sieci + std::to_string(i);
        skanuj_adres_ip(aktualne_ip);
    }

    WSACleanup();
    std::cout << "Skanowanie zakonczone sukcesem." << std::endl;
    
    // Zatrzymaj okno konsoli
    std::cin.get();
    return 0;
}
