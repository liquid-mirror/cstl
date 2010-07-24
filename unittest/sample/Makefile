CFLAGS = -Wall -ansi -pedantic-errors -g
PROG = sample
OBJS = main.o\
	   test_1.o\
	   test_2.o\
	   ../libunittest.a\
	   $(NULL)
UTOBJS = ../UnitTest.o\
		 ../TestCase.o\
		 ../TestSuite.o\
		 ../LibcImpl.o\
		 $(NULL)

all: $(PROG)

.SUFFIXES: .c .o

.c .o:
	$(CC) $(CFLAGS) -c $<


$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@


../libunittest.a: $(UTOBJS)
	cd .. && $(MAKE)


.PHONY: clean
clean:
	rm -f *.o $(PROG) ../*.o ../*.a

main.o: ../UnitTest.h test.h
test.h: ../UnitTest.h
test_1.o: test.h
test_2.o: test.h

../UnitTest.o: ../TestSuite.h ../LibcImpl.h
../TestCase.h: ../UnitTest.h
../TestCase.o: ../TestCase.h ../LibcImpl.h
../TestSuite.h: ../UnitTest.h
../TestSuite.o: ../TestCase.h ../TestSuite.h ../LibcImpl.h
../LibcImpl.o: ../LibcImpl.h ../UnitTest.h
