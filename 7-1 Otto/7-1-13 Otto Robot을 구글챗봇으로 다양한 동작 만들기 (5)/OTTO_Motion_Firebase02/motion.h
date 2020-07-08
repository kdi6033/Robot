#include "Oscillator.h"
#include <Servo.h>
#include "Oscillator.h"
#include <EEPROM.h>

#define N_SERVOS 4
//-- First step: Configure the pins where the servos are attached
/*
         --------------- 
        |     O   O     |
        |---------------|
YR 3==> |               | <== YL 2
         --------------- 
            ||     ||
            ||     ||
RR 5==>   -----   ------  <== RL 4
         |-----   ------|
*/
#define EEPROM_TRIM false 
// Activate to take callibration data from internal memory
#define TRIM_RR 7
#define TRIM_RL 4
#define TRIM_YR  4
#define TRIM_YL -7
//OTTO.setTrims(-7,-4,-4,7);

/*
#define PIN_RR 5
#define PIN_RL 4
#define PIN_YR 3
#define PIN_YL 2
*/
#define PIN_RR D4
#define PIN_RL D2
#define PIN_YR D3
#define PIN_YL D1

#define INTERVALTIME 10.0 

Oscillator servo[N_SERVOS];

// TEMPO: 121 BPM
int t=495;
double pause=0;

unsigned long final_time;
unsigned long interval_time;
int oneTime;
int iteration;
float increment[N_SERVOS]; 
int oldPosition[]={90,90,90,90};


void walk(int steps, int T=1000);
void turnLeft(int steps, int T=3000);
void turnRight(int steps, int T=3000);
void backyard(int steps, int T=3000);
void goingUp(int tempo);
void drunk (int tempo);
void noGravity(int tempo);
void kickLeft(int tempo);
void kickRight(int tempo);
void run(int steps, int T=500);
void backyardSlow(int steps, int T=5000);
void moonWalkLeft(int steps, int T=1000);
void moonWalkRight(int steps, int T=1000);
void crusaito(int steps, int T=1000);
void swing(int steps, int T=1000);
void upDown(int steps, int T=1000);
void flapping(int steps, int T=1000);
void initSetMotor();
void lateral_fuerte(boolean side, int tempo);
void setupMotor();
