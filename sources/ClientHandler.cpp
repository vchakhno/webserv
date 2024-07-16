#include "ClientHandler.hpp"
#include <iostream>
#include <unistd.h>

ClientHandler::ClientHandler(int fd) : fd(fd) {}

ClientHandler::~ClientHandler()
{
	close(fd);
}

void	ClientHandler::execute(
	int event_flags,
	EventPool &pool,
	HandlerManager<CGIHandler> &cgis,
	HandlerManager<FileHandler> &files
) {
	(void) pool;
	(void) files;
	(void) cgis;
	if (event_flags & EPOLLIN)
		std::cout << "EPOLLIN ";
	if (event_flags & EPOLLOUT)
		std::cout << "EPOLLOUT ";
	if (event_flags & EPOLLERR)
		std::cout << "EPOLLERR ";
	if (event_flags & EPOLLHUP)
	{
		std::cout << "EPOLLHUP ";
		char c;
		read(0, &c, 1);
	}
	if (event_flags & EPOLLRDHUP)
	{
		std::cout << "EPOLLRDHUP ";
		write(fd, "a", 1);
	}
	std::cout << std::endl;
}
