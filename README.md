<p>
<img src = "https://github.com/jcturing/Micromouse/blob/master/Multimedia/WhatsApp%20Image%202019-04-16%20at%2021.04.10%20(1).jpeg" align = "right" width = "100"/>
</p>

# Micromouse
Everything about my Micromouse Robot.

## Concept
This Micromouse robot was a project for a competition ([Cybertech](https://www.reset.etsii.upm.es/cybertech/cybertech2019/)) in the UPM in Madrid.
It basically has to get to the middle of a 8x8 or 16x16 maze as fast as possible.

## Description
The mouse has ultrasonic distance sensors to detect walls and encoders to know in which cell we are. It works with a **Flood-Fill** algorithm so it guesses
the way to the centre once it knows the maze.

## Prerequisites
### Hardware
Here I list all the component that the robot contains:
* Arduino Uno
* 30:1 Pololu Motors
* Battery
* Pololu Magnetic Encoders
* HC-SR04 Distance Sensors
* Buzzer, Led, Buttons, Ball and others...

### Software
The code provided in this repository uses the **Maze** and **MMouse** library. You can download them [here](https://github.com/jcturing/Arduino-Libraries).
These libraries also depends on others so you may have to download the rest of them.
In the Maze library, the maze is represented and the dimension and target cells of the maze can be changed. Here we run the Flood-Fill algorithm for the 
decision making and we also route the shortest path to the target.
In the MMouse library, every sensor and hardware is declarated. Also it contains functions to move as goForward, turnRight, turnLeft and turnAround.

## Contributing
I am aware that I can improve this robot a lot. My will is to create one like the profesional ones by myself.
This is my first try in Micromouse and more advanced version might come later on.
I built the robot with the hardware I knew some time ago and right now I realize that there are other sensors and components that would suit much better.
Here is a list of component that I would like to incorporate in the next version:
* Gyroscope: Helps to calculate better turns
* Higher resolution encoders: Helps to calculate moves.
* Smaller microcontroller. Might be Arduino Nano or some kind of STM. Faster and trying to make the robot smaller.
* IR led sensors or Laser distance sensors: Ultrasonic sensors are really slow and not very accurate.

Also, I have some ideas in mind that I would want to make:
* Avoid 3D printing and make the PCB as the entire chasis
* Motors "holds" chasis so there is no need of the ball

However, I tried to do my best with what I had. If you want to contribute with this project any help is welcome!

## Acknowledgments
I want to thank to all the people that shares their knowledge about Micromouse online. Specially:
* Peter Harrison: He is a profesional and real kindful. Here is his [Website](http://www.micromouseonline.com/) and his [Youtube Channel](https://www.youtube.com/channel/UCFOicGTdIc8PUo3OuTxQ_pw)
* The guy from [Project Futura](http://micromouseusa.com/?page_id=1342). This web has very handful information about the designing and building.
