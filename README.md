# Project Eva Telemedicine Robot
 

## Abstract
Telemedicine or Telehealth is the distribution of health-related services and information via 
electronic information and telecommunication technologies. It allows long-distance patient and 
clinician contact, care, advice, reminders, education, intervention, monitoring, and remote 
admissions. In this project we aim to build an easy to use, yet intelligent Robot assistant that will 
serve as a RC gateway between long distance patients and their families, doctors, loved ones
providing the patient with care, monitoring, & Telemedicine. The robot itself is a 3-wheel
differential drive robot built on a 3D printed chassis and a STM32F407VET microcontroller 
Featuring an Arm-Cortex-M4 168[MHz] Core. The robot is equipped with a mobile device 
(Tablet) to enable video conferencing. anyone with permissions can call the robot and remote 
control it from far away to monitor the patient/family member. The robot built in this project 
fully occupies these features, calling the robot from far away and driving in the patient’s home 
to firstly check on him and secondly check on his health and satisfaction, our conclusions from 
this robot are, firstly this concept could be implemented into a real life product with many use 
cases, secondly it is a product customers would like to use but don’t have the will to purchase 
because cheaper solutions can bring the same results at zero investment like using a 
smartphone to video conference, Etc.


## Project overview
Project EVA is a combination of 2 systems, an embedded real-time Robot system & an IoT 
software agent. the Robot system is independently handled with a microcontroller equipped 
with sensors, power distribution & management components, a camera, and an intelligent Real￾time software, whereas the IoT agent is built upon 2 applications running on different mobile 
devices connected via a firebase. The Robotic system receives commands from the IoT software 
agent via a Bluetooth wireless communication protocol.
