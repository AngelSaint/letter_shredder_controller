#include <Servo.h>
#include <string.h>

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
#define NEUTRAL 90

#define LL_SERVO_PIN PD3
#define RL_SERVO_PIN PD5
#define M_SERVO_PIN PD6
#define L_SERVO_PIN PB1
#define R_SERVO_PIN PB2

#define TOP_LASER_PIN PC4
#define BOX_1_LASER_PIN PC3
#define BOX_2_LASER_PIN PC2
#define BOX_3_LASER_PIN PC1
#define BOX_4_LASER_PIN PC0


Servo left_latch;
Servo right_latch;
Servo middle;
Servo left;
Servo right;


int top_laser = 1;
int box_1_laser = 1;
int box_2_laser = 1;
int box_3_laser = 1;
int box_4_laser = 1;


char bytes[100] = "";
String incomingBytes;


typedef enum{
  WAITING_FOR_CONNECTION,
  WAITING_FOR_MAIL,
  WAITING_FOR_PI,
  WAITING_FOR_DROP
}MailState;

MailState state = WAITING_FOR_CONNECTION;

void setup() {
  
  Serial.begin(9600);
  
  //Assign pins to motor
  left_latch.attach(LL_SERVO_PIN);
  right_latch.attach(RL_SERVO_PIN);
  middle.attach(M_SERVO_PIN);
  left.attach(L_SERVO_PIN);
  right.attach(R_SERVO_PIN);
  
  //SETTING LASER PIN TO INPUT
  pinMode(TOP_LASER_PIN, INPUT);
  pinMode(BOX_1_LASER_PIN, INPUT);
  pinMode(BOX_2_LASER_PIN, INPUT);
  pinMode(BOX_3_LASER_PIN, INPUT);
  pinMode(BOX_4_LASER_PIN, INPUT);

  // reset servos to neutral pos
  reset_servo();
}

void reset_servo(){
  left_latch.write(LL_CLOSED);
  right_latch.write(RL_CLOSED);
  delay(1000);
  middle.write(NEUTRAL);
  left.write(NEUTRAL);
  right.write(NEUTRAL);
  delay(5000);
}



void send_to_box_1(){
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
}

void send_to_box_2(){
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
}

void send_to_box_3(){
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

}

void send_to_box_4(){
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
}


int validString(char* bytes){
  if (strncmp(bytes, "Box: ", 5*sizeof(char))){
    return 1;
  }
  return 0;
}

int parseString(char* bytes){
  return atoi((char*)(bytes+5));
}

void loop() {
  while(state == WAITING_FOR_CONNECTION){
    incomingBytes = Serial.readStringUntil('\n');
    if (incomingBytes == "Begin"){
      state = WAITING_FOR_MAIL;
      Serial.println("Estab\n");
    }else{
      state = WAITING_FOR_CONNECTION;
    }
  }

  while (state == WAITING_FOR_MAIL){
    // Mail has shown up on the sensor
    if ((top_laser = analogRead(TOP_LASER_PIN)) > 512){
      Serial.printf("Ready\n", top_laser);
    }else{
      Serial.printf("\tInvalid Laser Value: %d\n", top_laser);
    }
  }

  while (state == WAITING_FOR_PI){
    incomingBytes = Serial.readStringUntil('\n');
    incomingBytes.toCharArray(bytes, 100);

    // Check for a valid string
    if (validString(bytes) == 1){
      int box_number = parseString(bytes);
      switch (box_number){
        case 1:
          send_to_box_1();
          state = WAITING_FOR_DROP;

          break;
        case 2:
          send_to_box_2();
          state = WAITING_FOR_DROP;
          break;
        case 3:
          send_to_box_3();
          state = WAITING_FOR_DROP;
          break;
        case 4:
          send_to_box_4();
          state = WAITING_FOR_DROP;
          break;
        default:
          Serial.printf("\tInvalid Box Number: %d\n", box_number);
          state = WAITING_FOR_PI;
          break;
      }
    }else{
      Serial.println("\tInvalid\n");
      state = WAITING_FOR_PI;
    }
  }

  while (state == WAITING_FOR_DROP){
    box_1_laser = analogRead(BOX_1_LASER_PIN);
    box_2_laser = analogRead(BOX_2_LASER_PIN);
    box_3_laser = analogRead(BOX_3_LASER_PIN);
    box_4_laser = analogRead(BOX_4_LASER_PIN);
    int sum =  box_1_laser + box_2_laser + box_3_laser + box_4_laser;
    // We have something blocking one of the sensors so we send the values of the lasers to the pi for debugging
    if (sum > 1536){
      Serial.printf("Mail is blocking sensor!! Box 1: %d\tBox 2: %d\tBox 3: %d\tBox 4: %d\tSum: %d\n", box_1_laser, box_2_laser, box_3_laser, box_4_laser, sum);
      state = WAITING_FOR_MAIL;
    }else{
      if (box_1_laser > 512){
        Serial.println("Mail: 1\n");
        state = WAITING_FOR_MAIL;
      }else if (box_2_laser > 512){
        Serial.println("Mail: 2\n");
        state = WAITING_FOR_MAIL;
      }else if (box_3_laser > 512){
        Serial.println("Mail: 3\n");
        state = WAITING_FOR_MAIL;
      }else if (box_4_laser > 512){
        Serial.println("Mail: 4\n");
        state = WAITING_FOR_MAIL;
      }
    }
  }



}

//TODO: Check is laser interface is right.
