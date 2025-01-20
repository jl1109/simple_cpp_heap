clang-test:
	clang -O3 llist.cpp heap.cpp main.cpp -o memory_test
	./memory_test	

gcc-test:
	gcc -O3 llist.cpp heap.cpp main.cpp -o memory_test
	./memory_test

clean:
	rm memory_test
