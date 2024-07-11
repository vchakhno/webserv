#include "server.hpp"
#include "EventPool.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <netinet/in.h>

int master_socket_create() throw(std::runtime_error) {
	struct protoent		*protocol_entry;
	int					protocol;
	int					master_socket;
	struct sockaddr_in	master_address;

	if ((protocol_entry = getprotobyname("tcp")) == NULL)
		throw std::runtime_error("Couldn't retrieve protocol by name (tcp).");

	protocol = protocol_entry->p_proto;
	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw std::runtime_error(strerror(errno));

	// Binds to all available interfaces
	master_address.sin_addr.s_addr = INADDR_ANY;
	master_address.sin_family = AF_INET;
	master_address.sin_port = htons(3000);
	if (bind(master_socket, (struct sockaddr *)&master_address, sizeof(master_address)) == -1)
	{
		close(master_socket);
		throw std::runtime_error(strerror(errno));
	}
	return master_socket;
}

int	main(int argc __attribute__((unused)), char **argv)
try {
	int					master_socket;
	EventPool 			pool;
	EventPool::t_event	*event;

	master_socket = master_socket_create();
	pool.observe(master_socket, EPOLLIN);
	if (listen(master_socket, 10) == -1)
		throw std::runtime_error(strerror(errno));

	while (true)
	{
		event = pool.get_event();
		if (event->data.fd == master_socket)
		{
			std::cout << "Incoming connection" << std::endl;
		}
	}

	close(master_socket);
}

catch (std::runtime_error e) {
	std::cerr << argv[0] << ": " << e.what() << std::endl;
	return 1;
}

