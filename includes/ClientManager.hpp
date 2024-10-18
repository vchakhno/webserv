#pragma once

#include <set>
#include <iostream>
#include "ClientConnection.hpp"

class ClientManager {
public:
	ClientManager();
	~ClientManager();

	void add_handler(ClientConnection *handler) throw (std::bad_alloc);
	void remove_handler(ClientConnection* handler);
private:
	std::set<ClientConnection*>	handlers;
};
