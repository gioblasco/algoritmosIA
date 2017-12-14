make: single avg

single:
	gcc -g -Wall -o single-link single-link.c -lm
avg:
	gcc -g -Wall -o avg-link average-link.c -lm
diff:
	diff datasets/pequeno1.txt datasets/pequeno1.clu
	diff datasets/pequeno2.txt datasets/pequeno2.clu
	diff datasets/pequeno3.txt datasets/pequeno3.clu
	diff datasets/pequeno4.txt datasets/pequeno4.clu
	diff datasets/pequeno5.txt datasets/pequeno5.clu
	diff datasets/pequeno6.txt datasets/pequeno6.clu
pequeno:
	./avg-link datasets/pequeno.txt 1 6
	 $(MAKE) diff

c1:
	./avg-link datasets/c2ds1-2sp.txt 2 5

c2:
	./avg-link datasets/

monkey:
	./avg-link datasets/monkey.txt 5 12