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
	HandlerManager<ScriptHandler>	scripts;
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
					((MasterHandler *)event.handler)->handle_event(event.flags, pool, clients);
					break;
				case CLIENT_HANDLER:
					((ClientHandler *)event.handler)->handle_event(event.flags, pool, scripts, clients);
					break;
				case SCRIPT_HANDLER:
					((ScriptHandler *)event.handler)->handle_event(event.flags);
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

