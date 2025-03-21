# Mykhailo Puhach, 23329993, csc1021, customshell stage 2

customshell: customshell.c utility.c customshell.h
	gcc -Wall -Wextra -std=gnu99 customshell.c utility.c -o customshell

clean:
	rm -f customshell
