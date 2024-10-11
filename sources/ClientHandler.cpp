#include "ClientHandler.hpp"
#include "HttpRequest.hpp"
#include "File.hpp"
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

void	ClientHandler::handle_event(
	int event_flags,
	EventPool &pool,
	HandlerManager<ScriptHandler> &scripts,
	HandlerManager<ClientHandler> &clients_manager
) throw (std::runtime_error)
try {
	(void) pool;
	(void) scripts;

	print_event_flags(event_flags);
	if (event_flags & EPOLLIN)
	{
		char					recv_buffer[RECV_SIZE];
		ssize_t					recv_size;
		size_t					previous_size;
		std::string::size_type	newline_pos;

		// std::cout << receive_line() << std::endl;

		if ((recv_size = recv(fd, recv_buffer, RECV_SIZE, 0)) == -1)
			throw std::runtime_error("Error reading from client socket");
		if (recv_size > 0)
		{
			previous_size = this->request_buffer.size();
			this->request_buffer.append(recv_buffer, recv_size);


			// TODO:
			// Put all this switch in HttpRequest inside of parse_request_chunk
			// Move request_buffer to HttpRequest

			switch (parsing_stage) {
				case PARSING_STAGE_REQUEST_LINE:
					std::string line;

					if ((newline_pos = this->request_buffer.find('\n', previous_size)) == std::string::npos)
						request.parse_request_line(line);

				case PARSING_STAGE_HEADER:
					while ((newline_pos = this->request_buffer.find('\n', previous_size)) != std::string::npos) {
						std::string line;
						line = this->request_buffer.substr(previous_size, newline_pos - previous_size);
						this->request.parse_request_header(line);
						previous_size = newline_pos;
					}

				case PARSING_STAGE_BODY:
					this->request_buffer.substr(previous_size, this->request_buffer.size() - previous_size);
					// body +=
					// if (content_length)
					//  -> done
			}
			std::cout << "Received " << recv_size << " bytes" << std::endl;
		}
	}
	if (event_flags & EPOLLOUT)
	{
		if (this->response_pos == this->response_buffer.size())
			return;

		ssize_t	last_send_size;
		
		if ((last_send_size = send(
				this->fd,
				&this->response_buffer[this->response_pos],
				this->response_buffer.size() - response_pos, 0
			)) == -1)
			throw std::runtime_error("Error sending to client socket");
		this->response_pos += last_send_size;
		if (this->response_pos == this->response_buffer.size())
		{
			std::cout << "Response fully sent" << std::endl;
			clients_manager.remove_handler(this);
		}
	}
	if (event_flags & EPOLLERR)
		clients_manager.remove_handler(this);
} catch (std::runtime_error error) {
	clients_manager.remove_handler(this); throw;
} catch (std::bad_alloc error) {
	clients_manager.remove_handler(this);
	throw std::runtime_error("Error allocating memory for the client handler");
}

// void ClientHandler::receive() throw(std::runtime_error)
// {
// }

// std::string ClientHandler::parse_line(std::string &line)
// {
	
// }

// std::string ClientHandler::receive_line()
// {

// }