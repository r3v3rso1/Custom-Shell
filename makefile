customshell: customshell.c utility.c customshell.h
	gcc -Wall -Wextra -std=gnu99 customshell.c utility.c -o customshell

clean:
	rm -f customshell
