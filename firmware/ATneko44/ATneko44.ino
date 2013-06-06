/***************************************
        *** ATneko Firmware ***
            
  Software Version:     0.0.0
  Supported Hardware:   ATneko44

***************************************/

// Pin Map
#define PIN_SX1 PIN_A4
#define PIN_SX2 PIN_A7
#define PIN_DX1 PIN_A5
#define PIN_DX2 PIN_A6
#define PIN_ACCEL_X PIN_A1
#define PIN_ACCEL_Y PIN_A2
#define PIN_ACCEL_Z PIN_A3 // Unused
#define PIN_CALIBRATION PIN_B2

// SAD Trigger
#define SAD_MAX_Y -20

// ALERT Trigger
#define ALERT_MIN_X -20
#define ALERT_MAX_X 20
#define ALERT_MIN_Y 10

// AWW Trigger
#define AWW_MIN_X 20
#define AWW_MIN_Y -20

// WINK Trigger
#define WINK_MAX_X -20
#define WINK_MIN_Y -20

// Animations
#define DEBUG 0
#define BOOT 1
#define NORMAL 2
#define SAD 3
#define ALERT 4
#define AWW 5
#define WINK 6

int pos(int servo1, int pos1, int servo2=-1, int pos2=0, int servo3=-1, int pos3=0, int servo4=-1, int pos4=0) {
  int i;
  
  pos1=map(pos1, 0, 179, 500, 2500);
  pos2=map(pos2, 0, 179, 500, 2500);
  pos3=map(pos3, 0, 179, 500, 2500);
  pos4=map(pos4, 0, 179, 500, 2500);

  for(i=0; i<25; i++) {
    digitalWrite(servo1, HIGH);
    delayMicroseconds(pos1);
    digitalWrite(servo1, LOW);
    if(servo2 != -1) {
      digitalWrite(servo2, HIGH);
      delayMicroseconds(pos2);
      digitalWrite(servo2, LOW);
    }
    if(servo3 != -1) {
      digitalWrite(servo3, HIGH);
      delayMicroseconds(pos3);
      digitalWrite(servo3, LOW);
    }
    if(servo4 != -1) {
      digitalWrite(servo4, HIGH);
      delayMicroseconds(pos4);
      digitalWrite(servo4, LOW);
    }
    delay(20);
  }
}

// global vars
int i;
int x, y, z;
int buffer_x[10], buffer_y[10], buffer_z[10];
int max, min;
int neutral_x, neutral_y, neutral_z;
int animation;
int debug;
int boot;

void setup() {
  
  analogReference(EXTERNAL);
  pinMode(PIN_SX1, OUTPUT);
  pinMode(PIN_SX2, OUTPUT);
  pinMode(PIN_DX1, OUTPUT);
  pinMode(PIN_DX2, OUTPUT);
  
  pinMode(PIN_ACCEL_X, INPUT);
  pinMode(PIN_ACCEL_Y, INPUT);
  pinMode(PIN_ACCEL_Z, INPUT); // unused
  
  pinMode(PIN_CALIBRATION, INPUT);
  digitalWrite(PIN_CALIBRATION, HIGH);

  i=0;
  boot=1;
  debug=0;

  delay(500);

  if(digitalRead(PIN_CALIBRATION)==0) {
    // DEBUG MODE
    debug=1;
  }
  else {
    // STANDARD MODE
    attachInterrupt(0, calibration, FALLING);
  }
} 

void loop() {
  
  if(debug == 1) {
    // DEBUG MODE
    if(boot==1) {
      boot=0;
      animation=DEBUG;
      i=NORMAL;
    }
    else {
      animation=i;
      i++;
      if(i>WINK) i=NORMAL;    
    }
  }
  else {
    // STANDARD MODE
  
    // Accelerometer Read
    for(i=0; i<10; i++) {
      buffer_x[i]=analogRead(PIN_ACCEL_X);
      buffer_y[i]=analogRead(PIN_ACCEL_Y);
      buffer_z[i]=analogRead(PIN_ACCEL_Z);
      delay(20);
    }
    
    // Accelerometer data normalization
    x=0;
    max=buffer_x[0];
    min=buffer_x[0];
    for(i=0; i<10; i++) {
      x+=buffer_x[i];
      if(buffer_x[i] > max) max=buffer_x[i];
      if(buffer_x[i] < min) min=buffer_x[i];
    }
    x=(x-max-min)/8;
    y=0;
    max=buffer_y[0];
    min=buffer_y[0];
    for(i=0; i<10; i++) {
      y+=buffer_y[i];
      if(buffer_y[i] > max) max=buffer_y[i];
      if(buffer_y[i] < min) min=buffer_y[i];
    }
    y=(y-max-min)/8;
    z=0;
    max=buffer_z[0];
    min=buffer_z[0];
    for(i=0; i<10; i++) {
      z+=buffer_z[i];
      if(buffer_z[i] > max) max=buffer_z[i];
      if(buffer_z[i] < min) min=buffer_z[i];
    }
    z=(z-max-min)/8;
    
    // Position detection
    if(boot==1) animation=BOOT;
    else if(y < neutral_y+SAD_MAX_Y) animation=NORMAL;
    else if(x > neutral_x+ALERT_MIN_X && x < neutral_x+ALERT_MAX_X && y > neutral_y+ALERT_MIN_Y) animation=ALERT;
    else if(x > neutral_x+AWW_MIN_X && y > neutral_y+AWW_MIN_Y) animation=AWW;
    else if(x > neutral_x+WINK_MAX_X && y > neutral_y+WINK_MIN_Y) animation=WINK;
  }
  
  // Run animation
  if(animation==DEBUG) {
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
  else if(animation==BOOT) {
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
  else if(animation==NORMAL) {
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
  else if(animation==SAD) {
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
  else if(animation==ALERT) {
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
  else if(animation==AWW) {
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
  else if(animation==WINK) {
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
}

void calibration() {
  // ZERO point calibration
  for(i=0; i<10; i++) {
    buffer_x[i]=analogRead(PIN_ACCEL_X);
    buffer_y[i]=analogRead(PIN_ACCEL_Y);
    buffer_z[i]=analogRead(PIN_ACCEL_Z);
    delay(20);
  }
    
  // Accelerometer data normalization
  neutral_x=0;
  max=buffer_x[0];
  min=buffer_x[0];
  for(i=0; i<10; i++) {
    neutral_x+=buffer_x[i];
    if(buffer_x[i] > max) max=buffer_x[i];
    if(buffer_x[i] < min) min=buffer_x[i];
  }
  neutral_x=(neutral_x-max-min)/8;
  neutral_y=0;
  max=buffer_y[0];
  min=buffer_y[0];
  for(i=0; i<10; i++) {
    neutral_y+=buffer_y[i];
    if(buffer_y[i] > max) max=buffer_y[i];
    if(buffer_y[i] < min) min=buffer_y[i];
  }
  neutral_y=(neutral_y-max-min)/8;
  neutral_z=0;
  max=buffer_z[0];
  min=buffer_z[0];
  for(i=0; i<10; i++) {
    neutral_z+=buffer_z[i];
    if(buffer_z[i] > max) max=buffer_z[i];
    if(buffer_z[i] < min) min=buffer_z[i];
  }
  neutral_z=(neutral_z-max-min)/8;
}
