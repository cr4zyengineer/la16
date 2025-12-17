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
	./$(OUT) -c asm/test/kernel.l16 asm/test/user.l16 asm/test/libuser.l16
	./$(OUT) -r a.out

clean:
	-rm $(OUT)
