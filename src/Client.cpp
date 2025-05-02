#include "Client.hpp"

Client::Client(): fd(-1) {}

Client::Client(int fd): fd(fd) {}

Client::~Client() {}
