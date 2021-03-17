# Automated Makefile

CC = g++
CFLAGS = -Wall -Werror -D_GLIBCXX_DEBUG -std=c++11 -g
INCLUDE = -I /home/pcchu/Documents/BOOKS/CPP_financial_engineering/test_eigen/eigen-3.3.7
LIBS     := -L /usr/lib/x86_64-linux-gnu -lboost_date_time
COMPILE = $(CC) $(CFLAGS) $(INCLUDE) -c
OBJFILES := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
PROG_NAME = KFilter

all: myprog

myprog: $(OBJFILES)
	$(CC) -o $(PROG_NAME) $(OBJFILES) $(LIBS)

%.o: %.cpp
	$(COMPILE) -o $@ $<
	
clean:
	rm -f *.o *.html $(PROG_NAME)
