# options de compilation
CC = gcc
CCFLAGS = -Wall 
LIBS =
IDIR =
LIBSDIR = 

# fichiers du projet
SRC = bank.c main.c 
OBJ = $(SRC:.c=.o)
EXEC = bank.out

# règle initiale
all: $(EXEC)

# règles de compilation
main.o : bank.h
bank.o : bank.h
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $< $(IDIR)

# règles d'édition de liens
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(LIBSDIR) -g

# règles supplémentaires
clean:
	rm -f *.o
	ls $(OBJ)
mproper:
	rm -f $(EXEC) *.o