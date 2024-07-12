#include "Channel.hpp"

Channel::Channel() {
    std::cout << "Default constructor called" << std::endl;
}

Channel::Channel() {
    std::cout << "Parameterized constructor called" << std::endl;
}

Channel::Channel(const Channel& src) {
    std::cout << "Copy constructor called" << std::endl;
}

Channel& Channel::operator=(const Channel& src) {
}

Channel::~Channel() {
}

