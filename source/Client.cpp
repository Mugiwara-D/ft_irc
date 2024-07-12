#include "Client.hpp"

Client::Client() {
    std::cout << "Default constructor called" << std::endl;
}

Client::Client() {
    std::cout << "Parameterized constructor called" << std::endl;
}

Client::Client(const Client& src) {
    std::cout << "Copy constructor called" << std::endl;
}

Client& Client::operator=(const Client& src) {
}

Client::~Client() {
}

