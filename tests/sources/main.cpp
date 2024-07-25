#include <criterion/criterion.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

// Returns true if the server was launched correctly, even if it terminated abruptly
pid_t	launch_server() {
	pid_t server_pid = fork();
	if (server_pid == -1)
		throw std::runtime_error(
			"Server process creation failed\n"
			"Note: Check the process limit"
		);
	if (server_pid == 0)
	{
		extern char **environ;
		char *args[] = {NULL};
		execve("../webserv", args, environ);
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

void	stop_server(pid_t server_pid)
{
	int wstatus;

	kill(server_pid, SIGTERM);
	waitpid(server_pid, &wstatus, 0);
	// todo: implement CTRL+C in webserv
	// if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0)
	// 	throw std::runtime_error("The server reported an error");
	if (!WIFSIGNALED(wstatus) || WTERMSIG(wstatus) != SIGTERM)
		throw std::runtime_error("The server reported an error");
}

int	connect_client() {
	struct protoent		*protocol_entry;
	int					protocol;
	struct sockaddr_in	server_address;
	int					client_fd;

	if ((protocol_entry = getprotobyname("tcp")) == NULL)
		throw std::runtime_error("The client couldn't retrieve the TCP protocol");

	protocol = protocol_entry->p_proto;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, protocol)) == -1)
		throw std::runtime_error(std::string("The client couldn't create a socket: ") + strerror(errno));

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(3002);
	inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
	if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		close(client_fd);
		throw std::runtime_error(std::string("The client couldn't connect to the server: ") + strerror(errno));
	}
	return client_fd;
}

// Tests if the server is up
Test(accepting_connnections, single_client) try {
	pid_t	server_pid = launch_server();

	int	client_socket = connect_client();
	usleep(100000);
	close(client_socket);
	stop_server(server_pid);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}
