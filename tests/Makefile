NAME			:= ./test

SOURCES			:= main.cpp client.cpp server.cpp
OBJECTS			:= $(SOURCES:.cpp=.o)
SOURCES			:= $(addprefix sources/,$(SOURCES))
OBJECTS			:= $(addprefix .build/,$(OBJECTS))
DEPENDENCIES	:= $(OBJECTS:.o=.d)

CXX				:= c++
CXXFLAGS		:= -Wall -Wextra -Werror -std=c++11 -MMD -MP -g3
INCLUDES		:= includes ../includes libraries/criterion-2.4.2/include
INCLUDES		:= $(addprefix -I,$(INCLUDES))

LDFLAGS			:= -Wl,-rpath=libraries/criterion-2.4.2/lib -Llibraries/criterion-2.4.2/lib -lcriterion

all: $(NAME)

-include $(DEPENDENCIES)

$(NAME): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

.build/%.o: sources/%.cpp Makefile
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

clean:
	rm -rf .build

fclean: clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re