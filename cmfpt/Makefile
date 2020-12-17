CC=gcc
CFLAGS=-O2 -Wall -pedantic

OBJS=adj.c adj_null.c com.c com_null.c adj1.c

all: adj adj_null com com_null

adj: adj_null
	${CC} ${CFLAGS} -o adj adj.c -lm

adj_null: adj_null
	${CC} ${CFLAGS} -o adj_null adj_null.c -lm

com: com
	${CC} ${CFLAGS} -o com com.c -lm

com_null: com_null
	${CC} ${CFLAGS} -o com_null com_null.c -lm
