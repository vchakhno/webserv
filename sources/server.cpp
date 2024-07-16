#include "EventPool.hpp"
#include "MasterSocket.hpp"
#include <iostream>
#include <exception>
#include <vector>

int	main(int argc __attribute__((unused)), char **argv)
try {
	MasterSocket	master;
	EventPool 		pool;
	ClientManager	clients;
	int				event_flags;

	master.listen(pool);
	while (true)
	{
		Event &event = pool.get_event(event_flags);
		event.execute(event_flags, pool, clients);
	}
}
catch (std::runtime_error e) {
	std::cerr << argv[0] << ": " << e.what() << std::endl;
	return 1;
}

