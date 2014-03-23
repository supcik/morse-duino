#include <PS2Keyboard.h>

const int LED   = 11;
const int AUDIO = 3;
const int FREQ  = 770;
int DIT_UNIT    = 100; // Milliseconds

const int DELTA = 60000;

PS2Keyboard keyboard;
String morse_letter[] = {
//  A       B       C       D       E      F       G
    ".-",   "-...", "-.-.", "-..",  ".",   "..-.", "--.",
//  H       I       J       K       L       M      N
    "....", "..",   ".---", "-.-",  ".-..", "--",  "-.", 
//  O       P       Q       R       S       T      U
    "---",  ".--.", "--.-", ".-.",  "...",  "-",   "..-",
//  V       W       X       Y       Z
    "...-", ".--",  "-..-", "-.--", "--.."

};

String morse_digit[] = {
// 0        1        2        3        4
   "-----", ".----", "..---", "...--", "....-", 
// 5   6        7        8        9     
   ".....", "-....", "--...", "---..", "----."
};

String idle_text = "Hello Fribot";

unsigned long t0;
unsigned long delta = DELTA;

void playDit() {
    Serial.print(".");
    digitalWrite(LED, LOW);
    tone(AUDIO, FREQ);
    delay(DIT_UNIT);
    digitalWrite(LED, HIGH);
    noTone(AUDIO);
    delay(DIT_UNIT);
}

void playDah() {
    Serial.print("-");
    digitalWrite(LED, LOW);
    tone(AUDIO, FREQ);
    delay(3 * DIT_UNIT);
    digitalWrite(LED, HIGH);
    noTone(AUDIO);
    delay(DIT_UNIT);
}

void play(String code) {
    Serial.print("<");
    for (int i = 0; i < code.length(); i++) {
        if (code.charAt(i) == '.') {
            playDit();
        } 
        else {
            playDah();
        }
    }
    Serial.print(">");
    Serial.println();
    delay(2 * DIT_UNIT);
}

void sendMorse (char c) {
    String code;
    t0 = millis();
    if (c == ' ') {
        delay(6 * DIT_UNIT);
    } 
    else if (c >= '0' && c <= '9') {
        code = morse_digit[c - '0']; 
        play(code);
    } 
    else if (c >= 'a' && c <= 'z') {
        code = morse_letter[c - 'a']; 
        play(code);
    } 
    else if (c >= 'A' && c <= 'Z') {
        code = morse_letter[c - 'A']; 
        play(code);
    }
}

void setup() {
    delay(100);
    keyboard.begin(8, 2, PS2Keymap_German);
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    sendMorse('e');
}

void loop() {
    if (keyboard.available()) {
        // read the next key
        delta = DELTA;
        char c = keyboard.read();
        // check for some of the special keys
        if (c == PS2_UPARROW) {
            DIT_UNIT = (DIT_UNIT * 12) / 10;
            sendMorse('e');
        } 
        else if (c == PS2_DOWNARROW) {
            if (DIT_UNIT > 50) {
                DIT_UNIT = (DIT_UNIT * 10) / 12;
            }
            sendMorse('e');
        } 
        else if (c == PS2_ESC) {
            DIT_UNIT = 100;
            sendMorse('e');
        } 
        else if ((c == ' ') || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            Serial.print(c);
            sendMorse(c);
        }
    }
    if (millis() - t0 > delta) {
        delta = DELTA;
        for (int i = 0; i < idle_text.length(); i++) {
            sendMorse(idle_text.charAt(i));
        }        
    }
}

