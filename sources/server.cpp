#include "EventPool.hpp"
#include "MasterHandler.hpp"
#include <iostream>
#include <exception>
#include <vector>
#include <csignal>
#include "HandlerType.hpp"


/// The server's entrypoint.
///
/// This is where the setup functions are called, and where the main loop resides.
///
/// @param argc Unused for now. In the future, will be set to 2 if using a config file.
/// @param argv Contains the executable name in argv[0], and in the future the path to the config file in argv[1].
/// @return The exit status, 0 for manual termination (to be implemented), 1 for a failure during setup or when running out of memory.

int	main(int argc __attribute__((unused)), char **argv)
try {
	MasterConnection					master;
	EventPool 						pool;
	HandlerManager<ClientConnection>	clients;
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
					reinterpret_cast<MasterConnection*>(event.handler)->handle_event(event.flags, pool, clients);
					break;
				case CLIENT_HANDLER:
					reinterpret_cast<ClientConnection*>(event.handler)->handle_event(event.flags, pool, clients);
					break;
			}
		// This catch handles any IO, parsing, or allocation error concerning a specific connection.
		} catch (std::runtime_error error) {
			std::cerr << argv[0] << ": " << error.what() << std::endl;
		}
	}

// This catch handles any setup error such as:
// - Socket errors (tcp not supported, address already in use...)
// - Epoll errors
// - Allocation errors
//
// Note: The try block should logically only surround the constructors,
// 		 but then the declaration wouldn't be accessible from the main loop.
} catch (std::runtime_error e) {
	std::cerr << argv[0] << ": " << e.what() << std::endl;
	return 1;
}

