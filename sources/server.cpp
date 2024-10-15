#include "EventPool.hpp"
#include "MasterHandler.hpp"
#include <iostream>
#include <exception>
#include <vector>
#include <csignal>
#include "HandlerType.hpp"

int	main(int argc __attribute__((unused)), char **argv)
try {
	MasterHandler					master;
	EventPool 						pool;
	HandlerManager<ClientHandler>	clients;
	EventPool::Event				event;

	// signal(SIGPIPE, SIG_IGN);
	master.listen(pool);
	while (true)
	{
		event = pool.get_event();
		try {
			switch (event.handler_type)
			{
				case MASTER_HANDLER:
					reinterpret_cast<MasterHandler*>(event.handler)->handle_event(event.flags, pool, clients);
					break;
				case CLIENT_HANDLER:
					reinterpret_cast<ClientHandler*>(event.handler)->handle_event(event.flags, pool, clients);
					break;
			}
		} catch (std::runtime_error error) {
			std::cerr << argv[0] << ": " << error.what() << std::endl;
		}
	}
} catch (std::runtime_error e) {
	std::cerr << argv[0] << ": " << e.what() << std::endl;
	return 1;
}

