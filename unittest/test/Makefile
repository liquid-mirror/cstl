CFLAGS = -DNO_STD_PRINTF -Wall -ansi -pedantic-errors -g
PROG = unittest_test
OBJS = test_LibcImpl.o\
		 ../LibcImpl.o\
		 $(NULL)

all: $(PROG)

.SUFFIXES: .c .o

.c .o:
	$(CC) $(CFLAGS) -c $<


$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@



.PHONY: clean
clean:
	rm -f *.o $(PROG) ../*.o


../LibcImpl.o: ../LibcImpl.h ../UnitTest.h

