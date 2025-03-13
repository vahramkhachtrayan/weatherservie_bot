# Compiler and linker settings
CC = gcc
CFLAGS = -Wall -std=c99 -I./telebot/include -I./rust_weather/include
LDFLAGS = -L./telebot/lib -L$(HOME)/workplace/WeatherService/rust_weather/target/release
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

