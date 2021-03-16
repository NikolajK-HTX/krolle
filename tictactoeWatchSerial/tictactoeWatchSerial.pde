// dette program forventer fra Arduino Unoen følgende linje
// xxoox xox \r\n
// De har pladserne:
// 012345678

import processing.serial.*;

StringList myTable;

Serial myPort;

void setup() {
  size(600, 600);
  background(220);
  stroke(50);

  myPort = new Serial(this, Serial.list()[0], 9600);
  
  myTable = new StringList();
  for (int i = 0; i < 9; i++) {
    myTable.append(" ");
  }
}

void draw() {
  updateBoard();
  drawGrid();
  drawBoard();
}

void updateBoard() {
  while (myPort.available() > 0) {
    String inBuffer = myPort.readStringUntil('\n');   
    for (int i = 0; i < 9; i++) {
      // StringList.set(int, String)
      myTable.set(i, str(inBuffer.charAt(i)));
    }
  }
}

void drawGrid() {
  // vertical lines
  line(width / 3, 0, width / 3, height);
  line((width / 3) * 2, 0, (width / 3) * 2, height);
  // horizontal lines
  line(0, height / 3, width, height / 3);
  line(0, (height / 3) * 2, width, (height / 3) * 2);
}

void drawBoard() {
  push();
  stroke(0);
  for (int i = 0; i < 9; i++) {
    String tableValue = myTable.get(i);
    int mx = i % 3 * 200 + 100;
    int my = floor(i / 3) * 200 + 100;
    if (tableValue == "X") drawCross(mx, my);
    if (tableValue == "O") drawCircle(mx, my);
  }
  pop();
}

void drawCircle(int mx, int my) {
  push();
  noFill();
  circle(mx, my, width / 3 * 0.75);
  pop();
}

void drawCross(int mx, int my) {
  push();
  translate(mx, my);
  line(-width / 6 * 0.75, -height / 6 * 0.75, width / 6 * 0.75, height / 6 * 0.75);
  line(-width / 6 * 0.75, height / 6 * 0.75, width / 6 * 0.75, -height / 6 * 0.75);
  pop();
}
