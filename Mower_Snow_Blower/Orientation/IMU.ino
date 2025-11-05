unsigned long IMU_lasttime = millis();
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
//String IMU_str;
uint8_t fifoBuffer[64]; // FIFO storage buffer


// ======================================================
void IMU_begin() {
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO

    Serial.println("Initializing I2C devices...");
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    delay(2000);
    // wait for ready
    //Serial.println("\nSend any character to begin DMP programming and demo: ");
    //while (Serial.available() && Serial.read()); // empty buffer
    //while (!Serial.available());                 // wait for data
    //while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    Serial.println("Initializing DMP...");
    devStatus = mpu.dmpInitialize();
    

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    while (devStatus != 0) {
       // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print("DMP Initialization failed (code ");
        Serial.print(devStatus);
        Serial.println(")");
        Serial.println("Initializing DMP...");

        delay(1000);
        devStatus = mpu.dmpInitialize();
        // supply your own gyro offsets here, scaled for min sensitivity
        mpu.setXGyroOffset(220);
        mpu.setYGyroOffset(76);
        mpu.setZGyroOffset(-85);
        mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    }
    if (devStatus == 0) {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();
        // turn on the DMP, now that it's ready
        Serial.println("Enabling DMP...");
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.print("Enabling interrupt detection (Arduino external interrupt ");
        Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
        Serial.println(")...");
       // attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println("DMP ready! Waiting for first interrupt...");
        IMU_Ready = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print("DMP Initialization failed (code ");
        Serial.print(devStatus);
        Serial.println(")");
    }
}

// ===================== IMU_Yaw ============================
void IMU_Yaw() {
  //static unsigned long lasttime;
    // read a packet from FIFO
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer) && (millis()-IMU_lasttime) > INTERVAL_IMU) { // Get the Latest packet 

        IMU_lasttime = millis();
        
        // display Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        Yaw = int(ypr[0]* 180/M_PI);
        //MQTT_publish();
        //IMU_pingpong = !IMU_pingpong;
        //IMU_str ="{\"Yaw\":" + String(YAW) + ",\"IMUping\":" + String(IMU_lasttime) + "}";
        //Serial.println(Yaw);
  }
  //return -1000;
}
