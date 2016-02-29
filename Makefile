DIR=$(shell pwd)/c_environment

INCS = -I.\
        -I$(DIR) \
        -I$(DIR)/hardware \
        -I$(DIR)/hardware/arduino \
        -I$(DIR)/hardware/arduino/cores \
        -I$(DIR)/hardware/arduino/cores/arduino \
        -I$(DIR)/hardware/arduino/variants \
        -I$(DIR)/hardware/arduino/variants/sunxi \
        -I$(DIR)/libraries \
        -I$(DIR)/libraries/Serial \
        -I$(DIR)/libraries/SPI \
	-I$(DIR)/libraries/Wire \
	-I$(DIR)/libraries/LiquidCrystal \
	-I$(DIR)/libraries/PN532_SPIa \
	-I$(DIR)/libraries/PID_v1 \
	-I$(DIR)/libraries/I2Cdev \
	-I$(DIR)/libraries/MPU6050

LIBS= $(DIR)/libarduino.a
TARGET= $(DIR)/../build/test

OBJS += Aircraft

all: 
    make -C $(DIR)
	@mkdir -p $(TARGET)
	@for i in $(OBJS); do echo "$(CXX) $(INCS) $$i.c -fPIC -o $(TARGET)/$$i.so $(LIBS) " && $(CXX) $(INCS) $$i.c -fPIC -o $(TARGET)/$$i.so $(LIBS) -shared; done

clean:
	@for i in $(OBJS); do rm -f $(TARGET)/$$i; done
