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

	char request[] = "GET /note.txt HTTP/1.1\r\n\
Host: example.org\r\n\
User-Agent: Mozila/5.0 (Macintosh; U; Intel Mac OS X 10.6; fr; rv:1.9.2.8) Gecko/20100722 Firefox/3.6.8\r\n\
Accept: */*\r\n\
Accept-Language: fr,fr-fr;q=0.8,en-us;q=0.5,en;q=0.3\r\n\
Accept-Encoding: gzip,deflate\r\n\
Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n\
Keep-Alive: 115\r\n\
Connection: keep-alive\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\
X-Requested-With: XMLHttpRequest\r\n\
Referer: http://example.org/test\r\n\
Cookie: foo=bar; lorem=ipsum;\r\n\
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