#include <Servo.h>
#include <ALLBOT.h>

ALLBOT BOT(8);

enum MotorName {
    hipFrontLeft,
    hipFrontRight,
    hipRearLeft,
    hipRearRight,
    kneeFrontLeft,
    kneeFrontRight,
    kneeRearLeft,
    kneeRearRight
};

byte buzzerPin = 13;

byte DEFAULT_HIP_POS = 45;
byte DEFAULT_KNEE_POS = 45;

// Positions for 'leaning' action.
// Note: 'Forward' and 'Backward' indicate direciton of movement, not sides of the BOT
enum Lean {
    LEAN_HIPS_FORWARD = 65,
    LEAN_HIPS_BACKWARD = 30,
    LEAN_KNEES_FORWARD = 30,
    LEAN_KNEES_BACKWARD = 75
};

enum LeanSide {
    LEAN_HIPS_SIDE_FORWARD = 30,
    LEAN_HIPS_SIDE_BACKWARD = 55,
    LEAN_KNEES_SIDE_FORWARD = 28,
    LEAN_KNEES_SIDE_BACKWARD = 80
};

enum Directions {
    DIR_FORWARD,
    DIR_BACKWARD,
    DIR_LEFT,
    DIR_RIGHT,
};
enum DirectionsDiagonal {
    DIR_DIAG_NW, // North-West
    DIR_DIAG_SW, // South-West
    DIR_DIAG_SE, // South-East
    DIR_DIAG_NE, // North-East
};



int speedms = 250;

void setup() {
    Serial.begin(9600);

    // BOT.attach(motorname, pin, init-angle, flipped, offset-angle);
    BOT.attach(hipFrontLeft,   11,  90, 0, 0);
    BOT.attach(hipFrontRight,   2,  90, 1, 0);
    BOT.attach(hipRearLeft,    10,  90, 1, 0);
    BOT.attach(hipRearRight,    3,  90, 0, 0);

    BOT.attach(kneeFrontLeft,   9,  90, 1, -5);
    BOT.attach(kneeFrontRight,  4,  90, 0, 0);
    BOT.attach(kneeRearLeft,    8,  90, 1, -2);
    BOT.attach(kneeRearRight,   5,  90, 0, 0);

    pinMode(buzzerPin, OUTPUT);

    rise();

    beep(1, 50);
    beep(1, 255);
    beep(3, 0);

    randomSeed(analogRead(A0));
}

void loop() {
    //listenToSerial();

    //int action = random(0, 9);
    //executeCommand(action);

    walk();
}

// allows to manipulate separate servos through Serial Bus
void listenToSerial() {

    if (Serial.available()) {
        String input = Serial.readString();
        // byte start = input.indexOf('[');
        // byte end = input.indexOf(']');
        // if (start < 0 || end < 0 || start>=end) {
        //     Serial.println("Invalid command");
        //     return;
        // }
        //input = input.substring(start+1, end);
        //Serial.println("Command: " + input);

        String command = input.substring(0, 3);
        command.toUpperCase();
        MotorName motor = getMotorByName(command);

        int angle = input.substring(4).toInt();
        angle = constrain(angle, 0, 180);

        Serial.println("Servo: " + command + ", angle: " + angle);

        // execute
        BOT.move(motor, angle);
        BOT.animate(speedms);
    }
}

MotorName getMotorByName(String motorName) {
    if (motorName == "HFR")
        return hipFrontRight;
    if (motorName == "HFL")
        return hipFrontLeft;
    if (motorName == "HRR")
        return hipRearRight;
    if (motorName == "HRL")
        return hipRearLeft;
    if (motorName == "KFR")
        return kneeFrontRight;
    if (motorName == "KFL")
        return kneeFrontLeft;
    if (motorName == "KRR")
        return kneeRearRight;
    if (motorName == "KRL")
        return kneeRearLeft;
    // Should never happen
    return hipFrontLeft;
}

void executeCommand(int action) {
    int iterations = random(2, 6);
    switch (action) {
        case 0:
            lean(DIR_FORWARD);
            break;
        case 1:
            lean(DIR_LEFT);
            break;
        case 2:
            lean(DIR_BACKWARD);
            break;
        case 3:
            lean(DIR_RIGHT);
            break;
        case 4:
            hoolaHoop(iterations);
            break;
        case 5:
            waveFrontLeft(iterations);
            break;
        case 6:
            knockFrontLeft(iterations);
            break;
        default:
            break;
    }

    delay(2000);
}

// Make the bot rise after activation
void rise() {
    int speed = 500;
    delay(speed);

    BOT.move(hipFrontLeft,  DEFAULT_HIP_POS);
    BOT.move(hipFrontRight, DEFAULT_HIP_POS);
    BOT.move(hipRearLeft,   DEFAULT_HIP_POS);
    BOT.move(hipRearRight,  DEFAULT_HIP_POS);
    BOT.animate(speed);

    delay(speed);

    BOT.move(kneeFrontLeft,  DEFAULT_KNEE_POS);
    BOT.move(kneeFrontRight, DEFAULT_KNEE_POS);
    BOT.move(kneeRearLeft,   DEFAULT_KNEE_POS);
    BOT.move(kneeRearRight,  DEFAULT_KNEE_POS);
    BOT.animate(speed);
}

void walk() {
    int angleKneeUp = 70;
    int angleKneeCarry = 45;    // 40? 50?

// reset
    setDefaultPos();
    BOT.animate(speedms);


    while(true) {

    // step left
        // move front leg
        BOT.move(kneeFrontRight, angleKneeUp);
        BOT.animate(speedms);
        BOT.move(hipFrontRight,  30);
        BOT.animate(speedms);
        BOT.move(kneeFrontRight, DEFAULT_KNEE_POS);
        BOT.animate(speedms);

        // move rear leg
        BOT.move(kneeRearLeft, angleKneeUp);
        BOT.animate(speedms);
        BOT.move(hipRearLeft,  60);
        BOT.animate(speedms);
        BOT.move(kneeRearLeft, DEFAULT_KNEE_POS);
        BOT.animate(speedms);

        BOT.move(kneeFrontLeft,  25); // move direction
        BOT.move(kneeRearLeft,   45);
        BOT.move(kneeRearRight,  60); // rear leg
        BOT.move(kneeFrontRight, 45);

        BOT.move(hipFrontLeft,  45);
        BOT.move(hipRearLeft,   45);  // weight is here
        BOT.move(hipRearRight,  45);
        BOT.move(hipFrontRight, 45);  // weight is here
        BOT.animate(speedms);
        delay(500);


    // step right
        // move front leg
        BOT.move(kneeFrontLeft, angleKneeUp);
        BOT.animate(speedms);
        BOT.move(hipFrontLeft,  30);
        BOT.animate(speedms);
        BOT.move(kneeFrontLeft, DEFAULT_KNEE_POS);
        BOT.animate(speedms);

        // move rear leg
        BOT.move(kneeRearRight, angleKneeUp);
        BOT.animate(speedms);
        BOT.move(hipRearRight,  60);
        BOT.animate(speedms);
        BOT.move(kneeRearRight, DEFAULT_KNEE_POS);
        BOT.animate(speedms);

        BOT.move(kneeFrontLeft,  45);
        BOT.move(kneeRearLeft,   60); // rear leg
        BOT.move(kneeRearRight,  45);
        BOT.move(kneeFrontRight, 25); // move direction

        BOT.move(hipFrontLeft,  45);  // weight is here
        BOT.move(hipRearLeft,   45);
        BOT.move(hipRearRight,  45);  // weight is here
        BOT.move(hipFrontRight, 45);
        BOT.animate(speedms);
        delay(500);
    }


    delay(60000);
}

void waveFrontLeft(byte iterations) {
    // Stabilize on 3 legs
    BOT.move(hipFrontRight, 20);
    BOT.move(hipRearLeft, 70);
    BOT.move(kneeRearRight, 60);
    BOT.animate(speedms);

    BOT.move(kneeFrontLeft, 175);
    BOT.animate(speedms);

    delay(speedms);

    for (byte i=0; i<iterations; i++) {
        BOT.move(hipFrontLeft, 10);
        BOT.animate(speedms*2/3);
        BOT.move(hipFrontLeft, 60);
        BOT.animate(speedms*2/3);
    }

    BOT.move(hipFrontLeft, DEFAULT_HIP_POS);
    BOT.animate(speedms/2);

    BOT.move(kneeFrontLeft, DEFAULT_KNEE_POS);
    BOT.animate(speedms);

    // Return to stable 4-leg position
    BOT.move(hipFrontRight, DEFAULT_HIP_POS);
    BOT.move(hipRearLeft, DEFAULT_HIP_POS);
    BOT.move(kneeRearRight, DEFAULT_KNEE_POS);
    BOT.animate(speedms);
}

void knockFrontLeft(byte iterations) {
    // Stabilize on 3 legs
    BOT.move(hipFrontRight, 20);
    BOT.move(hipRearLeft, 70);
    BOT.move(kneeRearRight, 60);
    BOT.animate(speedms);

    for (byte i=0; i<iterations; i++) {
        BOT.move(kneeFrontLeft, 80);
        BOT.animate(100);
        BOT.move(kneeFrontLeft, 35);
        BOT.animate(100);
    }

    // Return to stable 4-leg position
    BOT.move(hipFrontRight, DEFAULT_HIP_POS);
    BOT.move(hipRearLeft, DEFAULT_HIP_POS);
    BOT.move(kneeRearRight, DEFAULT_KNEE_POS);
    BOT.animate(speedms);
}


void lean(Directions dir) {
    switch (dir) {
        case DIR_FORWARD:
            setLeanForwardPos();
            break;
        case DIR_BACKWARD:
            setLeanBackwardPos();
            break;
        case DIR_LEFT:
            setLeanLeftPos();
            break;
        case DIR_RIGHT:
            setLeanRightPos();
            break;
        default:
            break;
    }

    BOT.animate(speedms);

    delay(speedms*2);

    setDefaultPos();
    BOT.animate(speedms);
}

// void puhsUps(byte count) {
//     setLeanForwardPos();
//     BOT.animate(speedms);
//     delay(speedms);
//
//     for (int i=0; i<count; i++) {
//         BOT.move(kneeFrontLeft, 15);
//         BOT.move(kneeFrontRight, 15);
//         BOT.animate(speedms);
//         BOT.move(kneeFrontLeft, 30);
//         BOT.move(kneeFrontRight, 30);
//         BOT.animate(speedms);
//     }
//     delay(speedms);
//
//     setDefaultPos();
//     BOT.animate(speedms);
// }

void hoolaHoop(byte iterations) {
    for (byte i=0; i<iterations; i++) {
        setLeanForwardPos();
        BOT.animate(speedms);
        setLeanLeftPos();
        BOT.animate(speedms);
        setLeanBackwardPos();
        BOT.animate(speedms);
        setLeanRightPos();
        BOT.animate(speedms);
    }
    setDefaultPos();
    BOT.animate(speedms);
}

// Moves all servos to 'leanForward' configuration. No .animate()
void setLeanForwardPos() {
    BOT.move(hipRearLeft, LEAN_HIPS_BACKWARD);
    BOT.move(hipRearRight, LEAN_HIPS_BACKWARD);
    BOT.move(hipFrontLeft, LEAN_HIPS_FORWARD);
    BOT.move(hipFrontRight, LEAN_HIPS_FORWARD);

    BOT.move(kneeFrontLeft, LEAN_KNEES_FORWARD);
    BOT.move(kneeFrontRight, LEAN_KNEES_FORWARD);
    BOT.move(kneeRearLeft, LEAN_KNEES_BACKWARD);
    BOT.move(kneeRearRight, LEAN_KNEES_BACKWARD);
}
void setLeanBackwardPos() {
    BOT.move(hipRearLeft, LEAN_HIPS_FORWARD);
    BOT.move(hipRearRight, LEAN_HIPS_FORWARD);
    BOT.move(hipFrontLeft, LEAN_HIPS_BACKWARD);
    BOT.move(hipFrontRight, LEAN_HIPS_BACKWARD);

    BOT.move(kneeFrontLeft, LEAN_KNEES_BACKWARD);
    BOT.move(kneeFrontRight, LEAN_KNEES_BACKWARD);
    BOT.move(kneeRearLeft, LEAN_KNEES_FORWARD);
    BOT.move(kneeRearRight, LEAN_KNEES_FORWARD);
}
void setLeanLeftPos() {
    BOT.move(hipRearLeft, LEAN_HIPS_SIDE_FORWARD);
    BOT.move(hipRearRight, LEAN_HIPS_SIDE_BACKWARD);
    BOT.move(hipFrontLeft, LEAN_HIPS_SIDE_FORWARD);
    BOT.move(hipFrontRight, LEAN_HIPS_SIDE_BACKWARD);

    BOT.move(kneeFrontLeft, LEAN_KNEES_SIDE_FORWARD);
    BOT.move(kneeFrontRight, LEAN_KNEES_SIDE_BACKWARD);
    BOT.move(kneeRearLeft, LEAN_KNEES_SIDE_FORWARD);
    BOT.move(kneeRearRight, LEAN_KNEES_SIDE_BACKWARD);
}
void setLeanRightPos() {
    BOT.move(hipRearLeft, LEAN_HIPS_SIDE_BACKWARD);
    BOT.move(hipRearRight, LEAN_HIPS_SIDE_FORWARD);
    BOT.move(hipFrontLeft, LEAN_HIPS_SIDE_BACKWARD);
    BOT.move(hipFrontRight, LEAN_HIPS_SIDE_FORWARD);

    BOT.move(kneeFrontLeft, LEAN_KNEES_SIDE_BACKWARD);
    BOT.move(kneeFrontRight, LEAN_KNEES_SIDE_FORWARD);
    BOT.move(kneeRearLeft, LEAN_KNEES_SIDE_BACKWARD);
    BOT.move(kneeRearRight, LEAN_KNEES_SIDE_FORWARD);
}


// Moves all servos to default configuration. Note: .animate() is not called!
void setDefaultPos() {
    BOT.move(hipFrontLeft,   DEFAULT_HIP_POS);
    BOT.move(hipFrontRight,  DEFAULT_HIP_POS);
    BOT.move(hipRearLeft,    DEFAULT_HIP_POS);
    BOT.move(hipRearRight,   DEFAULT_HIP_POS);
    BOT.move(kneeFrontLeft,  DEFAULT_KNEE_POS);
    BOT.move(kneeFrontRight, DEFAULT_KNEE_POS);
    BOT.move(kneeRearLeft,   DEFAULT_KNEE_POS);
    BOT.move(kneeRearRight,  DEFAULT_KNEE_POS);
}

void beep(int beeps, int speedms){
    for (int i = 0; i < beeps; i++){
        for (int j = 0; j < 255; j++){
            digitalWrite(buzzerPin, HIGH);
            delayMicroseconds((355-j) + (speedms*2));
            digitalWrite(buzzerPin, LOW);
            delayMicroseconds((355-j) + (speedms*2));
        }
        delay(30);
    }
}

// void moveOne(int servo, int angle) {
//     BOT.move(servo, angle);
//     BOT.animate(speedms);
//     delay(speedms);
// }
