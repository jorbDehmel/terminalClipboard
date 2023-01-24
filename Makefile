UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
target = installLinux
else
target = installWindows
endif

install: $(target)

installLinux:
	echo Installing for Linux...
	mkdir -p build
	cd build && cmake .. && make && cd ..
	sudo cp build/cb /bin/cb

installWindows:
	echo Installing for Windows...
	mkdir -p build
	cd build && cmake .. && make && cd ..
	mkdir C:\bin
	cp build\cb C:\bin\cb
	setx PATH "C:\bin;%PATH%"