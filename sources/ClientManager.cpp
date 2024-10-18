#include "ClientManager.hpp"

ClientManager::ClientManager() {}

ClientManager::~ClientManager()
{
	for (std::set<ClientConnection*>::iterator it = handlers.begin(); it != handlers.end(); it++)
		delete *it;
}

void ClientManager::add_handler(ClientConnection *handler) throw (std::bad_alloc)
{
	handlers.insert(handler);
	std::cout << "Handler added (" << handlers.size() << " currently)" << std::endl;
}

void ClientManager::remove_handler(ClientConnection* handler)
{
	std::set<ClientConnection*>::iterator it;
	
	it = handlers.find(handler);
	delete *it;
	handlers.erase(it);
	std::cout << "Handler removed (" << handlers.size() << " remaining)" << std::endl;
}