import processing.serial.*;

Serial myPort;

String state = "NONE";    // from Arduino
boolean showButton = false;
int buttonX = 100, buttonY = 200, buttonW = 200, buttonH = 60;

void setup() {
  size(600, 400);
  textSize(28);

  // Open COM port
  myPort = new Serial(this, "COM3", 9600);
  myPort.bufferUntil('\n');
}

void draw() {
  background(0);

  fill(255);

  // When object is detected
  if (state.equals("DETECTED")) {
    showButton = false;
    text("here is your butter, sir", 100, 150);
  } 
  else {
    // When object is gone
    text("what is my purpose?", 100, 150);
    showButton = true;
  }

  // Draw button (only when object gone)
  if (showButton) {
    drawButton();
  }
}

void drawButton() {
  fill(50, 150, 255);
  rect(buttonX, buttonY, buttonW, buttonH, 10);

  fill(255);
  textSize(20);
  textAlign(CENTER, CENTER);
  text("to bring me butter", buttonX + buttonW/2, buttonY + buttonH/2);
}

void serialEvent(Serial p) {
  state = trim(p.readString());
}

void mousePressed() {
  if (showButton &&
      mouseX > buttonX &&
      mouseX < buttonX + buttonW &&
      mouseY > buttonY &&
      mouseY < buttonY + buttonH) {

    // Send tantrum command to Arduino
    myPort.write('T');
  }
}
