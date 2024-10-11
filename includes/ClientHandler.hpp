#pragma once

#include <stdexcept>
#include "HandlerManager.tpp"
#include "ScriptHandler.hpp"
#include "EventPool.hpp"
#include <vector>
#include "HttpRequest.hpp"

#define RECV_SIZE 8192
#define READ_SIZE 8192

class ClientHandler {
public:
	ClientHandler(int fd);
	~ClientHandler();

	void	handle_event(
		int event_flags,
		EventPool &pool,
		HandlerManager<ScriptHandler> &scripts,
		HandlerManager<ClientHandler> &clients_manager
	) throw (std::runtime_error);

	std::string	receive_line();
	void receive() throw(std::runtime_error);
	std::string parse_line(std::string &line);

private:
	int						fd;
	std::string				request_buffer;
	std::vector<uint8_t>	response_buffer;
	std::size_t				response_pos;
	HttpRequest				request;
};
