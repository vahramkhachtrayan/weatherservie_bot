# Setting environment variables directly in the Makefile
export TELEBOT_INCLUDE = ./telebot/include
export RUST_WEATHER_INCLUDE = ./rust_weather/include
export TELEBOT_LIB = ./telebot/lib
export RUST_WEATHER_LIB = ./rust_weather/target/release

# Compiler and linker settings
CC = gcc
CFLAGS = -Wall -std=c99 -I$(TELEBOT_INCLUDE) -I$(RUST_WEATHER_INCLUDE)
LDFLAGS = -L$(TELEBOT_LIB) -L$(RUST_WEATHER_LIB)
LDLIBS = -ltelebot -lweather -lapr-1 -lcurl -pthread

# Source files and output
SRCS = weather_bot.c
OBJS = $(SRCS:.c=.o)
EXEC = weather_bot

# Default target
all: $(EXEC)

# Linking the executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $(EXEC)

# Compilation rules
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(EXEC)

# Rebuild the project
rebuild: clean all
