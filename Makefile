default: build

build:
	$(CC) -std=gnu99 rf.c -o rf

install: build
	cp rf /usr/bin/rf
