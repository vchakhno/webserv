#pragma once

#include <stdexcept>
#include "Event.hpp"
#include "EventPool.hpp"
#include "ClientManager.hpp"

#define BACKLOG_SIZE 10

class MasterSocket : Event {
public:
	MasterSocket() throw (std::runtime_error);
	~MasterSocket();

	void	listen(EventPool &pool) throw(std::runtime_error);

	void	execute(int event_flags, EventPool &pool, ClientManager &clients);
private:
	int	fd;
};
