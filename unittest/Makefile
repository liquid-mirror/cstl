CFLAGS = -Wall -ansi -pedantic-errors -g
PROG = libunittest.a
OBJS = UnitTest.o\
	   TestCase.o\
	   TestSuite.o\
	   LibcImpl.o\
	   $(NULL)

all: $(PROG)

.SUFFIXES: .c .o

.c .o:
	$(CC) $(CFLAGS) -c $<


$(PROG): $(OBJS)
	$(AR) rcs $(PROG) $^


.PHONY: clean
clean:
	rm -f *.o *.a

UnitTest.o: TestSuite.h LibcImpl.h
TestCase.h: UnitTest.h
TestCase.o: TestCase.h LibcImpl.h
TestSuite.h: UnitTest.h
TestSuite.o: TestCase.h TestSuite.h LibcImpl.h
LibcImpl.o: LibcImpl.h UnitTest.h
