CC=gcc

OUT = ctime

$(OUT): main.c
	$(CC) -o $(OUT) main.c
