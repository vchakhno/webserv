#pragma once

#include <stdexcept>
#include "HandlerManager.tpp"
#include "FileHandler.hpp"
#include "ScriptHandler.hpp"
#include "EventPool.hpp"

class ClientHandler {
public:
	ClientHandler(int fd);
	~ClientHandler();

	void	execute(
		int event_flags,
		EventPool &pool,
		HandlerManager<ScriptHandler> &scripts,
		HandlerManager<FileHandler> &files,
		HandlerManager<ClientHandler> &clients_manager
	);
private:
	int	fd;
	std::string	http_request;

};
