#pragma once

class ScriptHandler {
public:
	ScriptHandler();
	~ScriptHandler();

	void	handle_event(int event_flags);
};
