cb:	cb.cpp
	clang++ -pedantic -Wall cb.cpp -o cb

install:	cb
	sudo cp ./cb /bin/cb