// Include the Bounce2 library
#include <Bounce2.h>

// Interval for keys
#define INTERVAL 5

#define WIDTH 4
#define HEIGHT 2

#define INITIAL_WAIT 600
#define RAPID_WAIT 50

// Row one
#define KEY_00 10
#define KEY_01 7
#define KEY_02 4
#define KEY_03 2

// Row two
#define KEY_10 14
#define KEY_11 16
#define KEY_12 6
#define KEY_13 3

// Put keys into a 2D array
int key[HEIGHT][WIDTH] = {
  {KEY_00, KEY_01, KEY_02, KEY_03},
  {KEY_10, KEY_11, KEY_12, KEY_13}
};

Bounce deb[HEIGHT][WIDTH];

unsigned long last_up[HEIGHT][WIDTH] = {
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};

bool was_up[HEIGHT][WIDTH] = {
  {false, false, false, false},
  {false, false, false, false}
};

int rapid_amount[HEIGHT][WIDTH] = {
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};

void setup() {
  // Initiate Bounce objects
  // Setup the keys with an internal pull-up
  // and an interval of INTERVAL ms
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      deb[y][x] = Bounce();
      pinMode(key[y][x], INPUT_PULLUP);
      deb[y][x].attach(key[y][x]);
      deb[y][x].interval(INTERVAL);
    }
  }
}

void loop() {
  unsigned long mill = millis();       // Take the current time (this isn't needed that much as the time differences are super small)
  for (int y = 0; y < HEIGHT; y++) {   // Loop through the rows
    for (int x = 0; x < WIDTH; x++) {  // Loop through the columns
      deb[y][x].update();              // For each key update and read the state of the key
      int val = deb[y][x].read();      // so that we know whether it is pressed down or up
      if (val == HIGH) {               // [1] If the key is not pressed
        if (!was_up[y][x]) {           // Check if it was already pressed (was suddenly let go)
          rapid_amount[y][x] = 0;      // Reset the value of the amount of times the key was rapidly sent to 0
          was_up[y][x] = true;         // Set the was_up value to true so that we know that it wasn't 
        }                              // pressed down in the previous time this loop was run
      } else {                         // [1] else if the key was pressed
        if (was_up[y][x]) {            // [2] Check if it wasn't already pressed (suddenly pressed)
          last_up[y][x] = mill;        // Set the last time the key was up to now
          was_up[y][x] = false;        // Set the was_up value to false as it is now being pressed down
          key_action(y, x);            // Run the key action associated with this key as it was just pressed down
        }else if (mill - last_up[y][x] >= INITIAL_WAIT + RAPID_WAIT * rapid_amount[y][x]) {
                                       // [2] else check whether the difference between now and the last time the key was up is greater than the initial wait
                                       // time plus the rapid wait time multiplied by how many times it has been sent. This makes the program wait
          key_action(y, x);            // the initial wait send the action, then send the action on the interval of the rapid wait time
          rapid_amount[y][x]++;        // increase the rapid amount so that we wait another rapid wait time
        }
      }
    }
  }
}

int key_action(int y, int x) { // TODO: Better way to do this?
  switch (y) {
    case 0:
      switch (x) {
        case 0:
          Serial.println(millis());
          break;
        case 1:
          Serial.println("01");
          break;
        case 2:
          Serial.println("02");
          break;
        case 3:
          Serial.println("03");
          break;
        default:
          return 0;
      }
      break;
    case 1:
      switch (x) {
        case 0:
          Serial.println("10");
          break;
        case 1:
          Serial.println("11");
          break;
        case 2:
          Serial.println("12");
          break;
        case 3:
          Serial.println("13");
          break;
        default:
          return 0;
      }
      break;
    default:
      return 0;
  }
  return 1;
}


