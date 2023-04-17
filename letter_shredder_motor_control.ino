#include <Servo.h>

Servo left_latch;
Servo right_latch;
Servo middle;
Servo left;
Servo right;

int LL_pin = PD3; //J1
int RL_pin = PD6; //J2
int M_pin = PB1; //J3
int L_pin = PB2; //J4
int R_pin = PB3; //J5

int top_laser_pin = PC0; //J6
int laser_1_pin = PC1; //J7
int laser_2_pin = PC2; //J8
int laser_3_pin = PC3; //J9
int laser_4_pin = PC4; //J10

int wait_for_pi = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  //Assign pins to motor
  left_latch.attach(LL_pin);
  right_latch.attach(LR_pin);
  middle.attach(M_pin);
  left.attach(L_pin);
  right.attach(R_pin);
  
  //SETTING LASER PIN TO INPUT
  pinMode(top_laser_pin, INPUT);
  pinMode(laser_1_pin, INPUT);
  pinMode(laser_2_pin, INPUT);
  pinMode(laser_3_pin, INPUT);
  pinMode(laser_4_pin, INPUT);

  // reset servos to neutral pos
  left_latch.write(180);
  right_latch.write(180);
  middle.write(180);
  left.write(180);
  right.write(180);
}

string send_to_box1(){
    // STEP 1: Conrol inside servos
    left.write(45);
    middle.write(45);
    delay(100);
    // STEP 2: Control latch servos
    left_latch.write(90);
    right_latch.write(90);
    delay(1500);
    // STEP 3: Reset servos
    left_latch.write(180);
    right_latch.write(180);
    middle.write(180);
    left.write(180);
    delay(100);
    // STEP 4: Check success with lasers
    if (laser_1_pin == 1){
      return "SUCCESS";
    }
    return "ERROR";
  }
  string send_to_box2(){
    // STEP 1: Conrol inside servos
    left.write(135);
    middle.write(45);
    delay(100);
    // STEP 2: Control latch servos
    left_latch.write(90);
    right_latch.write(90);
    delay(1500);
    // STEP 3: Reset servos
    left_latch.write(180);
    right_latch.write(180);
    middle.write(180);
    left.write(180);
    delay(100);
    // STEP 4: Check success with lasers
    if (laser_2_pin == 1){
      return "SUCCESS";
    }
    return "ERROR";
  
  }
  string send_to_box3(){
  // STEP 1: Conrol inside servos
    right.write(45);
    middle.write(135);
    delay(100);
    // STEP 2: Control latch servos
    left_latch.write(90);
    right_latch.write(90);
    delay(1500);
    // STEP 3: Reset servos
    left_latch.write(180);
    right_latch.write(180);
    middle.write(180);
    right.write(180);
    delay(100);
    // STEP 4: Check success with lasers
    if (laser_3_pin == 1){
      return "SUCCESS";
    }
    return "ERROR";
  }
  string send_to_box4(){
  // STEP 1: Conrol inside servos
    right.write(135);
    middle.write(135);
    delay(100);
    // STEP 2: Control latch servos
    left_latch.write(90);
    right_latch.write(90);
    delay(1000);
    // STEP 3: Reset servos
    left_latch.write(180);
    right_latch.write(180);
    middle.write(180);
    right.write(180);
    delay(100);
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
