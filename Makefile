make: single avg

single:
	gcc -g -Wall -o single-link single-link.c -lm

avg:
	gcc -g -Wall -o avg-link average-link.c -lm

cspkmeans:
	python3 ./kmeans.py datasets/c2ds1-2sp.txt 5 10
	python3 ./kmeans.py datasets/c2ds1-2sp.txt 6 10
	python3 ./kmeans.py datasets/c2ds1-2sp.txt 7 10
	python3 ./kmeans.py datasets/c2ds1-2sp.txt 8 10
	python3 ./kmeans.py datasets/c2ds1-2sp.txt 9 10
	python3 ./kmeans.py datasets/c2ds1-2sp.txt 10 10
	python3 ./kmeans.py datasets/c2ds1-2sp.txt 11 10
	python3 ./kmeans.py datasets/c2ds1-2sp.txt 12 10

cgkmeans:
	python3 ./kmeans.py datasets/c2ds3-2g.txt 5 10
	python3 ./kmeans.py datasets/c2ds3-2g.txt 6 10
	python3 ./kmeans.py datasets/c2ds3-2g.txt 7 10
	python3 ./kmeans.py datasets/c2ds3-2g.txt 8 10
	python3 ./kmeans.py datasets/c2ds3-2g.txt 9 10
	python3 ./kmeans.py datasets/c2ds3-2g.txt 10 10
	python3 ./kmeans.py datasets/c2ds3-2g.txt 11 10
	python3 ./kmeans.py datasets/c2ds3-2g.txt 12 10

monkeykmeans:
	python3 ./kmeans.py datasets/monkey.txt 5 10
	python3 ./kmeans.py datasets/monkey.txt 6 10
	python3 ./kmeans.py datasets/monkey.txt 7 10
	python3 ./kmeans.py datasets/monkey.txt 8 10
	python3 ./kmeans.py datasets/monkey.txt 9 10
	python3 ./kmeans.py datasets/monkey.txt 10 10
	python3 ./kmeans.py datasets/monkey.txt 11 10
	python3 ./kmeans.py datasets/monkey.txt 12 10

cspsingle: single
	./single-link datasets/c2ds1-2sp.txt 2 5

cgsingle: single
	./single-link datasets/c2ds3-2g.txt 2 5

monkeysingle: single
	./single-link datasets/monkey.txt 5 12

cspavg: avg
	./avg-link datasets/c2ds1-2sp.txt 2 5

cgavg: avg
	./avg-link datasets/c2ds3-2g.txt 2 5

monkeyavg: avg
	./avg-link datasets/monkey.txt 5 12
