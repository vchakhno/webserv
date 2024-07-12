#pragma once

#include <stdexcept>
#include "Event.hpp"
#include "ClientManager.hpp"

class ClientSocket : Event {
public:
	ClientSocket(int fd);
	~ClientSocket();

	void	execute(int event_flags, EventPool &pool, ClientManager &clients);
private:
	int	fd;
};
