#pragma once

class CGIHandler {
public:
	CGIHandler();
	~CGIHandler();

	void	execute(int event_flags);
};
