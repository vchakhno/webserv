#pragma once

#include <stdexcept>
#include "EventPool.hpp"
#include "ClientConnection.hpp"
#include "ClientManager.hpp"

#ifndef PORT
	#define PORT 3000
#endif

#define BACKLOG_SIZE 10

// Wrapper for the master socket.
//
// Creates and binds the socket in the constructor.
// Uses RAII to manage the fd.

class MasterConnection {
public:
	MasterConnection() throw (std::runtime_error);
	~MasterConnection();

	void	listen(EventPool &pool) throw(std::runtime_error);

	void	handle_event(int event_flags, EventPool &pool, ClientManager &clients) throw (std::runtime_error);
private:
	int	fd;
};
