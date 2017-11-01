all:
	gcc -g -o natimplem natimplem.c
clean:
	rm -rf *.bin
	rm natimplem
