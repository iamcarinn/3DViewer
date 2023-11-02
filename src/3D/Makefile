CC = gcc
EFLAGS = -std=c11 -Wall -Wextra -Werror 
APP = 3D_V/*

GCOVFLAGS = -L. --coverage
TEST_FLAGS = -lm -lcheck

ifeq ($(shell uname), Linux)
	TEST_FLAGS += -lpthread -lrt -lsubunit -lm
endif

all: install

build:
	rm -rf build
	mkdir build
	cd build && qmake6 ../qvisual/ && make && mv qvisual.app 3D_V.app
	# cd build && qmake6 ../qvisual/ && make

install: build
	mkdir ~/Desktop/3D_V
	cp -rf ./build ~/Desktop/3D_V
	make clean

uninstall:
	rm -rf ~/Desktop/3D_V ~/Desktop/config.txt

dvi:
	open ../documentation/html/index.html

dist:
	rm -rf ../Archive_3D_V/
	mkdir ../Archive_3D_V/
	cp -r ../3D/ ../Archive_3D_V/
	cp -r ../documentation/ ../Archive_3D_V/
	tar -C ../ -czvf Archive_3D_V.tgz Archive_3D_V/
	mv Archive_3D_V.tgz ~/Desktop/
	rm -rf ../Archive_3D_V/

test: clean
	$(CC) $(EFLAGS) test.c parser.c transform.c $(TEST_FLAGS) -o test
	./test

check:
	$(CC) $(EFLAGS) test.c parser.c transform.c $(TEST_FLAGS) -o test
	CK_FORK=no leaks --atExit -- ./test

gcov_report: clean
	$(CC) $(EFLAGS) -coverage test.c parser.c transform.c $(TEST_FLAGS) -o gcovreport
	./gcovreport
	lcov -t "gcovreport" -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info
	open ./report/index.html
	rm -rf *.a *.info *.gcda *.gcno gcovreport

style:
	cp ../../materials/linters/.clang-format .clang-format
	clang-format -n -style=Google *.h *.c qvisual/*.cpp qvisual/*.h
	rm .clang-format

fix-style:
	cp ../../materials/linters/.clang-format .clang-format
	clang-format -i -style=Google *.h *.c qvisual/*.cpp qvisual/*.h
	rm .clang-format

clean:
	rm -rf *.o *.a config.txt report
	rm -rf *.gcda *.gcno *.info
	rm -rf gcovreport
	rm -rf build 
	rm -rf test
