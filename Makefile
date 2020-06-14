CC = gcc
#CC = c99
#CC = clang

STD = -std=gnu17
#STD = -std=gnu11

#WAR = -Wall -Wextra -pedantic

TicTacToe: TicTacToe.c
	${CC} TicTacToe.c -o TicTacToe ${STD} ${WAR}

clean :
	rm TicTacToe
