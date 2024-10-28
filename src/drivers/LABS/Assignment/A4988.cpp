// working A4998.cpp code

#include "A4988.h"
#include <stdio.h>
#include "GPIO_PINS.h"

// logic flags for movement
bool moveL = false;
bool moveR = false;
bool moveF = false;
bool moveB = false;

// Constructor: initializes the GPIO pins
A4988Stepper::A4988Stepper(uint stepPin, uint dirPin, uint resetPin, uint stepsPerRevolution)
    : _stepPin(stepPin), _dirPin(dirPin), _resetPin(resetPin), _stepsPerRevolution(stepsPerRevolution), _stepCounter(0) {
    
    // Set the GPIO pins as outputs
    gpio_init(_stepPin);
    gpio_set_dir(_stepPin, GPIO_OUT);
    
    gpio_init(_dirPin);
    gpio_set_dir(_dirPin, GPIO_OUT);

    gpio_init(_resetPin);
    gpio_set_dir(_resetPin, GPIO_OUT);

    // Disable the motor (reset state by default)
    disable();
}

// Enable the motor (bring it out of reset)
void A4988Stepper::enable() {
    gpio_put(_resetPin, 1);  // Set the reset pin to high to enable the motor
}

// Disable the motor (put it in reset)
void A4988Stepper::disable() {
    gpio_put(_resetPin, 0);  // Set the reset pin to low to disable the motor
}

// Set the direction of the motor
void A4988Stepper::setDirection(bool clockwise) {
    // store direction
    gpio_put(_dirPin, clockwise ? 1 : 0);  // Set direction pin
}

// Set the speed of the motor in RPM
void A4988Stepper::setSpeed(int rpm) {
    // Calculate delay between steps in microseconds
    _delayBetweenSteps = (60L * 1000000L) / (rpm * _stepsPerRevolution);
}

// Step the motor a specific number of steps
void A4988Stepper::step(int steps) {
    for (int i = 0; i < steps; i++) {
        pulseStepPin();
        sleep_us(_delayBetweenSteps);  // Wait between steps
    }
}

// Pulse the step pin (used to signal the motor driver to step)
void A4988Stepper::pulseStepPin() {
    gpio_put(_stepPin, 1);  // Set step pin high
    sleep_us(1);            // Short delay for the pulse
    gpio_put(_stepPin, 0);  // Set step pin low

    if(gpio_get(_dirPin) == 1){
        _stepCounter++;
    }
    else{
        _stepCounter--;
    }   
}

// Drive the motor forward by a number of steps
void A4988Stepper::stepf(int steps) {
    setDirection(true); // Set direction to backward
    step(steps);
}

// Drive the motor backward by a number of steps
void A4988Stepper::stepb(int steps) {
    setDirection(false); // Set direction to backward
    step(steps);         // Step the motor
}

// Drive the motor forward indefinitely (toggle with the flag)
void A4988Stepper::drive(bool forward) {
    setDirection(forward);  // Set direction based on the boolean value
    pulseStepPin();
    sleep_us(_delayBetweenSteps);  // Delay between steps
}

// Stop the motor
void A4988Stepper::stop() {
    moveL = false;  // Set movement flag to false to stop the loop
    moveR = false;
    moveF = false;
    moveB = false;
}

// keyboard control
void A4988Stepper::userInput(char input, A4988Stepper& stepperL, A4988Stepper& stepperR, A4988Stepper& stepperB, A4988Stepper& stepperF) {
    switch(input){
        case 'w': // drive forward
                moveF = !moveF;
            break;

        case 's': // drive backward
                moveB = !moveB;
            break;

        case 'a': // drive left
                moveL = !moveL;;
            break;

        case 'd': // drive right
                moveR = !moveR;
            break;     

        case 'e': // pick up payload
                stepperB.pickUp(stepperL, stepperR, stepperB, stepperF); 
            break;
        
        case 'q': // release payload
                stepperB.putDown(stepperB, stepperF); 
            break;
        
        case 'x': // stop all motors
            stop();

        default:
            printf("Invalid input\n");
            break;;
    }
}

// action motors
void A4988Stepper::moveMotors(A4988Stepper& stepperL, A4988Stepper& stepperR){
    if(moveF){
        stepperL.drive(false);
        stepperR.drive(true);
    }
    else if(moveB){
        stepperL.drive(true);
        stepperR.drive(false);
    }
    else if(moveL){
        stepperL.drive(true);
        stepperR.drive(true);
    }
    else if(moveR){
        stepperL.drive(false);
        stepperR.drive(false);
    }
    
    // printf("Left Motor Steps: %d, Right Motor Steps: %d\n", stepperL._stepCounter, stepperR._stepCounter);

}

// pick up payload
void A4988Stepper::pickUp(A4988Stepper& stepperL, A4988Stepper& stepperR, A4988Stepper& stepperB, A4988Stepper& stepperF ){
        // Step 1: Move stepperB back 200 steps
    stepperB.stepb(520);

    // Step 2: Move stepperF forward 100 steps
    stepperF.stepf(200);

    // Step 3: Move stepperB back 100 steps
    stepperB.stepb(300);

    // Step 4: Move stepperF forward 100 steps
    stepperF.stepf(300);

    // Step 5: Move stepperL and stepperR forward simultaneously 200 steps
    stepperL.setDirection(false); // Set direction to forward for stepperL
    stepperR.setDirection(true); // Set direction to forward for stepperR
    for (int i = 0; i < 200; i++) {
        stepperL.pulseStepPin(); // Step stepperL forward 1 step
        stepperR.pulseStepPin(); // Step stepperR forward 1 step
        sleep_us(stepperL._delayBetweenSteps); // Wait between steps
    }

    // Step 6: Move stepperB forward 200 steps and stepperF backward 200 steps simultaneously
    stepperB.setDirection(true);  // Set direction to forward for stepperB
    stepperF.setDirection(false); // Set direction to backward for stepperF
    for (int i = 0; i < 400; i++) {
        stepperB.pulseStepPin();  // Step stepperB forward 1 step
        stepperF.pulseStepPin();  // Step stepperF backward 1 step
        sleep_us(stepperB._delayBetweenSteps); // Wait between steps
    }

    // Step 7: Move stepperB forward 100 steps
    stepperB.stepf(420);
    stepperF.stepb(100);
}

// drop payload
void A4988Stepper::putDown(A4988Stepper& stepperB, A4988Stepper& stepperF){
    // Step 1: Move forearm to full extension
    stepperF.stepb(380);
    // Step 2: move bicep to full extension
    stepperB.stepb(750);
    // Step 3: shake 1

    stepperF.setSpeed(200);
    stepperF.stepf(100);
    stepperF.stepb(100);
    stepperF.stepf(100);
    stepperF.stepb(100);
    stepperF.stepf(100);
    stepperF.stepb(100);
    stepperF.stepf(100);
    stepperF.stepb(100);
    stepperF.stepf(100);
    stepperF.stepb(100);
    stepperF.stepf(100);
    stepperF.stepb(100);
    // Step 4: homing
    stepperF.setSpeed(60);
    stepperF.stepf(380);
    stepperB.stepf(750);
}

// return home
void A4988Stepper::homing(A4988Stepper& stepperL, A4988Stepper& stepperR){
    // Drive motor until step counter reaches 0
    while (_stepCounter != 0) {
        if (_stepCounter > 0) {
            setDirection(false);  // Move backward
        } else if (_stepCounter < 0) {
            setDirection(true);   // Move forward
        }
        // decrement counters
        pulseStepPin();

        sleep_us(_delayBetweenSteps);  // Delay between steps
    }
}

