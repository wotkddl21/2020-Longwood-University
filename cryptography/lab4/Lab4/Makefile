SHA1: sha1.cpp
	g++ sha1.cpp -o SHA1 -g

.phony: test

test: SHA1 testa.txt testb.txt
	./SHA1 < testa.txt
	./SHA1 < testb.txt
	./SHA1 < testc.txt
