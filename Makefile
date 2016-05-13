SRC := $(wildcard *.c)
OBJ := $(patsubst %.c,%.c.o,$(SRC))


test: $(OBJ)
	gcc -o $@ $^ -lrt
	
%.c.o: %.c
	gcc -c -Wall -g -o $@ $<

clean:
	rm -f $(OBJ)

.PHONY: clean 


