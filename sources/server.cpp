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
	HandlerManager<FileHandler>		files;
	EventPool::Event				event;

	signal(SIGPIPE, SIG_IGN);
	master.listen(pool);
	while (true)
	{
		event = pool.get_event();
		switch (event.handler_type)
		{
		case MASTER_HANDLER:
			((MasterHandler *)event.handler)->execute(event.flags, pool, clients);
			break;
		case CLIENT_HANDLER:
			((ClientHandler *)event.handler)->execute(event.flags, pool, scripts, files, clients);
			break;
		case SCRIPT_HANDLER:
			((ScriptHandler *)event.handler)->execute(event.flags);
			break;
		case FILE_HANDLER:
			((FileHandler *)event.handler)->execute(event.flags);
			break;
		}
	}
}
catch (std::runtime_error e) {
	std::cerr << argv[0] << ": " << e.what() << std::endl;
	return 1;
}

