# Compiler and linker settings
CC      := gcc
CFLAGS  := -Wall -Wextra -std=c99 \
           -D_POSIX_C_SOURCE=200809L \
           -O2 -Iinclude -Iutils
LDFLAGS := -lpthread

# Source files: kernel, port, and main
KERNEL_SRCS = \
    list.c queue.c tasks.c timers.c event_groups.c \
    stream_buffer.c

PORT_SRCS   = \
    port.c heap_3.c utils/wait_for_event.c

USER_SRCS   = main.c

.PHONY: all clean

all: freertos_app

# Build the FreeRTOS demo executable
freertos_app: $(KERNEL_SRCS) $(PORT_SRCS) $(USER_SRCS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f freertos_app

