CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -fsanitize=address
SRCDIR = source_files
INCDIR = header_files
OBJDIR = obj

TARGET_MON = bikeSharingMON.o
TARGET_NYC = bikeSharingNYC.o

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
INCLUDES = -I$(INCDIR)

all: MON NYC

MON: $(TARGET_MON)

NYC: $(TARGET_NYC)

$(TARGET_MON): $(OBJECTS) bikeSharingMON.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET_MON) $(OBJECTS) bikeSharingMON.c

$(TARGET_NYC): $(OBJECTS) bikeSharingNYC.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET_NYC) $(OBJECTS) bikeSharingNYC.c

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET_MON) $(TARGET_NYC) *.html query*.csv
