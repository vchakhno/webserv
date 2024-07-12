#include "ClientManager.hpp"
#include "ClientSocket.hpp"

ClientManager::ClientManager() {}

ClientManager::~ClientManager()
{
	for (size_t i = 0; i < clients.size(); i++)
		delete clients[i];
}

// todo: add throw
ClientSocket* ClientManager::add_client(int client_fd, struct sockaddr client_addr)
{
	ClientSocket	*client;

	(void) client_addr;
	client = new ClientSocket(client_fd);
	clients.push_back(client);
	return client;
}
