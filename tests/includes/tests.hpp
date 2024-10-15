#pragma once

#include <sys/types.h>
#include <stdexcept>

pid_t	launch_server() throw (std::runtime_error);
void	stop_server(pid_t server_pid) throw(std::runtime_error);
int		connect_client();
