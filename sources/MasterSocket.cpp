#include "MasterSocket.hpp"
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <cerrno>
#include <string>

MasterSocket::MasterSocket() throw (std::runtime_error)
{
	struct protoent		*protocol_entry;
	int					protocol;
	struct sockaddr_in	master_address;

	if ((protocol_entry = getprotobyname("tcp")) == NULL)
		throw std::runtime_error("Couldn't retrieve protocol by name (tcp).");

	protocol = protocol_entry->p_proto;
	if ((fd = socket(AF_INET, SOCK_STREAM, protocol)) == -1)
		throw std::runtime_error(std::string(__FILE__) + ": " + strerror(errno));

	// Binds to all available interfaces
	master_address.sin_addr.s_addr = INADDR_ANY;
	master_address.sin_family = AF_INET;
	master_address.sin_port = htons(3004);
	if (bind(fd, (struct sockaddr *)&master_address, sizeof(master_address)) == -1)
	{
		close(fd);
		throw std::runtime_error(std::string(__FILE__) + ": " + strerror(errno));
	}
}

MasterSocket::~MasterSocket()
{
	close(fd);
}

void	MasterSocket::listen(EventPool &pool) throw (std::runtime_error)
{
	pool.observe(fd, EPOLLIN, *this);
	if (::listen(fd, BACKLOG_SIZE) == -1)
		throw std::runtime_error(std::string(__FILE__) + ": " + strerror(errno));
}

void	MasterSocket::execute(int event_flags, EventPool &pool, ClientManager &clients)
{
	struct sockaddr	client_address;
	socklen_t		addr_len;
	int				client_fd;
	ClientSocket	*client;

	(void) event_flags;
	std::cout << "Incoming connection" << std::endl;
	if ((client_fd = accept(fd, &client_address, &addr_len)) == -1)
	{
		std::cerr << "Connection error" << std::endl;
		return;
	}
	client = clients.add_client(client_fd, client_address);
	pool.observe(client_fd, EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLRDHUP, *(Event *)client);
}
