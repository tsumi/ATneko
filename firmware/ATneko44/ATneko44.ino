/***************************************
        *** ATneko Firmware ***
            
  Software Version:    0.0.0
  Supported Hardware:  ATneko44 @ 8MHZ

***************************************/

// Pin Map
#define PIN_SX1 PIN_A5
#define PIN_SX2 PIN_A4
#define PIN_DX1 PIN_A6
#define PIN_DX2 PIN_A7
#define PIN_ACCEL_X 1
#define PIN_ACCEL_Y 2
#define PIN_ACCEL_Z 3 // Unused
#define PIN_CALIBRATION PIN_B2

#define PIN_DEBUG1 PIN_B0 // DEBUG
#define PIN_DEBUG2 PIN_B1 // DEBUG

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

// Accelerometer data collect function
void accel_read(int *x, int *y, int *z);

// Servo control function declaration
void pos(int servo1, int pos1, int servo2, int pos2);

// global vars
int i;
int x, y, z;
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
  
  pinMode(PIN_DEBUG1, OUTPUT); // DEBUG
  pinMode(PIN_DEBUG2, OUTPUT); // DEBUG
  
  pinMode(PIN_CALIBRATION, INPUT);
  digitalWrite(PIN_CALIBRATION, HIGH);

  i=0;
  boot=1;
  debug=0;
  
  neutral_x=511;
  neutral_y=511;
  neutral_z=511;

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
    accel_read(&x, &y, &z);
    
    digitalWrite(PIN_DEBUG1, HIGH);
    delay(100);
    digitalWrite(PIN_DEBUG1, LOW);
    
    delay(500);
    
    // Position detection
    if(boot==1) animation=BOOT;
    else if(y < neutral_y+SAD_MAX_Y) animation=SAD;
    else if(x > neutral_x+ALERT_MIN_X && x < neutral_x+ALERT_MAX_X && y > neutral_y+ALERT_MIN_Y) animation=ALERT;
    else if(x > neutral_x+AWW_MIN_X && y > neutral_y+AWW_MIN_Y) animation=AWW;
    else if(x < neutral_x+WINK_MAX_X && y > neutral_y+WINK_MIN_Y) animation=WINK;
    else animation=NORMAL;
  }
  
  // Run animation
  if(animation==DEBUG) {
    digitalWrite(PIN_DEBUG2, HIGH);
    pos(PIN_SX1, 170, PIN_DX1, 10);
    pos(PIN_SX2, 20, PIN_DX2, 160);
    delay(5000);
    digitalWrite(PIN_DEBUG2, LOW);
  }
  else if(animation==BOOT) {
    boot=0;
    pos(PIN_SX1, 170, PIN_DX1, 10);
    pos(PIN_SX2, 10, PIN_DX2, 170);
  }
  else if(animation==NORMAL) {
    digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
    pos(PIN_SX1, 160, PIN_DX1, 20);
    pos(PIN_SX2, 30, PIN_DX2, 150);
  }
  else if(animation==SAD) {
    digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
    digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
    pos(PIN_SX1, 60, PIN_DX1, 120);
    pos(PIN_SX2, 110, PIN_DX2, 70);
  }
  else if(animation==ALERT) {
    digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
    digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
    digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
    pos(PIN_SX1, 170, PIN_DX1, 10);
    pos(PIN_SX2, 10, PIN_DX2, 170);
  }
  else if(animation==AWW) {
    digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
        digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
        digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
        digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
    pos(PIN_SX1, 150, PIN_DX1, 30);
    pos(PIN_SX2, 40, PIN_DX2, 140);
  }
  else if(animation==WINK) {
    digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
        digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
        digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
        digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    delay(200);
        digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);

    
    
     //   pos(PIN_SX1, 160, PIN_DX1, 20);
   // pos(PIN_SX2, 30, PIN_DX2, 150);
    
    for(i=0; i<2; i++) {
      pos(PIN_SX1, 160, PIN_DX1, 130);
      pos(PIN_SX2, 30, PIN_DX2, 80);
      delay(100);
      pos(PIN_SX1, 160, PIN_DX1, 160);
      pos(PIN_SX2, 30, PIN_DX2, 110);
      delay(100);
      pos(PIN_SX1, 160, PIN_DX1, 130);
      pos(PIN_SX2, 30, PIN_DX2, 140);
      delay(100);
      pos(PIN_SX1, 160, PIN_DX1, 100);
      pos(PIN_SX2, 30, PIN_DX2, 110);
      delay(100);
    }
    pos(PIN_SX1, 160, PIN_DX1, 20);
    pos(PIN_SX2, 30, PIN_DX2, 150);
  }
  delay(500);
}

void accel_read(int *x, int *y, int *z) {
  int i;
  int buffer_x=0;
  int buffer_y=0;
  int buffer_z=0;
  int min_x=1023;
  int max_x=0;
  int min_y=1023;
  int max_y=0;
  int min_z=1023;
  int max_z=0;
  int tmp;

  // Accelerometer Read
  for(i=0; i<10; i++) {
    tmp=analogRead(PIN_ACCEL_X);
    if(tmp<min_x) min_x=tmp;
    if(tmp>max_x) max_x=tmp;
    buffer_x+=tmp;
    tmp=analogRead(PIN_ACCEL_Y);
    if(tmp<min_y) min_y=tmp;
    if(tmp>max_y) max_y=tmp;
    buffer_y+=tmp;
    tmp=analogRead(PIN_ACCEL_Z);
    if(tmp<min_z) min_z=tmp;
    if(tmp>max_z) max_z=tmp;
    buffer_z+=tmp;
    delay(20);
  }
    
    // Accelerometer data normalization
    *x=(buffer_x-min_x-max_x)/8;
    *y=(buffer_y-min_y-max_y)/8;
    *z=(buffer_z-min_z-max_z)/8;

}

void pos(int servo1, int pos1, int servo2=-1, int pos2=0) {
  int i;
  
  pos1=map(pos1, 0, 179, 500, 2500);
  pos2=map(pos2, 0, 179, 500, 2500);

  for(i=0; i<5; i++) {
    digitalWrite(servo1, HIGH);
    delayMicroseconds(pos1);
    digitalWrite(servo1, LOW);
    if(servo2 != -1) {
      digitalWrite(servo2, HIGH);
      delayMicroseconds(pos2);
      digitalWrite(servo2, LOW);
    }
    delay(20);
  }
}

void calibration() {
    accel_read(&neutral_x, &neutral_y, &neutral_z);
}
