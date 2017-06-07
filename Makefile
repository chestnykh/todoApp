SRC = src/

.PHONY: clean all
#in this top dir i think it should be saved only main targets: clean and all

all:
	if [ ! -d "bin/build" ]; \
	then \
		mkdir bin/build \
	fi \
	#make -C $(SRC) app

clean:
	make -C $(SRC) clean
