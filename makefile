
#compilateur
CC		= g++
#Flags
CFLAGS	= -Wall -O2 -g
#Includes
LDFLAGS	= -lSDL2 -lSDL2_image -lGLU -lGL -lm

BINDIR	= bin/
SRCDIR	= src/
OBJDIR	= obj/

# main
MAIN= main.o
EXEC_MAIN= main.out


# Regles compilation 

all :


main : $(OBJDIR)$(MAIN)
	$(CC) $(CFLAGS) $(OBJDIR)$(MAIN) -o $(BINDIR)$(EXEC_MAIN) $(LDFLAGS)


clean :
	rm -rf *~
	rm -rf $(SRCDIR)*/*~
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)*

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
