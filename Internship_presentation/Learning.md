1. 15 may 2024

    Meeting on making Test cases for VCU:
    1. DFMA: Design for manufacturability and assembly
        * Engineering methodology focused on designing products in a way that makes them easy and cost effective to manufacture and assemble.
    2. DFMEA: Design Failure Mode and Effects Analysis.
        * systematic methodology used to identify potential failure modes in a product design.Assess the effects and causes of these failures, and prioritize actions to mitigate the risks associated with them.
        1. Failure Modes:
            Definition: The specific ways in which a design can fail to meet its intended function.





what is a black box?
Black box is a component(inputs and get outputs) and other environmental inputs and other ouptuts like heat and wastage, leakage(to refine this we will write DFMEA)
ISO: 26262 (how to design component for automotive)
D356 for aerospace




Task:
1. Issue in the **Boost bar**




Meeting- 7.17 pm 

Custom mode: USP of the vehicle (user can play around with the power train, like how the vehicle should behave, eg: coorg trip - coorg mode(custom)- **modified with the help of the application**)

**1. Throttle map (gives the IC engine feeling)- throttle percentage vs torque**

**2. power map- for achieving the efficiency(by reducing the current and making the same speed)-usually caused due to the inertia(in the beginning the motor needs more energy to run the motor and once the motor started rotating the inertia is there,then rotation of motor is bit easy)**


**how to tell the controller the particular mode it is ?**
1. Torque percentage
2. current (what is the current for this torque percentage)
3. certain freq(rpm) for each mode (power - 105 amps)
4. Acceleration



**IMU- accelerometer + gyroscope (angular displacement)**


MCU contains two ways of control
1. speed mode control
2. torque mode control




**16 May 2024**

To do:
**1. Add RPM value through HIL (km/h) - RPM to km/hr is done by multiplying RPM with 0.01606**
**2. Add warnings to HIL like cellUnderVoltageWarning**


Blocker-----------> can't able to make RPM value available in VCU.
now, I am running the python code to share values through UART- so i cant able to see the debug statements which i am placing in c-------> I have to find a way


**Errors and warnings**

1. Battery
Prot - safe guarding( **user dont have to be notified**- because prot - will cause the vehicle to turn off (cutoff))
warning- warning the user.
Motor cutoff won't make the vehicle off always (it will stop the wheeel rotationg, but VCU will be on , so the warning can be shown)



May 20
1. Engineering point of Custom mode (what is custom mode)
2. what is custom mode (in point of the user)
3. Fail safe(CAN LOSS)



**WOT**
    WOT stands for Wide Open Throttle
    this condition will happen when **the accelerator pedal is pressed fully (or) throttle is applied fully.**- allowing the max amount air-fuel mixture to enter into the combustion chamber.
    Used to understand the limits of the engine(to evaluate the engine's max power output and overall performance) and drivetrain components.




May 21
Powertrain: Components that generate and delivers power to the wheels.
Drivetrain: Components that deliver power from the motor to the wheels(like belts,chains, shafts)


**Lock State:** It is the state in which vehicle is completely turned off(BMS WONT DISCHARGE)
**Wake-up State:** It is the state in which BMS starts discharging and MCU 
gets energized.
**Ride State:** It is the state in which throttle is active and mode change is 
accessible.
**Reverse State**: It is the state in which motor-controller activates the 
reverse mode.
**Protection State**:It is a detailed state that contains logic to protect the 
system.


May 27
1. NTP - Network Time Protocol
   *NTP ensures that hosts on a TCP/IP network maintain consistent timekeeping.  




