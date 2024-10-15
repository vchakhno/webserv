#include "tests.hpp"
#include <criterion/criterion.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

// Checks if the server is accepting clients
//
// Connects a client and disconnects it after 0.1s.
// If the client managed to connect successfully, the test passes.
Test(accepting_connections, single_client) try {
	pid_t	server_pid;
	int		client_socket;

	server_pid = launch_server();
	try {
		client_socket = connect_client();
	} catch (...) {
		stop_server(server_pid);
		throw;
	}
		
	// 0.1s
	usleep(100000);
	close(client_socket);

	stop_server(server_pid);
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

// Checks that the server refuses invalid method names
//
// Creates a client sends a request with an invalid method.
// The server is expected to respond with error code 400 (Bad Request) or 405 (Method not Allowed).
Test(request_line, invalid_method) try {
	pid_t	server_pid;
	int	client_socket;
	
	server_pid = launch_server();
	try {
		client_socket = connect_client();
	} catch (...) {
		stop_server(server_pid);
		throw;
	}

	// TODO:
	// Create a function that only takes request, max_response_size and timeout.
	// Automatically launches the server and everything.

	// Note: In the future, replace this with a proper request
	std::string	request = "GETTER /note.txt HTTP/1.1\r\n";
	ssize_t		send_size;

	// TODO: handler smaller packets
	if ((send_size = send(client_socket, request.data(), request.size(), 0)) != static_cast<ssize_t>(request.size())) {
		close(client_socket);
		stop_server(server_pid);
		throw std::runtime_error(std::string("The client failed to send the request: ") + strerror(errno));
	}

	char	response[14];
	ssize_t	response_size = std::string("HTTP/1.1 400 ").size();
	ssize_t	recv_size;

	// TODO: handler smaller packets
	if ((recv_size = recv(client_socket, response, response_size, 0)) != response_size) {
		close(client_socket);
		stop_server(server_pid);
		throw std::runtime_error(std::string("The client failed to retreive the response: ") + strerror(errno));
	}
	response[recv_size] = '\0';
	printf("Actual response: %.*s\n", (int) recv_size, response);

	close(client_socket);
	stop_server(server_pid);

	if (response != std::string("HTTP/1.1 400 ")
		&& response != std::string("HTTP/1.1 405 "))
		throw std::runtime_error(std::string("Wrong status code, expected 400 or 405."));
} catch (std::runtime_error e) {
	cr_assert(false, "Error: %s", e.what());
}

