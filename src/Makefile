CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -g -pedantic -D_GNU_SOURCE
PROFILE_FLAGS = -fprofile-arcs -ftest-coverage
OS = $(shell uname)

ifeq ($(OS), Linux)
#-lsubunit
	CHECKFLAGS=-lpthread -lcheck -pthread -lrt -lm
else
	CHECKFLAGS=-lcheck -lm -lpthread
endif

all: clean install dist dvi gcov_report check_style leaks clean

rebuild: clean all

install: uninstall
	mkdir build
	cp -rf 3dViewer_v1-0.pro build
	cp -rf c build
	cp -rf cpp build
	cp -rf qtGifImage build
	cp -rf ui build
	cp -rf resource_files build
	cd build && qmake 3dViewer_v1-0.pro
	make -C build
	cd build && make clean
	open ./build/3dViewer_v1-0.app/Contents/MacOS/3dViewer_v1-0

uninstall:
	rm -rf build
	make clean

dvi:
	open ./dvi_readme/readme.txt

dist:
	tar -cvf build/3dViewer1-0.tar build
	gzip build/3dViewer1-0.tar
	mv build/3dViewer1-0.tar.gz ./

test:
	$(CC) $(CFLAGS) $(CHECKFLAGS) c/*.c -o test.out -lgcov --coverage
	./test.out

gcov_report: test
	lcov -t "test" -o report.info -c -d . 
	genhtml -o gcov_report/ report.info
	rm -f *.gcno *.gcda *.info report.out *.gcov
	open ./gcov_report/index.html

style:
	cp ../materials/linters/.clang-format ./
	clang-format -i ./c/*.c
	clang-format -i ./c/*.h
	clang-format -i ./cpp/*.cpp
	clang-format -i ./cpp/*.h
	rm -rf .clang-format

check_style:
	cp ../materials/linters/.clang-format ./
	clang-format -n c/*.c >> logfile.log
	clang-format -n c/*.h >> logfile.log
	clang-format -n cpp/*.cpp >> logfile.log
	clang-format -n cpp/*.h >> logfile.log
	rm -rf .clang-format

# запуск через sh run.sh с запущенным докером
valgrind:
	$(CC) $(CFLAGS) c/*.c -o tests.out $(CHECKFLAGS)
	 CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./tests.out
	 make clean

leaks: test
	CK_FORK=no leaks --atExit -- ./tests.out >> logfile.log
	make clean

clean:
	rm -rf *.o \
		*.a \
		*.app \
		*.so \
		*.gcno \
		*gcda \
		*.gcov \
		gcov_report.* \
		*.dSYM \
        RESULT_VALGRIND.txt \
		*.out \
        unit-test \
        vtests_run \
		run \
		gcov \
		report/ \
		gcov_report/ \
		report.info \
		gcov_report \
		gcov_test \
