/*
 * libAircraft.c
 *
 *  Created on: 2016年2月29日
 *      Author: key_q
 */
#include <MPU6050_6Axis_MotionApps20.h>
MPU6050 mpu;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

void setup(){
	Wire.begin();
	// 初始化 I2C 設備
	printf("Initializing I2C devices...\n");
	mpu.initialize();
	// 測試一下連線是否正常
	printf("Testing device connections...\n");
	printf(mpu.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");
	// 載入與設定 DMP
	printf("Initializing DMP...\n");
	devStatus = mpu.dmpInitialize();
    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
    if (devStatus == 0) {
            // turn on the DMP, now that it's ready
            printf(F("Enabling DMP...\n"));
            mpu.setDMPEnabled(true);

            // enable Arduino interrupt detection
            printf(F("Enabling interrupt detection (Arduino external interrupt 0)...\n"));
            attachInterrupt(0, dmpDataReady, RISING);
            mpuIntStatus = mpu.getIntStatus();

            // set our DMP Ready flag so the main loop() function knows it's okay to use it
            printf(F("DMP ready! Waiting for first interrupt...\n"));
            dmpReady = true;

            // get expected DMP packet size for later comparison
            packetSize = mpu.dmpGetFIFOPacketSize();
        } else {
            // ERROR!
            // 1 = initial memory load failed
            // 2 = DMP configuration updates failed
            // (if it's going to break, usually the code will be 1)
            printf(F("DMP Initialization failed (code "));
            printf("%d",devStatus);
            printf(F(")\n"));
}

    void loop() {
        // if programming failed, don't try to do anything
        if (!dmpReady) return;

        // reset interrupt flag and get INT_STATUS byte
        mpuInterrupt = false;
        mpuIntStatus = mpu.getIntStatus();

        // get current FIFO count
        fifoCount = mpu.getFIFOCount();

        // check for overflow (this should never happen unless our code is too inefficient)
        if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
            // reset so we can continue cleanly
            mpu.resetFIFO();
            printf(F("FIFO overflow!\n"));

        // otherwise, check for DMP data ready interrupt (this should happen frequently)
        } else if (mpuIntStatus & 0x02) {
            // wait for correct available data length, should be a VERY short wait
            while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

            // read a packet from FIFO
            mpu.getFIFOBytes(fifoBuffer, packetSize);

            // track FIFO count here in case there is > 1 packet available
            // (this lets us immediately read more without waiting for an interrupt)
            fifoCount -= packetSize;

                // display quaternion values in easy matrix form: w x y z
                mpu.dmpGetQuaternion(&q, fifoBuffer);
                /*
                printf("quat\t");
                printf("%f",q.w);
                printf("\t");
                printf("%f",q.x);
                printf("\t");
                printf("%f",q.y);
                printf("\t");
                printf("%f\n",q.z);
*/
                // display Euler angles in degrees
                mpu.dmpGetQuaternion(&q, fifoBuffer);
                mpu.dmpGetEuler(euler, &q);
                /*
                printf("euler\t");
                printf("%f",euler[0] * 180/M_PI);
                printf("\t");
                printf("%f",euler[1] * 180/M_PI);
                printf("\t");
                printf("%f \n",euler[2] * 180/M_PI);
*/
                // display Euler angles in degrees
                mpu.dmpGetQuaternion(&q, fifoBuffer);
                mpu.dmpGetGravity(&gravity, &q);
                mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
                /*
                printf("ypr\t");
                printf("%f",ypr[0] * 180/M_PI);
                printf("\t");
                printf("%f",ypr[1] * 180/M_PI);
                printf("\t");
                printf("%f \n",ypr[2] * 180/M_PI);
*/
                // display real acceleration, adjusted to remove gravity
                mpu.dmpGetQuaternion(&q, fifoBuffer);
                mpu.dmpGetAccel(&aa, fifoBuffer);
                mpu.dmpGetGravity(&gravity, &q);
                mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
                /*
                printf("areal\t");
                printf("%f",aaReal.x);
                printf("\t");
                printf("%f",aaReal.y);
                printf("\t");
                printf("%f \n",aaReal.z);
*/
                // display initial world-frame acceleration, adjusted to remove gravity
                // and rotated based on known orientation from quaternion
                mpu.dmpGetQuaternion(&q, fifoBuffer);
                mpu.dmpGetAccel(&aa, fifoBuffer);
                mpu.dmpGetGravity(&gravity, &q);
                mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
                mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
                /*
                printf("aworld\t");
                printf("%f",aaWorld.x);
                printf("\t");
                printf("%f",aaWorld.y);
                printf("\t");
                printf("%f \n",aaWorld.z);
*/
                // display quaternion values in InvenSense Teapot demo format:
                teapotPacket[2] = fifoBuffer[0];
                teapotPacket[3] = fifoBuffer[1];
                teapotPacket[4] = fifoBuffer[4];
                teapotPacket[5] = fifoBuffer[5];
                teapotPacket[6] = fifoBuffer[8];
                teapotPacket[7] = fifoBuffer[9];
                teapotPacket[8] = fifoBuffer[12];
                teapotPacket[9] = fifoBuffer[13];
                teapotPacket[11]++; // packetCount, loops at 0xFF on purpose

        }
    }

