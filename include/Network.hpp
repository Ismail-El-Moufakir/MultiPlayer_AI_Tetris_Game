#include "SFML/Network.hpp"
#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "env.hpp"

bool SendData(sf::UdpSocket* socket, const char* data, std::size_t size, const sf::IpAddress& recipient, unsigned short port);
bool ReceiveData(sf::UdpSocket* socket, char* buffer, std::size_t size, std::size_t& received, sf::IpAddress& sender, unsigned short& port);
void SendCoordinates(Environnement* env, sf::UdpSocket* socket, unsigned short port);
void ReceiveCoordinates(sf::UdpSocket* socket, char dataHolder[8]);
int CheckConnection(sf::UdpSocket* socket) ;
void ReceiveMatrix(sf::UdpSocket* socket, int** Grid_Mtx);
void SendMatrix(Environnement* env, sf::UdpSocket* socket, unsigned short port, const sf::IpAddress& recipient);