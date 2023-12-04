CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -fsanitize=address -g
SRCDIR = source_files
INCDIR = header_files
OBJDIR = obj

TARGET_MON = bikeSharingMON.o
TARGET_NYC = bikeSharingNYC.o

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))
INCLUDES = -I$(INCDIR)

all: mon nyc

mon: CFLAGS += -DMONTREAL
mon: $(TARGET_MON)

nyc: CFLAGS += -DNEWYORK
nyc: $(TARGET_NYC)

$(TARGET_MON): $(OBJECTS) frontMON.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET_MON) $(OBJECTS) frontMON.c

$(TARGET_NYC): $(OBJECTS) frontNYC.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET_NYC) $(OBJECTS) frontNYC.c

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET_MON) $(TARGET_NYC)

