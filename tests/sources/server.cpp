#include <stdexcept>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

pid_t	launch_server() throw (std::runtime_error)
{
	pid_t server_pid = fork();
	if (server_pid == -1)
		throw std::runtime_error(
			"Server process creation failed\n"
			"Note: Check the process limit"
		);
	if (server_pid == 0)
	{
		extern char **environ;
		execve("../webserv", (char *[]){NULL}, environ);
		exit(125);
	}
	usleep(100000);

	int wstatus;
	if (waitpid(server_pid, &wstatus, WNOHANG) == server_pid
		&& WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 125)
		throw std::runtime_error(
			"Couldn't find or launch the server\n"
			"Note: Check the server's permissions and executable format"
		);
	return server_pid;
}

void	stop_server(pid_t server_pid) throw(std::runtime_error)
{
	int wstatus;

	kill(server_pid, SIGTERM);
	waitpid(server_pid, &wstatus, 0);
	// todo: implement CTRL+C in webserv
	// if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0)
	// 	throw std::runtime_error("The server reported an error");
	// std::cout << "Exited: " << (const char *[]){"false", "true"}[!!WIFEXITED(wstatus)] << std::endl;
	// std::cout << "Signaled: " << (const char *[]){"false", "true"}[!!WIFSIGNALED(wstatus)] << std::endl;
	// std::cout << "Termsig: " << WTERMSIG(wstatus) << std::endl;
	if (!WIFSIGNALED(wstatus) || WTERMSIG(wstatus) != SIGTERM)
		throw std::runtime_error("The server reported an error");
}
