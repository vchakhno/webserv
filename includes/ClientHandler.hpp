#pragma once

#include <stdexcept>
#include "HandlerManager.tpp"
#include "FileHandler.hpp"
#include "CGIHandler.hpp"
#include "EventPool.hpp"

class ClientHandler {
public:
	ClientHandler(int fd);
	~ClientHandler();

	void	execute(
		int event_flags,
		EventPool &pool,
		HandlerManager<CGIHandler> &cgis,
		HandlerManager<FileHandler> &files
	);
private:
	int	fd;
};
