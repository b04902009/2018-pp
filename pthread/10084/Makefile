main:
	gcc -std=c99 -O2 -pthread 10084.c secret.c -o 10084
	gcc -std=c99 -O2 -pthread prefixsum-seq.c secret.c -o seq
clean:
	rm -rf 10084
run:
	./10084