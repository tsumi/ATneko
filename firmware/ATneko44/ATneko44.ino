/***************************************
        *** ATneko Firmware ***
            
  Software Version:     0.0.0
  Supported Hardware:   ATneko44

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

// Servo control function declaration
void pos(int servo1, int pos1, int servo2, int pos2, int servo3, int pos3, int servo4, int pos4);

// global vars
int i;
int x, y, z;
int buffer_x, buffer_y, buffer_z;
int max_x, min_x, max_y, min_y, max_z, min_z;
int tmp;
int neutral_x, neutral_y, neutral_z;
int animation;
int debug;
int boot;

void setup() {
  
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

  delay(500);

  if(digitalRead(PIN_CALIBRATION)==0) {
    // DEBUG MODE
    debug=1;
  }
  else {
    // STANDARD MODE
    attachInterrupt(0, calibration, FALLING);
  }
  
    analogReference(EXTERNAL);
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
  
    //Initialization buffers
    buffer_x=0;
    buffer_y=0;
    buffer_z=0;
    min_x=1023;
    max_x=0;
    min_y=1023;
    max_y=0;
    min_z=1023;
    max_z=0;
    
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
    x=(buffer_x-min_x-max_x)/8;
    y=(buffer_y-min_y-max_y)/8;
    z=(buffer_z-min_z-max_z)/8;
    
    digitalWrite(PIN_DEBUG1, LOW);
    digitalWrite(PIN_DEBUG2, LOW);
    
    delay(500);
    
    // Position detection
    if(boot==1) animation=BOOT;
    else if(y < neutral_y+SAD_MAX_Y) animation=NORMAL;
    else if(x > neutral_x+ALERT_MIN_X && x < neutral_x+ALERT_MAX_X && y > neutral_y+ALERT_MIN_Y) animation=ALERT;
    else if(x > neutral_x+AWW_MIN_X && y > neutral_y+AWW_MIN_Y) animation=ALERT;
    else if(x > neutral_x+WINK_MAX_X && y > neutral_y+WINK_MIN_Y) animation=WINK;
  }
  
  // Run animation
  if(animation==DEBUG) {
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
  else if(animation==BOOT) {
    boot=0;
    digitalWrite(PIN_DEBUG1, HIGH);
    digitalWrite(PIN_DEBUG2, HIGH);
    pos(PIN_SX1, 180, PIN_SX2, 180, PIN_DX1, 180, PIN_DX2, 180);
  }
  else if(animation==NORMAL) {
    digitalWrite(PIN_DEBUG1, HIGH);
    digitalWrite(PIN_DEBUG2, HIGH);
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
  else if(animation==SAD) {
    digitalWrite(PIN_DEBUG1, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG1, LOW);
    digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    pos(PIN_SX1, 180, PIN_SX2, 180, PIN_DX1, 180, PIN_DX2, 180);
  }
  else if(animation==ALERT) {
    digitalWrite(PIN_DEBUG1, LOW);
    digitalWrite(PIN_DEBUG2, HIGH);
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
  else if(animation==AWW) {
    digitalWrite(PIN_DEBUG2, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG2, LOW);
    digitalWrite(PIN_DEBUG1, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG1, LOW);
    pos(PIN_SX1, 180, PIN_SX2, 180, PIN_DX1, 180, PIN_DX2, 180);
  }
  else if(animation==WINK) {
    digitalWrite(PIN_DEBUG1, HIGH);
    digitalWrite(PIN_DEBUG2, LOW);
    pos(PIN_SX1, 0, PIN_SX2, 0, PIN_DX1, 0, PIN_DX2, 0);
  }
  delay(500);
}

void pos(int servo1, int pos1, int servo2=-1, int pos2=0, int servo3=-1, int pos3=0, int servo4=-1, int pos4=0) {
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

void calibration() {
  
    digitalWrite(PIN_DEBUG1, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG1, LOW);
    
    digitalWrite(PIN_DEBUG1, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG1, LOW);
    
    digitalWrite(PIN_DEBUG1, HIGH);
    delay(200);
    digitalWrite(PIN_DEBUG1, LOW);
  
    //Initialization buffers
    buffer_x=0;
    buffer_y=0;
    buffer_z=0;
    min_x=1023;
    max_x=0;
    min_y=1023;
    max_y=0;
    min_z=1023;
    max_z=0;
    
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
    neutral_x=(buffer_x-min_x-max_x)/8;
    neutral_y=(buffer_y-min_y-max_y)/8;
    neutral_z=(buffer_z-min_z-max_z)/8;
}
