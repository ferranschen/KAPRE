
KAPRE: 
	gcc -o KAPRE kapre.c -I /usr/local/include/pbc -L /usr/local/lib -Wl,-rpath /usr/local/lib  -lpbc -lgmp	


clean:
	rm -rf KAPRE *.o *.a
