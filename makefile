SRCFILES := $(wildcard src/*.c src/*/*.c)

all:
	gcc $(SRCFILES) -lpsapi -o./bin/dbg/B2ETcli

release:
	gcc -static -static-libgcc -static-libstdc++ $(SRCFILES) -o./bin/rel/B2ETcli