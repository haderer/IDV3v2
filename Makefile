ARGS = -Wall -ansi -pedantic -Werror -g

COMPIL = gcc -c $^ -o $@ $(ARGS)
EXEC   = gcc -o $@ $^
ALL    = gcc $^ -o $@ $(ARGS)


test : tag.o tag_test.o
	$(EXEC)
	./test

tag.o : tag.c
	$(COMPIL)


tag_test.o : src_test/tag_test.c
	$(COMPIL)

###### supprime tous les fichiers générés lors de la compilation
#--- command 'make clean'
clean : 
		$(shell rm *.o)
		$(shell rm test)
		
		