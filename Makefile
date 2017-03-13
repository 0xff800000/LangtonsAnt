CC = g++
CFLAGS = -W -Wall
LDFLAGS = -lSDL2main -lSDL2 -lstdc++
EXEC = langton

all: $(EXEC)

$(EXEC): $(EXEC).cpp
	$(CC) $^ $(CFLAGS) -o $(EXEC) $(LDFLAGS)