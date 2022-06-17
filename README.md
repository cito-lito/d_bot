# Embedded Systems Project WS 2020 
![d_bot](https://gitlab.cs.hs-rm.de/mmait001/d_bot/-/raw/master/logo.jpg)

##### Demo Videos:

[Without Obstacle](https://youtu.be/d9CEZ9sfACA)

[With Obstacle](https://youtu.be/VZWVzdQHuu4)

## delivery bot

#### Zephyr OS

This project is built using the Zephyr Operating System.
You can find [here](https://docs.zephyrproject.org/latest/application/index.html#application) a basic overview of how to build an Application using this OS.

#### Mobile Robot

This Project is a two-driven wheels robot. This approach is one of the simplest implementations of mobile robots and the Project goal is the more accurate. With two DC Motors, this robot uses
the “differential drive” approach, letting us drive forwards, turn on the spot, and many other drive actions.

#### Map of this Robot

The Map where this robot will operate is a Matrix, where the nodes will be represented with blue colors and black lines will connect these nodes.

#### Line Follower Robot

In order to go drive from node to node, the robot will follow the black lines between them.

#### Microcontroller

The brain of this project is the [ST Nucleo F446RE board](https://www.st.com/en/evaluation-tools/nucleo-f446re.html) 

#### Sensors

*Reflective Optical Sensor:*

For the implementation of a line follower, two of these sensors will be used. They will tell the robot if a black line is detected or not.

*Color Sensor:*

This Robot uses the blue nodes of the Matrix as map points. For the tract of these points, the TCS3200 color sensor will be used.

*Ultrasonic Sonar Sensor:*

The robot should avoid collisions. This Sensor will tell the robot if an Object is in his way.

#### Actuators

*DC Motors:*

They will drive the wheels.

*Motor Drive:*

For the control of the direction and speed of the two DC motors, the dual H-Bridge motor driver L298N will be used.





#### Example: find_node(2,2);
![image](https://user-images.githubusercontent.com/41654186/170017637-d7db2622-509b-4fd1-a0a0-7d81f56b163e.png)





more info: https://wwwvs.cs.hs-rm.de/vs-wiki/index.php/EM2019WSP06
