#  Makefile

CC := clang
CFLAGS := -I ./src
CFILES := $(shell find src -name "*.c")
OUT := la16

all: compile execute clean

compile:
	$(CC) $(CFLAGS) $(CFILES) -o $(OUT)

execute:
	chmod +x $(OUT)
	./$(OUT) -c asm/test/kernel.s asm/test/user.s asm/test/libuser.s
	./$(OUT) -r a.out

clean:
	-rm $(OUT)
