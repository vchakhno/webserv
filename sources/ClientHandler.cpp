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

void	ClientHandler::execute(
	int event_flags,
	EventPool &pool,
	HandlerManager<ScriptHandler> &scripts,
	HandlerManager<ClientHandler> &clients_manager
) try {
	(void) pool;
	(void) scripts;

	// print_event_flags(event_flags);
	if (event_flags & EPOLLIN)
	{
		char		recv_buffer[RECV_SIZE];
		ssize_t		recv_size;

		if ((recv_size = recv(fd, recv_buffer, RECV_SIZE, 0)) == -1)
			throw std::runtime_error("Error while reading from client socket");
		if (recv_size > 0)
		{
			this->request_buffer.append(recv_buffer, recv_size);
			std::cout << "Received " << recv_size << " bytes" << std::endl;
		}
		else if (this->response_buffer.empty())
		{
			std::cout << "Received zero" << std::endl;
			HttpRequest	request = parse_http_request(this->request_buffer);
			this->request_buffer.clear();

			std::cout << "Request path: " << request.path << std::endl;

			File		file("website" + request.path);
			if (file.size > 2000000)
				throw std::runtime_error("Error: file too big");
			response_buffer.resize(file.size, 0);
			if ((read(file.fd, &response_buffer[0], file.size)) < file.size)
				throw std::runtime_error("Error while reading the requested file");
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
				this->response_buffer.size() - response_pos, MSG_DONTWAIT
			)) == -1)
			throw std::runtime_error("Error while sending to client socket");
		this->response_pos += last_send_size;
		std::cout << "Response fully sent" << std::endl;
		clients_manager.remove_handler(this->fd);
	}
	if (event_flags & EPOLLERR)
		clients_manager.remove_handler(this->fd);
}
catch (std::runtime_error error)
{
	std::cerr << error.what() << std::endl;
	clients_manager.remove_handler(this->fd);
}
catch (std::bad_alloc error)
{
	std::cerr << "Error while reserving RAM for the response" << std::endl;
	clients_manager.remove_handler(this->fd);
}