// ======== Pin Definitions ========

// Sensor pins
#define S1 A0
#define S2 A1
#define S3 A2
#define S4 A3
#define S5 A4

// ======== Motor Driver 1 (Left side) ========
#define ENA 11
#define IN1 3
#define IN2 4
#define ENB 5
#define IN3 8
#define IN4 9

// ======== Motor Driver 2 (Right side, reversed OUT connections) ========
#define ENA2 6
#define IN1_2 2
#define IN2_2 7
#define ENB2 10
#define IN3_2 12
#define IN4_2 13

// ======== Motor Speed ========
int baseSpeed = 225; // Adjust between 100–255
int turnSpeed = 195;

// ======== Setup ========
void setup() {
  Serial.begin(9600);

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  // Left motor driver
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Right motor driver
  pinMode(IN1_2, OUTPUT);
  pinMode(IN2_2, OUTPUT);
  pinMode(IN3_2, OUTPUT);
  pinMode(IN4_2, OUTPUT);
  pinMode(ENA2, OUTPUT);
  pinMode(ENB2, OUTPUT);
}

// ======== Motor Control Functions ========
// ======== Motor Control Functions (Fixed Direction) ========
void leftForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void leftBackward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void rightForward() {
  digitalWrite(IN3_2, HIGH);
  digitalWrite(IN4_2, LOW);
  digitalWrite(IN1_2, HIGH);
  digitalWrite(IN2_2, LOW);
}

void rightBackward() {
  digitalWrite(IN3_2, LOW);
  digitalWrite(IN4_2, HIGH);
  digitalWrite(IN1_2, LOW);
  digitalWrite(IN2_2, HIGH);
}

void moveStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN1_2, LOW);
  digitalWrite(IN2_2, LOW);
  digitalWrite(IN3_2, LOW);
  digitalWrite(IN4_2, LOW);
}

// ======== Main Control ========
void loop() {
  int s1 = analogRead(S1);
  int s2 = analogRead(S2);
  int s3 = analogRead(S3);
  int s4 = analogRead(S4);
  int s5 = analogRead(S5);

  // Threshold: adjust depending on your surface (e.g. 500)
  int threshold =500;

  int L2 = s1 > threshold ? 1 : 0;
  int L1 = s2 > threshold ? 1 : 0;
  int C  = s3 > threshold ? 1 : 0;
  int R1 = s4 > threshold ? 1 : 0;
  int R2 = s5 > threshold ? 1 : 0;

  Serial.print(L2); Serial.print(L1); Serial.print(C); Serial.print(R1); Serial.println(R2);

  // Basic Line Following Logic
    // --- Improved Line Following Logic ---
  if (C == 1 && L1 == 0 && R1 == 0 && L2 == 0 && R2 == 0) {
    // Move straight
    leftForward();
    rightForward();
    analogWrite(ENA, baseSpeed);
    analogWrite(ENB, baseSpeed);
    analogWrite(ENA2, baseSpeed);
    analogWrite(ENB2, baseSpeed);
  }
  else if (L1 == 0 && C == 1) {
    // Slight left correction
    rightForward();
    leftForward();
    analogWrite(ENA2, turnSpeed);
    analogWrite(ENB2, turnSpeed);
    analogWrite(ENA, baseSpeed);
    analogWrite(ENB, baseSpeed);
  }
  else if (R1 == 0 && C == 1) {
    // Slight right correction
    leftForward();
    rightForward();
    analogWrite(ENA, baseSpeed);
    analogWrite(ENB, baseSpeed);
    analogWrite(ENA2, turnSpeed);
    analogWrite(ENB2, turnSpeed);

  }
  else if (L2 == 1) {
    // Hard left turn
    leftBackward();
    rightForward();
    analogWrite(ENA, turnSpeed);
    analogWrite(ENB, turnSpeed);
    analogWrite(ENA2, baseSpeed);
    analogWrite(ENB2, baseSpeed);
    //     if (L1 == 1 && C == 0) {
    //     leftForward();
    //     rightForward();
    //     analogWrite(ENA2, turnSpeed);
    //     analogWrite(ENB2, turnSpeed);
    //     analogWrite(ENA, baseSpeed);
    //     analogWrite(ENB, baseSpeed);
    //     delay(300);

    // }
  }
  else if (R2 == 1) {
    // Hard right turn
    leftForward();
    rightBackward();
    analogWrite(ENA, baseSpeed);
    analogWrite(ENB, baseSpeed);
    analogWrite(ENA2, turnSpeed);
    analogWrite(ENB2, turnSpeed);
    //     if (R1 == 1 && C ==0){
    //     rightForward();
    //     leftForward();
    //     analogWrite(ENA, baseSpeed);
    //     analogWrite(ENB, baseSpeed);
    //     analogWrite(ENA2, turnSpeed);
    //     analogWrite(ENB2, turnSpeed);
    //     delay(300);
    // }
  }
  else {
    // Line lost — stop or slow search
    moveStop();
  }
}
