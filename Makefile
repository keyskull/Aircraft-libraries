DIR=$(shell pwd)

INCS = -I.\
        -I$(DIR)/c_environment \
        -I$(DIR)/c_environment/hardware \
        -I$(DIR)/c_environment/hardware/arduino \
        -I$(DIR)/c_environment/hardware/arduino/cores \
        -I$(DIR)/c_environment/hardware/arduino/cores/arduino \
        -I$(DIR)/c_environment/hardware/arduino/variants \
        -I$(DIR)/c_environment/hardware/arduino/variants/sunxi \
        -I$(DIR)/c_environment/libraries \
        -I$(DIR)/c_environment/libraries/Serial \
        -I$(DIR)/c_environment/libraries/SPI \
        -I$(DIR)/c_environment/libraries/Wire \
        -I$(DIR)/c_environment/libraries/LiquidCrystal \
        -I$(DIR)/c_environment/libraries/PN532_SPIa \
        -I$(DIR)/libraries/PID_v1 \
        -I$(DIR)/libraries/I2Cdev \
        -I$(DIR)/libraries/MPU6050 \
        -I$(DIR)/Includes 

LIBS= $(DIR)/c_environment/libarduino.a
TARGET= $(DIR)/build/test

OBJS += libAircraft

all: 
	 make -C $(DIR)/c_environment
	 mkdir -p $(TARGET)
	 echo "$(CXX) $(INCS) libAircraft.c -fPIC -o $(TARGET)/$$i.so $(LIBS) " 
	 $(CXX) $(INCS) libAircraft.c -fPIC -o $(TARGET)/libAircraft.so $(LIBS) -shared

clean:
	rm -f $(TARGET)/*
