NAME			:= ./webserv

SOURCES			:= \
					server.cpp \
					EventPool.cpp \
					MasterHandler.cpp \
					ClientHandler.cpp \
					CGIHandler.cpp \
					FileHandler.cpp
OBJECTS			:= $(SOURCES:.cpp=.o)
SOURCES			:= $(addprefix sources/,$(SOURCES))
OBJECTS			:= $(addprefix .build/,$(OBJECTS))
DEPENDENCIES	:= $(OBJECTS:.o=.d)

CXX				:= c++
CXXFLAGS		:= -Wall -Wextra -Werror -std=c++98 -Iincludes -MMD -MP

LDFLAGS			:=

all: $(NAME)

-include $(DEPENDENCIES)

$(NAME): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

.build/%.o: sources/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

print:
	@echo $(DEPENDENCIES)

clean:
	rm -rf .build

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re