#include <Adafruit_PWMServoDriver.h>

#define USMIN  500
#define USMAX  1500
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

const float H[4] = {10, 10, 10, 10}; // height for each legs
const float L = 7.2;
const float l = 8.4;
const int initialAngles[8] = {90, 180, 90, 0, 90, 180, 90, 0};
const int mDelay = 10;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
int angles[8];
float x[4], y[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);

  x[0] = x[1] = x[2] = x[3] = 0;
  y[0] = y[1] = y[2] = y[3] = 0;

  for(int i = 0 ; i < 8 ; i++) {
    pwm.writeMicroseconds(i, map(initialAngles[i], 0, 180, USMIN, USMAX));
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  // up
  for(y[0] = 0, y[3] = 0; y[0] <= 2, y[3] <= 2; y[0]++, y[3]++) {
    IK();
    updatePositions();
    delay(mDelay);
  }
  // right
  for(x[0] = 0, x[3] = 0; x[0] <= 2, x[3] <= 2; x[0]++, x[3]++) {
    IK();
    updatePositions();
    delay(mDelay);
  }
  // down
  for(y[0] = 2, y[3] = 2; y[0] >= 0, y[3] >= 0; y[0]--, y[3]--) {
    IK();
    updatePositions();
    delay(mDelay);
  }
  // left
  for(x[0] = 2, x[3] = 2; x[0] >= 0, x[3] >= 0; x[0]--, x[3]--) {
    IK();
    updatePositions();
    delay(mDelay);
  }
  //
  // Opposite side
  //
  for(y[1] = 0, y[2] = 0; y[1] <= 2, y[2] <= 2; y[1]++, y[2]++) {
    IK();
    updatePositions();
    delay(mDelay);
  }
  // right
  for(x[1] = 0, x[2] = 0; x[1] <= 2, x[2] <= 2; x[1]++, x[2]++) {
    IK();
    updatePositions();
    delay(mDelay);
  }
  // down
  for(y[1] = 2, y[2] = 2; y[1] >= 0, y[2] >= 0; y[1]--, y[2]--) {
    IK();
    updatePositions();
    delay(mDelay);
  }
  // left
  for(x[1] = 2, x[2] = 2; x[1] >= 0, x[2] >= 0; x[1]--, x[2]--) {
    IK();
    updatePositions();
    delay(mDelay);
  }
}

void updatePositions() {
  for(int i = 0 ; i < 8 ; i++) {
    int angle = angles[i];
    int microseconds = map(angle, 0, 180, USMIN, USMAX);
    pwm.writeMicroseconds(i, microseconds);
  }
}

void IK() {

  float a[4], b[4], theta[4], h[4];

  for(int i = 0 ; i < 4 ; i++) {
 
    theta[i] = atan(x[i] / H[i]);
    h[i] =  (H[i] - y[i])/ cos(theta[i]);

    a[i] = acos((pow(L, 2) + pow(h[i], 2) - pow(l, 2)) / (2 * L * h[i]));
    b[i] = acos((pow(L, 2) + pow(l, 2) - pow(h[i], 2)) / (2 * L * l));

  }

  // front right leg --> index = 0
  angles[0] = initialAngles[0] + abs(a[0] * 180 / PI) - (theta[0] * 180 / PI);
  angles[1] = initialAngles[1] - 180 + abs(b[0] * 180 / PI);

  // back left leg --> index = 3
  angles[6] = initialAngles[6] - abs(a[3] * 180 / PI) + (theta[3] * 180 / PI);
  angles[7] = initialAngles[7] + 180 - abs(b[3] * 180 / PI);

  // Opposite side :
  // front left leg --> index = 1
  angles[2] = initialAngles[2] - abs(a[1] * 180 / PI) + (theta[1] * 180 / PI);
  angles[3] = initialAngles[3] + 180 - abs(b[1] * 180 / PI);

  // back right leg --> index = 2
  angles[4] = initialAngles[4] + abs(a[2] * 180 / PI) - (theta[2] * 180 / PI);
  angles[5] = initialAngles[5] - 180 + abs(b[2] * 180 / PI);
}
