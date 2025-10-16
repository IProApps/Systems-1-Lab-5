lab5: main.c quicksort.c 
	gcc -O3 quicksort.c main.c -o lab5-gcc
	clang -O3 -DUSE_CMOV quicksort.c main.c -o lab5-clang
