TARGET = cross.bin

SRCDIR = srcs
INCLUDEDIR = $(SRCDIR)/include

SOURCE = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(INCLUDEDIR)/*.h)

CC = g++
CFLAGS = -g

all: $(TARGET)

$(TARGET): $(SOURCE) $(HEADERS)
	$(CC) -o $(TARGET) $(SOURCE) -I$(INCLUDEDIR)

debug: $(SOURCE) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) -I$(INCLUDEDIR)

clean:
	rm *.bin

# Regra para rodar o programa (opcional)
run: $(TARGET)
	./$(TARGET)
	