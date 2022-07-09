// LED pin definitions
#define LED_RED     13
#define LED_GREEN   12
#define LED_BLUE    10
#define LED_YELLOW  11

// Correct/Incorrect LEDs
#define CORRECT_LED 6
#define INCORRECT_LED 7

// Button pin definitions
#define BUTTON_RED    2
#define BUTTON_GREEN  3
#define BUTTON_BLUE   5
#define BUTTON_YELLOW 4

// Buzzer definitions
#define BUZZER  9
#define RED_TONE 500
#define GREEN_TONE 1000
#define BLUE_TONE 1500
#define YELLOW_TONE 2000
#define TONE_DURATION 250

// Game Variables
int GAME_SPEED = 250;
int GAME_STATUS = 0;
int const GAME_MAX_SEQUENCE = 50;
int GAME_SEQUENCE[GAME_MAX_SEQUENCE];
int GAME_STEP = 0;
int READ_STEP = 0;

void setup(){
  Serial.begin(9600);
  
  // start random number generator
  randomSeed(analogRead(0));
  
  // initialize all pins for input or output
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(CORRECT_LED, OUTPUT);
  pinMode(INCORRECT_LED, OUTPUT);
}

void loop(){
  
  // In what mode are we?
  // Based on the 4 cases, this swtich
  // determines which part of the game is currently
  // happening
  switch(GAME_STATUS){
    case 0:
      resetGame();
      break;
    case 1:
      playSequence();
      break;
    case 2:
      readSequence();
      break;
    case 3:
      gameOver();
      break;
  }
  
}

void playSequence(){
  // play a step of our sequence
  // loop for the number of game steps that
  // have happened and pick a random LED
  // for the next step
  for(int i=0; i<=GAME_STEP; i++){
    Serial.print("Set LED");
    Serial.println(GAME_SEQUENCE[i]);
    delay(GAME_SPEED*2);
    setLED(GAME_SEQUENCE[i]);
    playTone(GAME_SEQUENCE[i]);
    delay(GAME_SPEED);
    clearLEDs();
    
  }
  // Go to next step: reading buttons
  GAME_STATUS = 2;
}

void readSequence(){
  // read our buttons
  int button_value = readButtons();
  
  if(button_value > 0){
    // a button has been pressed
    if(button_value == GAME_SEQUENCE[READ_STEP]){
      // correct value!
      setLED(button_value);
      // turn on Correct (green) LED
      digitalWrite(CORRECT_LED, HIGH);
      playTone(button_value);
      delay(GAME_SPEED);
      // turn off Correct (green) LED
      digitalWrite(CORRECT_LED, LOW);
      clearLEDs();
      
      // Lets speed it up!
      GAME_SPEED = GAME_SPEED-15;
      
      Serial.println("Correct!");
      
      if(READ_STEP == GAME_STEP){
        // reset read step
        READ_STEP = 0;
        // Go to next game step
        GAME_STEP++;
        // Go to playback sequence mode of our game
        GAME_STATUS = 1;
        Serial.println("Go To Next Step");
        
        // Light all LEDs to indicate next sequence
        setLEDs(true,true,true,true);
        delay(GAME_SPEED);
        setLEDs(false,false,false,false);
        
        
      }else{
        READ_STEP++;
      }
      
      delay(10);
      
    }else{
      // wrong value!
      // Go to game over mode
      GAME_STATUS = 3;
      // Turn on Incorrect (red) LED
      digitalWrite(INCORRECT_LED, HIGH);
      Serial.println("Game Over!");
      tone(BUZZER, 250, TONE_DURATION);
      delay(3000);
      digitalWrite(INCORRECT_LED, LOW);
      // restart the game
      digitalWrite(LED_RED, HIGH);
      tone(BUZZER, RED_TONE, TONE_DURATION);
      delay(TONE_DURATION);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      tone(BUZZER, GREEN_TONE, TONE_DURATION);
      delay(TONE_DURATION);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, HIGH);
      tone(BUZZER, YELLOW_TONE, TONE_DURATION);
      delay(TONE_DURATION);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_BLUE, HIGH);
      tone(BUZZER, BLUE_TONE, TONE_DURATION);
      delay(TONE_DURATION);
      digitalWrite(LED_BLUE, LOW);
      GAME_SPEED = 250;
      GAME_STATUS = 0;
    }
  }
  
  delay(10);
}

void resetGame(){
  // reset steps
  READ_STEP = 0;
  GAME_STEP = 0;
  
  // create random sequence
  for(int i=0; i<GAME_MAX_SEQUENCE; i++){
    GAME_SEQUENCE[i] = random(4) + 1;
  }
  
  // Go to next game state; show led sequence
  GAME_STATUS = 1;
}

void gameOver(){
  // Red RGB
  // Play Pwa Pwa Pwa
  tone(BUZZER, 98, TONE_DURATION);
  delay(TONE_DURATION);
  tone(BUZZER, 93, TONE_DURATION);
  delay(TONE_DURATION);
  tone(BUZZER, 87, TONE_DURATION);
  delay(TONE_DURATION);
  delay(GAME_SPEED);
}

// HELPER FUNCTIONS
// the setLED function turns on an LED based on the number input (1-4)
void setLED(int id){
  switch(id){
    case 0:
      setLEDs(false,false,false,false);
      break;
    case 1:
      setLEDs(true,false,false,false);
      break;
    case 2:
      setLEDs(false,true,false,false);
      break;
    case 3:
      setLEDs(false,false,true,false);
      break;
    case 4:
      setLEDs(false,false,false,true);
      break; 
  }
}

// Play a sound based on which button was pressed
// or which LED is the next step
void playTone(int id){
  switch(id){
    case 0:
      noTone(BUZZER);
      break;
    case 1:
      tone(BUZZER, RED_TONE, TONE_DURATION);
      break;
    case 2:
      tone(BUZZER, GREEN_TONE, TONE_DURATION);
      break;
    case 3:
      tone(BUZZER, BLUE_TONE, TONE_DURATION);
      break;
    case 4:
      tone(BUZZER, YELLOW_TONE, TONE_DURATION);
      break; 
  }
}

// the setLEDs function determines which LED should be lit
void setLEDs(boolean red, boolean green, boolean blue, int yellow ){
  if (red) digitalWrite(LED_RED, HIGH);
  else digitalWrite(LED_RED, LOW);
  if (green) digitalWrite(LED_GREEN, HIGH);
  else digitalWrite(LED_GREEN, LOW);
  if (blue) digitalWrite(LED_BLUE, HIGH);
  else digitalWrite(LED_BLUE, LOW);
  if (yellow) digitalWrite(LED_YELLOW, HIGH);
  else digitalWrite(LED_YELLOW, LOW);
}

// turn off all LEDs
void clearLEDs(){
  setLEDs(false,false,false,false);
}

// Return 1,2,3, or 4 based on which button is pressed
int readButtons(void){
  if (digitalRead(BUTTON_RED) == 0) return 1;
  else if (digitalRead(BUTTON_GREEN) == 0) return 2;
  else if (digitalRead(BUTTON_BLUE) == 0) return 3;
  else if (digitalRead(BUTTON_YELLOW) == 0) return 4;
  
  return 0;
}
