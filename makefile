compile: write.c control.c
	gcc control.c -o control
	gcc write.c -o write
clean:
	rm *~
