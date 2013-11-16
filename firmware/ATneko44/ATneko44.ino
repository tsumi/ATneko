/***************************************
        *** ATneko Firmware ***
            
  Software Version:    0.1.0
  Supported Hardware:  ATneko44 @ 8MHZ

***************************************/

// Pin Map
#define PIN_SX1 PIN_A5
#define PIN_SX2 PIN_A6
#define PIN_DX1 PIN_A4
#define PIN_DX2 PIN_A7
#define PIN_ACCEL_X 1
#define PIN_ACCEL_Y 2
#define PIN_ACCEL_Z 3 // Unused
#define PIN_CALIBRATION PIN_B2
#define PIN_DEBUG1 PIN_B0
#define PIN_DEBUG2 PIN_B1

// SAD Trigger
#define SAD_MAX_Y -20

// ALERT Trigger
#define ALERT_MIN_X -10
#define ALERT_MAX_X 10
#define ALERT_MIN_Y 20

// AWW Trigger
#define AWW_MIN_X 10
#define AWW_MIN_Y -20

// WINK Trigger
#define WINK_MAX_X -10
#define WINK_MIN_Y -20

// Animations
#define DEBUG 0
#define BOOT 1
#define NORMAL 2
#define SAD 3
#define ALERT 4
#define AWW 5
#define WINK 6

// Accelerometer data collect function declaration
void accel_read(int *x, int *y, int *z);

// Servo control function declaration
void pos(int sx1, int pos_sx1, int dx1, int pos_dx1);

// Debug led function declaration
void debug_led(int count, int sleep);

// global vars
int i;
int x, y, z;
int neutral_x, neutral_y, neutral_z;
int animation;
int debug;
int boot;
int last_animation;
int curpos_sx1, curpos_sx2, curpos_dx1, curpos_dx2;

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
  
  curpos_sx1=-180;
  curpos_sx2=-180;
  curpos_dx1=-180;
  curpos_dx2=-180;

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
      if(i>AWW) i=NORMAL;    
    }
    debug_led(animation, 200);
    delay(5000);
  }
  else {
    // STANDARD MODE
    
    // Accelerometer Read
    accel_read(&x, &y, &z);
    
    // Position detection
    if(boot==1) animation=BOOT;
    else if(y < neutral_y+SAD_MAX_Y) animation=SAD;
    else if(x > neutral_x+ALERT_MIN_X && x < neutral_x+ALERT_MAX_X && y > neutral_y+ALERT_MIN_Y) animation=ALERT;
    else if(x > neutral_x+AWW_MIN_X && y > neutral_y+AWW_MIN_Y) animation=AWW;
    else if(x < neutral_x+WINK_MAX_X && y > neutral_y+WINK_MIN_Y) animation=AWW; //animation=WINK;
    else animation=NORMAL;
  }
  
  // Run animation
  if(animation==DEBUG) {
    pos(PIN_SX1, 170, PIN_DX1, 10, PIN_SX2, 20, PIN_DX2, 160);
    delay(5000);
  }
  else if(animation==BOOT) {
    boot=0;
    pos(PIN_SX1, 160, PIN_DX1, 20, PIN_SX2, 30, PIN_DX2, 150);
    last_animation=animation;
  }
  else if(animation==NORMAL) {
    if(last_animation!=animation) {
      pos(PIN_SX1, 160, PIN_DX1, 20, PIN_SX2, 30, PIN_DX2, 150);
      last_animation=animation;
    }
  }
  else if(animation==SAD) {
    if(last_animation!=animation) {
      pos(PIN_SX1, 90, PIN_DX1, 90, PIN_SX2, 90, PIN_DX2, 90);
      last_animation=animation;
    }
  }
  else if(animation==ALERT) {
    if(last_animation!=animation) {
      pos(PIN_SX1, 170, PIN_DX1, 10, PIN_SX2, 15, PIN_DX2, 165);
      last_animation=animation;
    }
  }
  else if(animation==AWW) {
    if(last_animation!=animation) {
      pos(PIN_SX1, 140, PIN_DX1, 40, PIN_SX2, 50, PIN_DX2, 130);
      last_animation=animation;
    }
  }
  /*else if(animation==WINK) {
    if(last_animation!=animation) {
      for(i=0; i<2; i++) {
        pos(PIN_SX1, 160, PIN_DX1, 130, PIN_SX2, 30, PIN_DX2, 80);
        delay(100);
        pos(PIN_SX1, 160, PIN_DX1, 160, PIN_SX2, 30, PIN_DX2, 110);
        delay(100);
        pos(PIN_SX1, 160, PIN_DX1, 130, PIN_SX2, 30, PIN_DX2, 140);
        delay(100);
        pos(PIN_SX1, 160, PIN_DX1, 100, PIN_SX2, 30, PIN_DX2, 110);
        delay(100);
      }
      pos(PIN_SX1, 160, PIN_DX1, 20, PIN_SX2, 30, PIN_DX2, 150);
      last_animation=animation;
    }
  }*/
  delay(200);
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
  for(i=0; i<15; i++) {
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
  *x=(buffer_x-min_x-max_x)/13;
  *y=(buffer_y-min_y-max_y)/13;
  *z=(buffer_z-min_z-max_z)/13;
}

void pos(int sx1, int pos_sx1, int dx1, int pos_dx1, int sx2, int pos_sx2, int dx2, int pos_dx2) {
  int i, k;
  int max1, max2;
  
  i=pos_sx1-curpos_sx1;
  if(i<0) i=i*(-1);
  k=pos_dx1-curpos_dx1;
  if(k<0) k=k*(-1);
  max1=max(i, k);
  max1=(max1/10)+3;
  
  i=pos_sx2-curpos_sx2;
  if(i<0) i=i*(-1);
  k=pos_dx2-curpos_dx2;
  if(k<0) k=k*(-1);
  max2=max(i, k);
  max2=(max2/10)+3;
  
  curpos_sx1=pos_sx1;
  curpos_dx1=pos_dx1;
  curpos_sx2=pos_sx2;
  curpos_dx2=pos_dx2;
  
  pos_sx1=map(pos_sx1, 0, 179, 500, 2500);
  pos_dx1=map(pos_dx1, 0, 179, 500, 2500);
  pos_sx2=map(pos_sx2, 0, 179, 500, 2500);
  pos_dx2=map(pos_dx2, 0, 179, 500, 2500);

  for(i=0; i<max1; i++) {
    digitalWrite(sx1, HIGH);
    delayMicroseconds(pos_sx1);
    digitalWrite(sx1, LOW);
    digitalWrite(dx1, HIGH);
    delayMicroseconds(pos_dx1);
    digitalWrite(dx1, LOW);
    delayMicroseconds(20000-pos_sx1-pos_dx1);
  }
  
  for(i=0; i<max2; i++) {
    digitalWrite(sx2, HIGH);
    delayMicroseconds(pos_sx2);
    digitalWrite(sx2, LOW);
    digitalWrite(dx2, HIGH);
    delayMicroseconds(pos_dx2);
    digitalWrite(dx2, LOW);
    delayMicroseconds(20000-pos_sx2-pos_dx2);
  }
}

void calibration() {
    accel_read(&neutral_x, &neutral_y, &neutral_z);
}

void debug_led(int count, int sleep) {
  int i;
  for(i=0; i<count; i++) {
    digitalWrite(PIN_DEBUG1, HIGH);
    delay(sleep);
    digitalWrite(PIN_DEBUG1, LOW);
    delay(sleep);
  }
}
