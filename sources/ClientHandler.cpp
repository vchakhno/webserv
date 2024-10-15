#include "ClientHandler.hpp"
#include "HttpRequest.hpp"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

ClientHandler::ClientHandler(int fd) : fd(fd), response_pos() {}

ClientHandler::~ClientHandler()
{
	close(fd);
}

void	print_event_flags(int event_flags)
{
	if (event_flags & EPOLLIN)
		std::cout << "EPOLLIN ";
	if (event_flags & EPOLLOUT)
		std::cout << "EPOLLOUT ";
	if (event_flags & EPOLLERR)
		std::cout << "EPOLLERR ";
	if (event_flags & EPOLLHUP)
		std::cout << "EPOLLHUP ";
	if (event_flags & EPOLLRDHUP)
		std::cout << "EPOLLRDHUP ";
	std::cout << std::endl;
}

void	ClientHandler::receive_request() throw (std::runtime_error) {
	char	recv_buffer[RECV_SIZE];
	ssize_t	recv_size;

	if ((recv_size = recv(fd, recv_buffer, RECV_SIZE, 0)) == -1)
		throw std::runtime_error("Error reading from client socket");
	if (recv_size > 0)
	{
		std::cout << "Received " << recv_size << " bytes" << std::endl;
		this->request.parse_request_chunk(recv_buffer, recv_size);
		std::cout << "Parsing stage: " << (int) this->request.parsing_stage << std::endl;

		if (this->request.parsing_stage == PARSING_STAGE_DONE) {
			std::cout << "Request complete:\n" << this->request << std::endl;

			std::string response("Request complete\n");
			this->response_buffer.insert(this->response_buffer.end(), response.begin(), response.end());
		}
	} else {
		std::cout << "Request fully received" << std::endl;

		if (this->request.parsing_stage != PARSING_STAGE_DONE) {
			std::cout << "Incomplete request" << std::endl;

			std::string response("Incomplete request\n");
			this->response_buffer.insert(this->response_buffer.end(), response.begin(), response.end());
		}
	}
}

void	ClientHandler::send_response(HandlerManager<ClientHandler> &clients_manager) throw (std::runtime_error) {
	ssize_t	send_size;

	// If everything has been sent already, return
	if (response_pos == response_buffer.size())
		return;
	// Try to send everything present in the reponse buffer
	if ((send_size = send(fd, &response_buffer[response_pos], response_buffer.size() - response_pos, 0)) == -1)
		throw std::runtime_error("Error sending to client socket");

	std::cout << "Sent " << send_size << " bytes" << std::endl;
	// Update the cursor's position
	this->response_pos += send_size;
	if (this->response_pos == this->response_buffer.size())
	{
		std::cout << "Response fully sent" << std::endl;
		clients_manager.remove_handler(this);
	}
}

void	ClientHandler::handle_event(
	int event_flags,
	EventPool &pool,
	HandlerManager<ClientHandler> &clients_manager
) throw (std::runtime_error)
try {
	(void) pool;

	print_event_flags(event_flags);
	if (event_flags & EPOLLIN)
		this->receive_request();
	if (event_flags & EPOLLOUT)
		this->send_response(clients_manager);
	if (event_flags & EPOLLERR)
		clients_manager.remove_handler(this);
} catch (std::runtime_error error) {
	clients_manager.remove_handler(this); throw;
} catch (std::bad_alloc error) {
	clients_manager.remove_handler(this);
	throw std::runtime_error("Error allocating memory for the client handler");
}
