# BreakerBot – Autonomous Breaker Alignment Robot

Boston University Senior Capstone Project
College of Engineering, ECE Department & ME Department

[Final Deliverable](https://www.youtube.com/watch?v=yyhNmTSDY2Q)

#### Contributors

- Daniel Gorelick - [dqgorelick](https://github.com/dqgorelick)
- Alex Wong – [AlexDWong](https://github.com/AlexDWong)
- Pete Benja – [kbenja](https://github.com/kbenja)
- Paul Gennaro – [pgennaro](https://github.com/pgennaro)
- Neeraj Basu – EE
- Emily Stern – ME
- Carolyn Nicolo – ME
- David Miller – ME

BreakerBot's software consists of two main stacks:
- Main Control Program (C++)
- User Interface (Node.js / Javascript)

The relationship between the two stacks and the hardware can be seen in the system **UML diagram** located under [Other Materials](https://github.com/BostonUniversitySeniorDesign/Breaker-bot/tree/master/2-Software/other%20material).

## Control Program
The control program is the C++ code which controls the hardware on the robot. The program is built in a modular, object-oriented programming framework. You should first familiarize yourself with the breakerbot.cpp program to understand the flow of the program, and then delve further into the different modules to understand how the program works as a whole.

#### Building and Compiling
The only way to compile the code is to do so on the Intel Edison itself (this is due to the hardware library being used). The code can either be compiled using the Makefile, or by using Eclipse to compile by SSHing onto the Edison. For those comfortable with the command-line, compiling using the Makefile allows for **much more rapid development**. Code can be transferred to the Intel Edison using github. **Take advantage of github**.

Both these methods require that you have already set up the Intel Edison and have connected it either to your computer with a MicroUSB cable (the Eclipse method) or by SSH over a network (the SSH method). For instructions on connecting or setting up your Intel Edison, please follow the official Intel Edison instructions below.

* [Setting up the Edison and Arduino Breakout Board](https://software.intel.com/en-us/get-started-edison-windows)
* [Programming the Edison using the Eclipse IDE](https://software.intel.com/en-us/intel-system-studio-iot-edition-guide-for-c)

##### How to build using SSH
Using the command line, navigate into the directory where the Makefile is located. If there is an existing version, either move it or remove it. When ready to create the program, run the `make` command. There are various make commands to make different executables, i.e. `make stop` to create the stopping executable which is called in the event where the main program fails.

##### How to build using Eclipse
  * Ensure your copy of the code is up to date with the master GitHub branch
  * Make sure Edison IoT Eclipse SDK is installed properly
  * Open Eclipse, set up new IoT C++ project in root-directory/control_program
  * If it does not work make sure to erase any files that show up when you do a `git status` that are either new or red.
  * After creating a new project make sure to do `git stash` since Eclipse will over-write the breakerbot.cpp file with its template version.
  * Refresh all files in eclipse, and build to see if it works
  * To run make sure you have the correct IP setup, and it should work!

#### Modes
- Manual: The operator controls the robot using the Gamepad via the web application user interface
- Auto: The robot autonomously aligns with the mock circuit breaker using sensory data
- Intake: Operator either loads or unloads the mock circuit breaker from the cabinet
- Test: Runs through various hardware tests. Highly recommended after major changes have been made.

Almost all hardware elements have their own module. The heirarchy is as follows:
```
|–– Makefile
|–– bin
|   |–– main
|   |–– pots
|   |–– stop
|   |–– swerve
|–– src/breakerbot.cpp
    |–– include
        |–– drive_module
        |   |–– swerve_module x 4
        |       |–– motor_module (driving motor)
        |       |–– motor_module (swerve motor)
        |       |–– pot_module (potentiometer)
        |       |–– pot_module (potentiometer)
        |–– intake_module
        |–– alignment_module
        |   |–– navx_module
        |   |–– lidar_module
        |–– ipc_module
        |–– i2c_library
        |–– test_module
```

##### Main program (breakerbot.cpp)
Program which contains the main while loop which runs the robot. All of the other files are initialized from this program. The main loop is a non-blocking while loops with a cycle-time of 50ms. The start of the loop is where the data is sent and receieved from the Node.js server. The next part of the loop depends on which mode the operator has choses (refer to Modes section above).

##### Drive Module (drive_module.h)
Module which controls the four swerve modules. This module contains the logic needed to communicate between all of the swerve modules. The function takes an argument for the direction and speed.

##### Swerve Module (swerve_module.h)
Four swerve modules control the swerve drive for the robot. Each swerve module has a swerve motor (controls the orientation of the wheel), and a drive motor (controls the movement of the wheel). Each swerve module also has a potentiometer module attached, so that the module will know where the wheel is orientated. Each swerve module will rotate to the correct position, wait for the OK from the drive module, and then will drive until it receives a break command.

##### Motor Module (motor_module.h)
Motor module can either be used for the drive motor, swerve motor, or intake module (polymorphism should be implemented in the future). The motor module takes a signal which is sent to the i2c chip and converted into a PWM to drive the motor at a specific speed (refer to the hardware readme).

##### Potentiometer Module (pot_module.h)
The potentiometer is able to measure the rotation of the wheel which it is attached to. This rotation reading is used in the swerve module as a form of feedback for the rotation of the wheel.

##### Intake Module (intake_module.h)
The intake module is used once the robot is aligned with the breaker. The intake module receieved input from the game controller to control the singular motor located in the back of the robot.

##### Alignment Module (alignment_module.h)
This module contains the autonomous alignment algorithm (which is currently in progress). The module will take inputs from the NavX, the Lidar, and the image processing which happens on the frontend. The module should return the inputs which should be sent to the drive module, and in turn align the robot.

##### NavX Module (navx_module.h)
The NavX chip is a specialized hardware module which has a very accurate gyro-accelerometer. This module is able to keep track of the robot (with slight drifting), so that the alignment algorithm will always maintain a base orientation. The output of the NavX chip can be ready via i2c.

##### Lidar Module (lidar_module.h)
This module is used in the alignment algorithm to measure the distance the robot is away from the circuit breaker. The lidar outputs an analog value which can be read by the ADC port on the Intel Edison.

##### IPC Module (ipc_module.h)
The inter process communication module is used to send and receieve data to the Node.js server usign a unix socket. The socket is currently blocking, but can be set to non-blocking mode (works half the time).

##### I2C Library (i2c_library.h)
This library of functions is used in many of the hardware components. The i2c bus must first be initialized on the Intel Edison before operating.

##### Test Module (test_module.h)
The test module contains all of the individual hardware tests which can be run to make sure each component is performing properly.

## User Interface
The user interface is in the form of a web application. The web application stack uses a Node.js backend server. This server has a TCP/IP socket to the frontend, which will be the single operator. The operator will use a gamepad controller to send commands to the robot. Additionally, the user interface offers the ability and send the signal to emergency stop the robot.

Some dependencies required to run the web application:
- Node (version 4+)
- FFMPEG video library

Technologies used:
- Gulp (task runner)
- Express.js
- SASS styling
- tracking.js

You must make sure to have all of the necessary dependencies are installed before you will be able to run the Node server. Make sure node is properly installed in the computer. Once it is installed, navigate to the directory where the package.json is located. To install all of the node modules, run `npm install` and wait. When running the program, you can run `node server.js` to start the web application. If you are doing development on the server, you can run `gulp` which will start the Gulp task runner. This task runner handles all of the sass compilations, as well as nodemon (server re-initializing).

The frontend web application uses Angular.js as the frontend framework. The advantages to using angular is that the data being sent from the control program will be able to be continuously updated by using the two-way data-binding between the controller and view.

Controllers:
- DataCtrl.js `// displays data receieved from the Intel Edison on the lefthand panel on the UI`
- GamepadCtrl.js `// handles all input from the gamepad in a 60fps cycle`
- LoggingCtrl.js  `// logs all relevant info to the HUD on the video stream`
- VideoCtrl.js  `// handles the video rendering and the image tracking`

Services:
- CommunicationCtrl.js  `// used to talk between the frontend and the Node.js server in the form of a TCP/IP socket`
- LoggingService.js `// used to talk between controllers, to show status of the robot

Other:
- loopManager.js `// used to read the gamepad controller in a 60fps loop`
- XMLHTTPRequest.js `// used to make GET requests to the Node server`

The Angular app is created in the `app.js` file (where more single-use controllers are located).

Server:
The server file is aptly named server.js. This file will (should) run upon the powering on of the Intel Edison, but if not then you can navigate to the folder and run `node server.js`.

The Node.js server is the connection between the user interface and the control program. There is a TCP/IP socket which handles the communication between the server and the user interface. The communication between the server and the control program (C++) is handled by a Unix socket. Please refer to the comments in the code to understand the inner workings of the server.

## Running the entire system
My normal setup to run the program is to have two SSH clients open, one running the Node.js server and one running the C++ control program. Start the server by writing `node server.js`. Once the node server is running you can navigate to the IP listed at port 8080 to see the user interface. It is advised to plug in the gamepad controller and make sure the buttons are being registered on the Node server before running the control program.

The server will repeatedly print "UNIX SOCKET NOT CONNECTED" as it waits for the control program to connect. In the other SSH window run the control program by running `./main` in the control program directory. Once the control program is running, the Node server should print "UNIX SOCKET CONNECTED". The program should be ready to go at this point. If the user interface has chosen the manual mode, then the robot will listen for the commands being sent through the gamepad controller. Please refer to the user manual for more information on how to use the gamepad.
