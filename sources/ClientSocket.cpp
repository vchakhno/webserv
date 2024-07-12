#include "ClientSocket.hpp"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>

ClientSocket::ClientSocket(int fd) : fd(fd) {}

ClientSocket::~ClientSocket()
{
	close(fd);
}

void	ClientSocket::execute(int event_flags, EventPool &pool, ClientManager &clients)
{
	(void) pool;
	(void) clients;
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
