#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <cerrno>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

int	main(void)
{
	struct protoent		*protocol_entry;
	int					protocol;
	struct sockaddr_in	server_address;
	int					client_fd;

	if ((protocol_entry = getprotobyname("tcp")) == NULL)
		throw std::runtime_error("Couldn't retrieve protocol by name (tcp).");

	protocol = protocol_entry->p_proto;
	if ((client_fd = socket(AF_INET, SOCK_STREAM, protocol)) == -1)
		throw std::runtime_error(std::string(__FILE__) + ": " + strerror(errno));

	// Binds to all available interfaces
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(3002);
	if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
		close(client_fd);
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	if (connect(client_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
	{
		close(client_fd);
		throw std::runtime_error(std::string(__FILE__) + ": " + strerror(errno));
	}

	char request[] = "GET /note.txt HTTP/1.1\
Host: example.org\
User-Agent: Mozila/5.0 (Macintosh; U; Intel Mac OS X 10.6; fr; rv:1.9.2.8) Gecko/20100722 Firefox/3.6.8\
Accept: */*\
Accept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3\
Accept-Encoding: gzip,deflate\
Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\
Keep-Alive: 115\
Connection: keep-alive\
Content-Type: application/x-www-form-urlencoded\
X-Requested-With: XMLHttpRequest\
Referer: http://example.org/test\
Cookie: foo=bar; lorem=ipsum;\
";
	size_t	size = sizeof(request) / sizeof(*request);
	send(client_fd, request, size, 0);
	shutdown(client_fd, SHUT_WR);
	char	recv_buffer[100];
	ssize_t	recv_size;

	if ((recv_size = recv(client_fd, recv_buffer, 100, 0)) == -1)
	{
		std::cout << "Client failure" << std::endl;
		return 1;
	}

	printf("%.*s\n", (int) recv_size, recv_buffer);
}