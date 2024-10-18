#include "MasterHandler.hpp"
#include "HandlerType.hpp"
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <cerrno>
#include <string>

MasterConnection::MasterConnection() throw (std::runtime_error)
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
	master_address.sin_port = htons(PORT);
	if (bind(fd, (struct sockaddr *)&master_address, sizeof(master_address)) == -1)
	{
		close(fd);
		throw std::runtime_error(std::string(__FILE__) + ": " + strerror(errno));
	}
}

MasterConnection::~MasterConnection()
{
	close(fd);
}

void	MasterConnection::listen(EventPool &pool) throw (std::runtime_error)
{
	EventPool::Event master_event = {
		.flags = EPOLLIN,
		.handler_type = MASTER_HANDLER,
		.handler = this,
	};
	pool.observe(fd, master_event, "master socket");
	if (::listen(fd, BACKLOG_SIZE) == -1)
		throw std::runtime_error(std::string(__FILE__) + ": " + strerror(errno));
	std::cout << "Server listening on port " << PORT << std::endl;
}


void	MasterConnection::handle_event(int event_flags, EventPool &pool, HandlerManager<ClientConnection> &clients) throw (std::runtime_error)
{
	int					client_fd;
	ClientConnection		*client;

	(void) event_flags;
	std::cout << "Incoming connection" << std::endl;
	if ((client_fd = accept(fd, NULL, NULL)) == -1)
	{
		std::cerr << "Connection error" << std::endl;
		return;
	}

	try {
		client = new ClientConnection(client_fd);
		clients.add_handler(client);
	} catch (std::bad_alloc) {
		throw std::runtime_error("Error allocating memory for the client handler");
	}

	EventPool::Event client_event = {
		.flags = EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR | EPOLLRDHUP,
		.handler_type = CLIENT_HANDLER,
		.handler = client,
	};

	try {
		pool.observe(client_fd, client_event, "client socket");
	} catch (std::runtime_error) {
		clients.remove_handler(client); throw;
	}
}
