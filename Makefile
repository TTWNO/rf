default: build

build:
	$(CC) -std=c89 rf.c -o rf

install: build
	cp rf /usr/bin/rf
