#pragma once

#include <vector>
#include <netdb.h>
#include "ClientSocket.hpp"

class ClientSocket;

class ClientManager {
public:
	ClientManager();
	~ClientManager();

	// todo: add throw
	ClientSocket* add_client(int client_fd, struct sockaddr client_addr);
private:
	std::vector<ClientSocket*>	clients;
};