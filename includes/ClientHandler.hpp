#pragma once

#include <stdexcept>
#include "HandlerManager.tpp"
#include "ScriptHandler.hpp"
#include "EventPool.hpp"
#include <vector>

#define RECV_SIZE 8192
#define READ_SIZE 8192

class ClientHandler {
public:
	ClientHandler(int fd);
	~ClientHandler();

	void	execute(
		int event_flags,
		EventPool &pool,
		HandlerManager<ScriptHandler> &scripts,
		HandlerManager<ClientHandler> &clients_manager
	);
private:
	int						fd;
	std::string				request_buffer;
	std::vector<uint8_t>	response_buffer;
	size_t					response_pos;
};
