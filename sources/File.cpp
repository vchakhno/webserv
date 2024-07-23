#include "File.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sys/stat.h>

File::File(const std::string &path) throw(std::runtime_error)
{
	struct stat	statbuf;

	if ((this->fd = open(path.c_str(), O_RDONLY)) == -1)
		throw std::runtime_error(std::string("Error while opening ") + path + ": " + strerror(errno));
	if (stat(path.c_str(), &statbuf) == -1)
	{
		close(this->fd);
		throw std::runtime_error(std::string("Error while retrieving file size: ") + strerror(errno));
	}
	this->size = statbuf.st_size;
}

File::~File()
{
	close(this->fd);
}