CC = g++
CFLAGS = -g -Wall -pedantic -Wextra -Werror -std=c++17

TARGET = scream

all: $(TARGET)

rc: $(TARGET) run clean

rl: $(TARGET) launch clean

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	@rm -rf $(TARGET) $(TARGET).dSYM .DS_Store .mypy_cache

run:
	./$(TARGET)

launch:
	./launch.py $(TARGET)
