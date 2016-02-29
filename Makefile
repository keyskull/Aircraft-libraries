#
# Assume standlone toolchain
#
#CC  = arm-linux-gnueabihf-gcc
#CXX = arm-linux-gnueabihf-g++
#AR  = arm-linux-gnueabihf-ar

LIB_STATIC = libarduino.a
LIB_SHARE = libarduino.so
LIB = $(LIB_STATIC) $(LIB_SHARE)

all: $(LIB)
	make -C c_environment/



clean:
	rm -f $(LIB_STATIC) $(LIB_SHARE) $(OBJS)


