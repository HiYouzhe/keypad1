// --- 引腳定義 ---
#define SEG_A 38
#define SEG_B 39
#define SEG_C 40
#define SEG_D 41
#define SEG_E 42
#define SEG_F 43
#define SEG_G 44

#define COM1 30
#define COM2 31
#define COM3 32
#define COM4 33

// 鍵盤引腳
#define COL1 22
#define COL2 23
#define COL3 24
#define COL4 25
#define ROW1 26
#define ROW2 27
#define ROW3 28
#define ROW4 29

// 共陽極段碼表 (0-F)
const byte digits[] = {
  0b11000000, 0b11111001, 0b10100100, 0b10110000,
  0b10011001, 0b10010010, 0b10000010, 0b11111000,
  0b10000000, 0b10010000, 0b10001000, 0b10000011,
  0b11000110, 0b10100001, 0b10000110, 0b10001110
};

const byte segPins[] = {SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G};
const byte comPins[] = {COM1, COM2, COM3, COM4};
const byte colPins[] = {COL1, COL2, COL3, COL4};
const byte rowPins[] = {ROW1, ROW2, ROW3, ROW4};

int displayBuffer[4] = {-1, -1, -1, -1};
bool lastKeyState = false;

void setup() {
  for (int i = 0; i < 7; i++) pinMode(segPins[i], OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(comPins[i], OUTPUT);
    digitalWrite(comPins[i], HIGH); // PNP 熄滅
    pinMode(rowPins[i], INPUT_PULLUP); // 設為輸入上拉
    pinMode(colPins[i], OUTPUT);
    digitalWrite(colPins[i], HIGH);
  }
}

void loop() {
  int pressedKey = scanKeypad();

  if (pressedKey != -1) {
    if (!lastKeyState) { // 簡單的按壓觸發邏輯
      displayBuffer[0] = displayBuffer[1];
      displayBuffer[1] = displayBuffer[2];
      displayBuffer[2] = displayBuffer[3];
      displayBuffer[3] = pressedKey;
      lastKeyState = true;
      delay(50); // 去彈跳
    }
  } else {
    lastKeyState = false;
  }

  refreshDisplay();
}

// 手動掃描鍵盤函式
int scanKeypad() {
  char keyMap[4][4] = {
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {8, 9, 10, 11}, // 10=A, 11=B
    {12, 13, 14, 15} // 12=C, 13=D, 14=E, 15=F
  };

  for (int c = 0; c < 4; c++) {
    digitalWrite(colPins[c], LOW); // 拉低目前的列
    for (int r = 0; r < 4; r++) {
      if (digitalRead(rowPins[r]) == LOW) { // 偵測到按下
        digitalWrite(colPins[c], HIGH);
        return keyMap[r][c];
      }
    }
    digitalWrite(colPins[c], HIGH); // 恢復高電位
  }
  return -1;
}

void refreshDisplay() {
  for (int i = 0; i < 4; i++) {
    if (displayBuffer[i] != -1) {
      for (int j = 0; j < 4; j++) digitalWrite(comPins[j], HIGH);
      byte pattern = digits[displayBuffer[i]];
      for (int s = 0; s < 7; s++) digitalWrite(segPins[s], bitRead(pattern, s));
      digitalWrite(comPins[i], LOW);
      delay(2);
    }
  }
}
