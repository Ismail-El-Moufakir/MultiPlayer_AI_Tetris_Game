#include "./include/Network.hpp"

// Helper function to send data
bool SendData(sf::UdpSocket* socket, const char* data, std::size_t size, const sf::IpAddress& recipient, unsigned short port) {
    if (socket->send(data, size, recipient, port) != sf::Socket::Done) {
        std::cerr << "Error: Failed to send data to " << recipient << " on port " << port << "." << std::endl;
        return false;
    }
    std::cout << "Data sent successfully to " << recipient << " on port " << port << "." << std::endl;
    return true;
}

// Helper function to receive data
bool ReceiveData(sf::UdpSocket* socket, char* buffer, std::size_t size, std::size_t& received, sf::IpAddress& sender, unsigned short& port) {
    if (socket->receive(buffer, size, received, sender, port) != sf::Socket::Done) {
        std::cerr << "Error: Failed to receive data." << std::endl;
        return false;
    }
    std::cout << "Received " << received << " bytes from " << sender << " on port " << port << "." << std::endl;
    return true;
}

// Send current piece coordinates
void SendCoordinates(Environnement* env, sf::UdpSocket* socket, unsigned short port) {
    if (!env || !env->Current_Piece) {
        std::cerr << "Error: Environnement or Current_Piece is not initialized." << std::endl;
        return;
    }

    char data[8];
    for (int i = 0; i < 4; ++i) {
        int row = env->Current_Piece->Coordinates[i][0];
        int col = env->Current_Piece->Coordinates[i][1];

        if (row < -128 || row > 127 || col < -128 || col > 127) {
            std::cerr << "Error: Coordinate value out of range for char." << std::endl;
            return;
        }

        data[i * 2] = static_cast<char>(row);
        data[i * 2 + 1] = static_cast<char>(col);
    }

    sf::IpAddress recipient = "127.0.0.1";
    SendData(socket, data, sizeof(data), recipient, port);
}

// Receive piece coordinates into a data holder
void ReceiveCoordinates(sf::UdpSocket* socket, char dataHolder[8]) {
    char data[8];
    std::size_t received;
    sf::IpAddress sender;
    unsigned short port;

    if (ReceiveData(socket, data, sizeof(data), received, sender, port)) {
        std::copy(data, data + 8, dataHolder);
    }
}

// Check connection by receiving a small packet
int CheckConnection(sf::UdpSocket* socket) {
    char data[1] = {0};
    std::size_t received;
    sf::IpAddress sender;
    unsigned short port;

    if (ReceiveData(socket, data, sizeof(data), received, sender, port)) {
        std::cout << "Connection check data: " << static_cast<int>(data[0]) << std::endl;
        return 1;
    }
    return 0;
}

// Receive the grid matrix
void ReceiveMatrix(sf::UdpSocket* socket, int** Grid_Mtx) {
    char data[200];
    std::size_t received;
    sf::IpAddress sender;
    unsigned short port;

    if (ReceiveData(socket, data, sizeof(data), received, sender, port)) {
        for (int i = 0; i < 200; ++i) {
            Grid_Mtx[i / 10][i % 10] = static_cast<int>(data[i]);
        }
    }
}

// Send the grid matrix
void SendMatrix(Environnement* env, sf::UdpSocket* socket, unsigned short port, const sf::IpAddress& recipient) {
    if (!env || !env->Grid_Mtx) {
        std::cerr << "Error: Environnement or Grid_Mtx is not initialized." << std::endl;
        return;
    }

    char data[200];
    for (int i = 0; i < 200; ++i) {
        int value = env->Grid_Mtx[i / 10][i % 10];

        if (value < -128 || value > 127) {
            std::cerr << "Error: Grid value out of range for char at position (" 
                      << i / 10 << ", " << i % 10 << "): " << value << std::endl;
            return;
        }

        data[i] = static_cast<char>(value);
    }

    SendData(socket, data, sizeof(data), recipient, port);
}
