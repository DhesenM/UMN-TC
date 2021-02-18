CC = gcc -Wall -g

PROGRAMS = \
	fp_basics \
	qsort_demo \
	sort_float \
	sort_doubles \

all : $(PROGRAMS)

%.o : %.c
	$(CC) -c $<

fp_basics : fp_basics.o
	$(CC) -o $@ $^

qsort_demo : qsort_demo.o
	$(CC) -o $@ $^

sort_doubles : sort_doubles.o
	$(CC) -o $@ $^

sort_float : sort_float.o
	$(CC) -o $@ $^

clean :
	rm -f *.o $(PROGRAMS)
