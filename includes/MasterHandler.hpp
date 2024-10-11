#pragma once

#include <stdexcept>
#include "EventPool.hpp"
#include "ClientHandler.hpp"
#include "HandlerManager.tpp"

#ifndef PORT
	#define PORT 3000
#endif

#define BACKLOG_SIZE 10

class MasterHandler {
public:
	MasterHandler() throw (std::runtime_error);
	~MasterHandler();

	void	listen(EventPool &pool) throw(std::runtime_error);

	void	handle_event(int event_flags, EventPool &pool, HandlerManager<ClientHandler> &clients) throw (std::runtime_error);
private:
	int	fd;
};
