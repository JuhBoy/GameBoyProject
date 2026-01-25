CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -O2
LDFLAGS = -mconsole
TARGET  = generator.exe

all: $(TARGET)

$(TARGET): lut_generator.c
	$(CC) $(CFLAGS) lut_generator.c -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
