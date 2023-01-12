cb:	cb.cpp
	clang++ -pedantic -Wall cb.cpp -o cb

installLinux:	cb
	sudo cp ./cb /bin/cb

installWindows:	cb
	mkdir C:\bin
	cp .\cb C:\bin\cb
	setx PATH "C:\bin;%PATH%"