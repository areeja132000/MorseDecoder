int baseline = 0;
int filtered = 0;

bool morseTone = 0;
bool previousTone = 0;

unsigned long toneStart = 0;
unsigned long silenceStart = 0;

char morseSymbols[10];
int morseSymbolsIndex = 0;

// -------------------- morse code dictionary and necessary data structure --------------------

struct MorseEntry {
  const char* morseSymbols;
  char letter;
};

const MorseEntry morseTable[] = {
  {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'},
  {".", 'E'}, {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'},
  {"..", 'I'}, {".---", 'J'}, {"-.-", 'K'}, {".-..", 'L'},
  {"--", 'M'}, {"-.", 'N'}, {"---", 'O'}, {".--.", 'P'},
  {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
  {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'},
  {"-.--", 'Y'}, {"--..", 'Z'}
};

// -------------------- thresholds --------------------
const int DOT_MIN = 140;
const int DOT_MAX = 340;

const int DASH_MIN = 620;
const int DASH_MAX = 820;

const int LETTER_MIN = 620;
const int LETTER_MAX = 820;

const int WORD_MIN = 1580;
const int WORD_MAX = 1780;

// -------------------- setup --------------------
void setup() {
  Serial.begin(9600);
  calibrateBaseline();
}

// ------ calibrate baseline for current env -----
void calibrateBaseline() {
  long sum = 0;
  for (int i = 0; i < 1000; i++) {
    sum += analogRead(A0);
    delay(1);
  }
  baseline = sum / 1000;
}

// -------------------- loop (commented out code for determining kHz sampling) --------------------
void loop() {
  //unsigned long startTime = micros();
  filtered = filterInput(readInput());
  morseTone = isSoundAboveThreshold(filtered);
  translateMorse();
  previousTone = morseTone;
  //unsigned long duration = micros() - startTime;
  //Serial.println(duration); // microseconds per sample
}

// -------------------- read input --------------------
int readInput() {
  int raw = analogRead(A0);
  int centered = raw - baseline;
  return abs(centered);
}

// -------------------- filter --------------------
int filterInput(int value) {
  return (filtered * 7 + value) / 8;
}

// -------------------- core state machine --------------------
void translateMorse() {
  if (morseTone == 1 && previousTone == 0) {
    toneStart = millis();
  }

  if (morseTone == 0 && previousTone == 1) {
    silenceStart = millis();
  }

  if (morseTone == 0 && previousTone == 1 && toneStart != 0) {
    unsigned long duration = millis() - toneStart;

    if (duration > DOT_MIN && duration < DOT_MAX) {
      appendChar('.');
    }

    if (duration > DASH_MIN && duration < DASH_MAX) {
      appendChar('-');
    }
  }

  if (morseTone == 1 && previousTone == 0 && silenceStart != 0) {
    unsigned long duration = millis() - silenceStart;
    if (duration > LETTER_MIN && duration < LETTER_MAX) {
      Serial.print(decodeMorse(morseSymbols));
      resetMorseSymbols();
    } else if (duration > WORD_MIN) {
      Serial.print(decodeMorse(morseSymbols));
      Serial.print(" ");
      resetMorseSymbols();
    }
  }
}

// -------------------- decode morse symbols into characters using dictionary --------------------
char decodeMorse(const char* input) {

  int morseTableSize = sizeof(morseTable) / sizeof(morseTable[0]);

  for (int i = 0; i < morseTableSize; i++) {
    if (strcmp(input, morseTable[i].morseSymbols) == 0) {
      return morseTable[i].letter;
    }
  }

  return '?'; // unknown pattern
}

// -------------------- symbol handling --------------------
void appendChar(char c) {
  if (morseSymbolsIndex < 10) {
    morseSymbols[morseSymbolsIndex] = c;
    morseSymbolsIndex++;
    morseSymbols[morseSymbolsIndex] = '\0';
  }
}

// -------------------- reset morse sybols buffer --------------------
void resetMorseSymbols() {
  morseSymbolsIndex = 0;
  morseSymbols[0] = '\0';
}

// -------------------- sound threshold for morse tone --------------------
bool isSoundAboveThreshold(int sound) {
  return sound > 50; 
}
