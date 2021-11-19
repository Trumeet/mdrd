.POSIX:
CFLAGS += -I.
CFLAGS += -I/usr/local/include
CFLAGS += -std=c99
CFLAGS += -Wall
LDFLAGS += -L/usr/local/lib
LDLIBS += -lcmark
LDLIBS += -lfcgi
OBJ = main.o
BIN = mdrd
PREFIX = /usr/local

debug:
	make all LDFLAGS="-fsanitize=address $(LDFLAGS)" CFLAGS+="-fsanitize=address -DDEBUG -O0 -g3 -rdynamic $(CFLAGS)"

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJ) -o $(BIN)

clean:
	rm -rf $(BIN) $(OBJ)

install: $(BIN)
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 $(BIN) $(DESTDIR)$(PREFIX)/bin/

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<
