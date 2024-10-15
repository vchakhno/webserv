#pragma once

#include <stdexcept>
#include "EventPool.hpp"
#include "ClientHandler.hpp"
#include "HandlerManager.tpp"

#ifndef PORT
	#define PORT 3000
#endif

#define BACKLOG_SIZE 10

// Wrapper for the master socket.
//
// Creates and binds the socket in the constructor.
// Uses RAII to manage the fd.

class MasterHandler {
public:
	MasterHandler() throw (std::runtime_error);
	~MasterHandler();

	void	listen(EventPool &pool) throw(std::runtime_error);

	void	handle_event(int event_flags, EventPool &pool, HandlerManager<ClientHandler> &clients) throw (std::runtime_error);
private:
	int	fd;
};
