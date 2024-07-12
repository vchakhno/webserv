#pragma once

#include "EventPool.hpp"
#include <stdexcept>
#include <vector>

class ClientManager;

class Event {
protected:
	virtual ~Event() {}
public:
	virtual void execute(int event_flags, EventPool &pool, ClientManager &clients) = 0;
};
