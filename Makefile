CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g $(shell pkg-config --cflags gtk+-3.0)
LIBS = $(shell pkg-config --libs gtk+-3.0)
TARGET = cloud_simulator
OBJS = src/main.o src/scheduler.o src/banker.o src/storage.o src/job.o src/gui.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)

run: all
	./$(TARGET)
