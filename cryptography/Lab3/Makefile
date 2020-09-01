all: Encrypt Decrypt

Encrypt: spn-encrypt.cpp
		g++ -g -o Encrypt spn-encrypt.cpp

Decrypt: spn-decrypt.cpp
		g++ -g -o Decrypt spn-decrypt.cpp
.PHONY: test test1 test2 test3 clean

test : test0 test1 test2 test3
test0:
		echo "micky" | ./Encrypt 2 2 4 6 | ./Decrypt 2 2 4 6 > test0.txt
		cat test0.txt
test1:
		echo "hello" | ./Encrypt 2 1 9 2 | ./Decrypt 2 1 9 2 > test1.txt
		cat test1.txt
test2:
		echo "jack" | ./Encrypt 3 1 9 2 123 | ./Decrypt 3 1 9 2 123 > test2.txt
		cat test2.txt
test3:
		echo "jack" | ./Encrypt 3 191191 1 9 2 | ./Decrypt 3 101101 1 9 2 > test3.txt
		cat test3.txt
clean:
		rm Encrypt Decrypt test*.txt
