NAME =

SRC = 

OBJ = 


all:

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all