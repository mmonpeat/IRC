#include "Channel.hpp"

Channel::Channel(std::string name, Client *client) : _name(name), _topic("No topic is set"){
    //add the operator
    client->getNick(); //esta aqui de chorrada para que no se queje el make
    std::cout << "Channel " << this->_name << " constructor has been called" << std::endl;
    return;
}

Channel::~Channel(void) {
    // check if there are allocated memory and delete it before destruction!!!
    std::cout << "Channel " << this->_name << " destructor has been called" << std::endl;
    return;
}
