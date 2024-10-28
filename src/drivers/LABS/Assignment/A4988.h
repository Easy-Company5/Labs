// working A4988.h

#ifndef A4988_H
#define A4988_H

#include "pico/stdlib.h"

// GPIO Pins for all 4 stepper motors with corrected pin mappings

#define STEPS_PER_REVOLUTION 200  // Adjust based on the motor

class A4988Stepper {
public:
    // Constructor: sets up the pins for step, direction, and reset
    A4988Stepper(uint stepPin, uint dirPin, uint resetPin, uint stepsPerRevolution);
    // Enable the motor (bring the motor out of reset)
    void enable();
    // Disable the motor (put the motor in reset mode)
    void disable();
    // Set the direction of the motor
    void setDirection(bool clockwise);
    // Set the speed of the motor in RPM
    void setSpeed(int rpm);
    // Step the motor a specific number of steps
    void step(int steps);
    // Drive the motor forward by a number of steps
    void stepf(int steps);
    // Drive the motor backward by a number of steps
    void stepb(int steps);
    // load sequence of bucket
    void pickUp(A4988Stepper& stepperL, A4988Stepper& stepperR, A4988Stepper& stepperB, A4988Stepper& stepperF);
    // deload sequence of bucket
    void putDown(A4988Stepper& stepperB, A4988Stepper& stepperF);
    // allow character inputs
    void userInput(char input, A4988Stepper& stepperL, A4988Stepper& stepperR, A4988Stepper& stepperB, A4988Stepper& stepperF);
    // Function to pulse the step pin
    void pulseStepPin();
    // for pulse step pin function
    int _delayBetweenSteps; // Delay between steps in microseconds
    // stop motors
    void stopMotor(bool &move);
    // drive forward continuously
    void drive(bool forward);
    // stop motors
    void stop();
    // action motors
    void moveMotors(A4988Stepper& stepperL, A4988Stepper& stepperR);
    // return home
    void homing(A4988Stepper& stepperL, A4988Stepper& stepperR);

private:
    uint _stepPin;
    uint _dirPin;
    uint _resetPin;
    uint _stepsPerRevolution;
    int _stepCounter;
};

#endif
