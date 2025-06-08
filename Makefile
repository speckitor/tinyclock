CC=gcc

OUT = tinyclock

$(OUT): main.c
	$(CC) -o $(OUT) main.c
