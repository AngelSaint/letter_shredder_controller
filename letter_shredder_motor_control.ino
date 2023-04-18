#include <Servo.h>
#define LL_PIN 3
#define RL_PIN 5
#define M_PIN 6
#define L_PIN 10
#define R_PIN 9

#define LL_CLOSED 100
#define LL_OPEN 160
#define RL_CLOSED 90
#define RL_OPEN 20

#define L_45 55
#define L_135 110
#define R_45 55
#define R_135 100
#define M_45 45
#define M_135 135



Servo left_latch;
Servo right_latch;
Servo middle;
Servo left;
Servo right;

int LL_pin = 10; //J1
int RL_pin = 9; //J2
int M_pin = 3; //J3
int L_pin = 5; //J4
int R_pin = 6; //J5

int top_laser_pin = PC0; //J6
int laser_1_pin = PC1; //J7
int laser_2_pin = PC2; //J8
int laser_3_pin = PC3; //J9
int laser_4_pin = PC4; //J10

int wait_for_pi = 0;

enum MailState{
WAITING_FOR_CONNECTION,
WAITING_FOR_MAIL,
WAITING_FOR_PI,
MOVING_SERVO,
DROPPING
}

MailState state;

void setup() {
  
  Serial.begin(9600);
  
  //Assign pins to motor
  left_latch.attach(LL_PIN);
  right_latch.attach(LR_PIN);
  middle.attach(M_PIN);
  left.attach(L_PIN);
  right.attach(R_PIN);
  
  //SETTING LASER PIN TO INPUT
  pinMode(top_laser_pin, INPUT);
  pinMode(laser_1_pin, INPUT);
  pinMode(laser_2_pin, INPUT);
  pinMode(laser_3_pin, INPUT);
  pinMode(laser_4_pin, INPUT);

  // reset servos to neutral pos
  left_latch.write(LL_CLOSED);
  right_latch.write(RL_CLOSED);
  delay(1000);
  middle.write(180);
  left.write(90);
  right.write(90);
}

void reset_servo(){
  left_latch.write(LL_CLOSED);
  right_latch.write(RL_CLOSED);
  delay(1000);
  middle.write(180);
  left.write(90);
  right.write(90);
  
  delay(5000);

}
string send_to_box1(){
    // STEP 1: Conrol inside servos
    left.write(L_135);
    middle.write(M_135);
    delay(1000);
    // STEP 2: Control latch servos
    left_latch.write(LL_OPEN);
    right_latch.write(RL_OPEN);
    delay(1500);
    // STEP 3: Reset servos
    reset_servo();
    // STEP 4: Check success with lasers
    if (laser_1_pin == 1){
      return "SUCCESS";
    }
    return "ERROR";
  }
  string send_to_box2(){
    // STEP 1: Conrol inside servos
    left.write(L_45);
    middle.write(M_135);
    delay(100);
    // STEP 2: Control latch servos
    left_latch.write(LL_OPEN);
    right_latch.write(RL_OPEN);
    delay(1500);
    // STEP 3: Reset servos
    reset_servo();
    // STEP 4: Check success with lasers
    if (laser_2_pin == 1){
      return "SUCCESS";
    }
    return "ERROR";
  
  }
  string send_to_box3(){
  // STEP 1: Conrol inside servos
    right.write(R_135);
    middle.write(M_45);
    delay(100);
    // STEP 2: Control latch servos
    left_latch.write(LL_OPEN);
    right_latch.write(RL_OPEN);
    delay(1500);
    // STEP 3: Reset servos
    reset_servo();
    // STEP 4: Check success with lasers
    if (laser_3_pin == 1){
      return "SUCCESS";
    }
    return "ERROR";
  }
  string send_to_box4(){
  // STEP 1: Conrol inside servos
    right.write(R_45);
    middle.write(M_45);
    delay(100);
    // STEP 2: Control latch servos
    left_latch.write(LL_OPEN);
    right_latch.write(RL_OPEN);
    delay(1000);
    // STEP 3: Reset servos
    reset_servo();
    // STEP 4: Check success with lasers
    if (laser_4_pin == 1){
      return "SUCCESS";
    }
    return "ERROR";
  }

void loop() {
  //Check is there is mail to be sorted into boxes
  if ( !wait_for_pi && top_laser_pin = 1 && Serial.available() > 0) {
     //Ask RPI to read mail info
    //Serial.write(); //FIGURE OUT WHAT SIGNAL TO SEND PI TO START PROGRAM
    wait_for_pi = 1;  
  }

  delay(100);

  if (wait_for_pi && Serial.available() > 0) {
  
    // read the incoming byte:
    incomingByte = Serial.readStringUntil('\n');

     if(incomingByte == 1){
        Serial.println(send_to_box1());
        wait_for_pi = 0;

    } else if (incomingByte == 2) {
        Serial.println(send_to_box2());
        wait_for_pi = 0;

    } else if (incomingByte == 3) {
        Serial.println(send_to_box3());
        wait_for_pi = 0;
      
    } else if (incomingByte == 4) {
        Serial.println(send_to_box4());
        wait_for_pi = 0;
      
    } else {
        //Still waiting for pi....
    } 
  }


}

//TODO: Check is laser interface is right.
