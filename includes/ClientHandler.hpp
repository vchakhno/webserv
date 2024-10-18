#pragma once

#include <stdexcept>
#include "HandlerManager.tpp"
#include "EventPool.hpp"
#include <vector>
#include "HttpRequest.hpp"

#define RECV_SIZE 8192
#define READ_SIZE 8192

class ClientConnection {
public:
	ClientConnection(int fd);
	~ClientConnection();

	void	handle_event(
		int event_flags,
		EventPool &pool,
		HandlerManager<ClientConnection> &clients_manager
	) throw (std::runtime_error);
	void	receive_request() throw (std::runtime_error);
	void	send_response(HandlerManager<ClientConnection> &clients_manager) throw (std::runtime_error);


private:
	int						fd;
	HttpRequest				request;
	std::vector<uint8_t>	response_buffer;
	std::size_t				response_pos;
};
