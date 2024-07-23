#pragma once

#include <string>
#include <stdexcept>
#include <sys/types.h>

struct File {
	File(const std::string &path) throw(std::runtime_error);
	~File();

	int 	fd;
	off_t	size;
};