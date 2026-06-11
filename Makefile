# Compiler and flags
CC = gcc
CFLAGS = -g
# DEBUG  CFLAGS = -Wall -Wextra -g -Wunused-but-set-variable

# Target executable name
TARGET = cercamail

# Source files
SRCS = cercamail.c functions.c

# Object files (automatically derived)
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compilation step (pattern rule)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)
