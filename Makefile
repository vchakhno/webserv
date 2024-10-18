NAME			:= ./webserv
PORT			:= 3000

SOURCES			:= \
					server.cpp \
					EventPool.cpp \
					MasterConnection.cpp \
					ClientConnection.cpp \
					ClientManager.cpp \
					HttpRequest.cpp
OBJECTS			:= $(SOURCES:.cpp=.o)
SOURCES			:= $(addprefix sources/,$(SOURCES))
OBJECTS			:= $(addprefix .build/,$(OBJECTS))
DEPENDENCIES	:= $(OBJECTS:.o=.d)

CXX				:= c++
CXXFLAGS		:= -Wall -Wextra -Werror -std=c++98 -Iincludes -MMD -MP -g3
DEFINES			:= -D PORT=$(PORT)

all: $(NAME)

-include $(DEPENDENCIES)

$(NAME): $(OBJECTS)
	$(CXX) $^ -o $@

.build/%.o: sources/%.cpp Makefile
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(DEFINES) -c $< -o $@

clean:
	rm -rf .build

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re