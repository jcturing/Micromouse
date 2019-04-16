#include <Maze.h>
#include <MMouse.h>
#include <SoftwareSerial.h>

/*
    CAMBIOS A IMPLEMENTAR:
    - Probar delay del Newping 29ms y 50ms
*/

//PINES & CONSTANTES
const bool DEBUG = false;
const bool SHOW_VALUES = true;

//DECLARACIONES

SoftwareSerial out(RX, TX);

Maze maze;

MMouse mouse;

void setup() {
  if (DEBUG) out.begin(38400);

  mouse.setDebug(DEBUG);
  mouse.initialize(50);

  attachInterrupt(0, leftEncoderEvent, CHANGE);
  attachInterrupt(1, rightEncoderEvent, CHANGE);

  maze.setDraw(true);
  maze.setDebug(DEBUG);
  maze.setShowValues(SHOW_VALUES);
  maze.initialize();
  maze.print(out);
}

void loop() {
  if (mouse.rightButton.wasPressed()) {
    while(!searchRun()) while(!mouse.rightButton.wasPressed());
    maze.route(out);
    while(true){
      if (mouse.rightButton.wasPressed()) performRun();
      else if (mouse.leftButton.wasPressed()) mouse.incrementRunSpeed(out);
    }
  }
}

//FUNCIONES

bool searchRun() {
  mouse.music.playBeep();
  mouse.rightLed.turnOn();
  mouse.waitForUser();
  if(!makeSearch(true)){
    maze.resetStart();
    return false;
  }
  mouse.waitForUser();
  if (!makeSearch(false)){
    maze.resetStart();
    return false;
  }
  mouse.rightLed.turnOff();
  return true;
}

bool makeSearch(bool middle) {
  bool leftWall = false, centerWall = false, rightWall = false;
  int average, ticksCount = 0;
  bool check_once = false;
  ticksCount = FORWARD_TICKS;
  if (middle) {
    maze.setChoice('F');
  }
  else {
    mouse.motors.setSpeeds(MIN_SPEED + LEFT_MOTOR_FIT, MIN_SPEED);
    maze.flood(middle);
    maze.makeChoice();
  }
  mouse.setLastRightCount(-1);
  mouse.setLastLeftCount(-1);
  mouse.setBackStage(1);
  mouse.setRightStage(1);
  mouse.setLeftStage(1);
  mouse.setLeftCount(0);
  mouse.setRightCount(0);
  maze.flood(middle);

  while (!maze.isGoal()) {
    //Collision Check
    if(mouse.leftButton.isPressed() || mouse.rightButton.isPressed()){
      mouse.music.playBeep();
      mouse.rightLed.turnOff();
      mouse.motors.setSpeeds(0, 0);
      delay(500);
      return false;
    }
        
    //Read Sensors
    mouse.readDistance();
    //Update & Choose
    if (mouse.getLeftCount() >= TURN_CHECK && mouse.getRightCount() >= TURN_CHECK &&  mouse.checkCero() && maze.getChoice() == 'F' && !check_once) {
      mouse.setLeftCount(0);
      mouse.setRightCount(0);
      ticksCount = TURN_PREFORWARD_TICKS;
      check_once = true;
      //mouse.music.playBeep();
    }
    average = (mouse.getLeftCount() + mouse.getRightCount()) / 2;
    if (average > ticksCount) {
      //Update
      check_once = false;
      mouse.setLeftCount(0);
      mouse.setRightCount(0);
      mouse.resetCollision();
      ticksCount = FORWARD_TICKS;
      maze.updatePosition();
      if (mouse.choosePopular(0) != 0) leftWall = true;
      else leftWall = false;
      if (mouse.choosePopular(1) !=  0) centerWall = true;
      else centerWall = false;
      if (mouse.choosePopular(2) !=  0) rightWall = true;
      else rightWall = false;
      maze.updateMap(leftWall, centerWall, rightWall);

      //Choose
      maze.flood(middle);
      maze.makeChoice();
      mouse.setLastRightCount(-1);
      mouse.setLastLeftCount(-1);
      if (maze.getChoice() == 'B') mouse.setBackStage(1);
      else if (maze.getChoice() == 'R') mouse.setRightStage(1);
      else if (maze.getChoice() == 'L') mouse.setLeftStage(1);
      maze.setDraw(true);
    }

    //Write Motors
    mouse.setLeftMotorSpeed(0);
    mouse.setRightMotorSpeed(0);

    if (maze.getChoice() == 'F') mouse.goForward(mouse.getSearchSpeed());
    else if (maze.getChoice() == 'R') {
      mouse.turnRight();
      if (mouse.getRightStage() == -1) {
        maze.updateOri();
        ticksCount = -1;
      }
    }
    else if (maze.getChoice() == 'L') {
      mouse.turnLeft();
      if (mouse.getLeftStage() == -1) {
        maze.updateOri();
        ticksCount = -1;
      }
    }
    else if (maze.getChoice() == 'B') {
      mouse.turnAround();
      if (mouse.getBackStage() == -1) {
        maze.updateOri();
        ticksCount = -1;
      }
    }
    
    if(!maze.isGoal()) mouse.writeMotorSpeed();
        
    //Debug If Necessary
    //maze.print(out);
    //mouse.print(out);
  }
  mouse.motors.setSpeeds(0, 0);
  return true;
}

void performRun() {
  mouse.music.playBeep();
  mouse.rightLed.turnOn();
  mouse.waitForUser();
  for (int i = 0; i < maze.getCommandListTam(); i++) {
    if(i == maze.getCommandListTam()-1) executeCommand(maze.getCommand(i), Command('F', 1));
    else executeCommand(maze.getCommand(i), maze.getCommand(i+1));
  }
  mouse.motors.setSpeeds(0, 0);
  mouse.rightLed.turnOff();
}

void executeCommand(Command com, Command nextCom) {
  int average, ticksCount = com.num * FORWARD_TICKS;
  bool commandDone = false, check_once = false;
  mouse.setLeftCount(0);
  mouse.setRightCount(0);
  mouse.setRightStage(1);
  mouse.setLeftStage(1);

  while (!commandDone) {
    mouse.readDistance();
    mouse.setLeftMotorSpeed(0);
    mouse.setRightMotorSpeed(0);
    if (com.type == 'F') {
      if(mouse.getLeftCount() > (FORWARD_TICKS/2) && mouse.getLeftCount() < ((com.num - 1) * FORWARD_TICKS) && mouse.getRightCount() > (FORWARD_TICKS/2) && mouse.getRightCount() < ((com.num - 1) * FORWARD_TICKS)){
        mouse.goForward(mouse.getRunSpeed());
      }
      else{
        mouse.goForward(mouse.getSearchSpeed());
      }
      
      if (mouse.getLeftCount() >= (((com.num - 1) * FORWARD_TICKS) + TURN_CHECK) && mouse.getRightCount() >= (((com.num - 1) * FORWARD_TICKS) + TURN_CHECK) &&  mouse.checkCero() && !check_once) {
        mouse.setLeftCount(0);
        mouse.setRightCount(0);
        check_once = true;
        ticksCount = TURN_PREFORWARD_TICKS;
        //mouse.music.playBeep();
      }
      average = (mouse.getLeftCount() + mouse.getRightCount()) / 2;
      if (average >= ticksCount) {
        commandDone = true;
      }
    }
    else if (com.type == 'R') {
      mouse.turnRight();
      if(mouse.getRightStage() == -1) commandDone = true;
    }
    else if (com.type == 'L') {
      mouse.turnLeft();
      if (mouse.getLeftStage() == -1) commandDone = true;
    }
    mouse.writeMotorSpeed();
  }
}


void leftEncoderEvent() {
  if (digitalRead(encoder1_A) == HIGH) {
    if (digitalRead(encoder1_B) == LOW) {
      mouse.setLeftCount(mouse.getLeftCount() - 2);
    } else {
      mouse.setLeftCount(mouse.getLeftCount() + 2);
    }
  } else {
    if (digitalRead(encoder1_B) == LOW) {
      mouse.setLeftCount(mouse.getLeftCount() + 2);
    } else {
      mouse.setLeftCount(mouse.getLeftCount() - 2);
    }
  }
}

void rightEncoderEvent() {
  if (digitalRead(encoder2_A) == HIGH) {
    if (digitalRead(encoder2_B) == LOW) {
      mouse.setRightCount(mouse.getRightCount() + 2);
    } else {
      mouse.setRightCount(mouse.getRightCount() - 2);
    }
  } else {
    if (digitalRead(encoder2_B) == LOW) {
      mouse.setRightCount(mouse.getRightCount() - 2);
    } else {
      mouse.setRightCount(mouse.getRightCount() + 2);
    }
  }
}

