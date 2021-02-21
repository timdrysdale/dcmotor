# dcmotor
simple library for manipulating dc motor drive signals to compensate for non-idealities


## 0. Introduction

The output drive signal from a controller contains an abstract representation
of the ideal drive input to apply to the plant. There may be aspects of the plant
which are not adequately handled by the controller. Some of these non-idealities
can be ameliorated to a greater or lesser extent by manipulating the drive signal
depending on known properties of the plant, or according to live secondary 
measurements of the plant, e.g. the derivative of the controlled quantity.

## 1. Background

Controller output y ranges from plantMin <= y <= plantMax, where the 
controller output bounds are plant-specific and enforced in the controller.

An PWM drive signal d ranges from -1 <= dMin <= d <= dMax <= +1, where the
drive system may have more narrow constraints than the theoretical 0%-100% 
range of an ideal PWM signal. 

These two domains requiring mapping onto each other. 

## 2. Factors within scope for this library

A realistic motor PWM drive signal may not use the full range, 
and may need adjusting to accommodate non-idealities.   
The full range may not be used (e.g. too little drive may not result in
the plant responding, or too much may over-energise the plant). 
Non-idealities of interest in this library include

 - friction
 - non-linearity in the drive response

Deadbands/backlash are out of scope because they relate to 
how you interpret measured plant output (e.g. if deadbanding, you would zero 
the controller output). 

## 3. Method

Non-linearity can be at least partly compensated by using an interpolation approach to adjust
drive values. Multi-segment interpolation can be implemented as seen [here](https://www.electro-tech-online.com/threads/linear-interpolation-and-lookup-tables-c.147507/). 

Friction can be compensated by adding additional drive to overcome breakaway friction, then removing
this additional drive when the velocity has become non-zero.   Dynamic friction can be compensated by 
adding a constant offset to the signal. This is equivalent to defining
an asymmetric (odd) DC offset for the linearisation curve.

### 3.1 Linearisation

The driver is instantiated with an N segment interpolator which maps the controller output onto the 
drive domain. This can be a simple one-segment interpolator for linear scaling, or have additional
segments if compensating for a non-linear response to the drive signal. 

### 3.2 Static Friction

If friction compensation is specified, a second curve is supplied with (typically) additional drive strength applied in a 
narrow window of velocities around zero velocity. A velocity threshold is supplied as a parameter, and this determines
which curve is selected for interpolation. In this way, a larger range of velocities can be compensated for in the curve, 
with the actual range narrowed down by reducing the threshold - useful for tuning the system.

### 3.3 Dynamic Compensation (not included)

Dynamic friction models can be potentially complex. However, a small amount of testing with a particular plant of interest
shows that a constant offset applied at all speeds, when moving, with the same sign as the drive, is sufficient to reduce or
remove the steady state error for a range of speeds and loads. The positive and negative direction offsets are specified
separately in case of differences between CW and CCW rotation.

## Testing

The googletest framework is used to test this library. In the main directory, issue

```
make
./runTests
```
