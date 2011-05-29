NAME = vecx
PATH := ${ALCHEMY_HOME}/achacks:${PATH}
CC = gcc
CFLAGS = -O2 -Wall -Wextra -swc
OBJS = e6809.o osint.o vecx.o

.c.o:
	$(CC) $(INC) $(CFLAGS) -c $<

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) -swc -o $(NAME).swc $(OBJS)

clean:
	rm $(OBJS) $(NAME).swc 
	rm *achacks*
