/**
   Use Arduino and IR receiver sensor to accept TV remote signal and send
   keyboard instruction to host computer to simulate keystoke, hence, instruct
   host computer to do something - content navigation using Chome TV mode.

    # This project requires the following libaries
   - IRemote v3.1, https://github.com/Arduino-IRremote/Arduino-IRremote
    - HID-project v2.8, https://github.com/NicoHood/HID
    The above libraries can be installed from Tools->Library Manager

    # Reference
    - https://uboopenfactory.univ-brest.fr/Les-Labs/MusicLab/Projets/Arduino-Media-Keys

   # Author
   Nick Jiang <congtao.jiang@outlook.com>
   2021/04/27

*/
#include <IRremote.h>
#include <HID-Project.h>
#include <HID-Settings.h>
//#include <Keyboard.h>  // implicitly include

// Test Remote - IR commands, depends on TV remote
/*
const uint16_t CMD_KEY_DOWN_ARROW = 0x16;
const uint16_t CMD_KEY_UP_ARROW = 0x44;
const uint16_t CMD_KEY_RIGHT_ARROW = 0x09;
const uint16_t CMD_KEY_LEFT_ARROW = 0x07;
const uint16_t CMD_MEDIA_VOLUME_UP = 0x40;
const uint16_t CMD_MEDIA_VOLUME_DOWN = 0x19;
const uint16_t CMD_KEY_RETURN = 0x15;
const uint16_t CMD_MEDIA_PLAY_PAUSE = 0xD;
const uint16_t CMD_KEY_ESC = 0x43;
*/

// Digtial Frame Remote Control - IR commands
const uint16_t CMD_KEY_DOWN_ARROW = 0x2;
const uint16_t CMD_KEY_UP_ARROW = 0xA;
const uint16_t CMD_KEY_RIGHT_ARROW = 0x7;
const uint16_t CMD_KEY_LEFT_ARROW = 0x5;
const uint16_t CMD_MEDIA_VOLUME_UP = 0x10;
const uint16_t CMD_MEDIA_VOLUME_DOWN = 0xC;
const uint16_t CMD_KEY_RETURN = 0x6;
const uint16_t CMD_MEDIA_PLAY_PAUSE = 0x9;
const uint16_t CMD_KEY_ESC = 0xB;

bool is_cmd_triggered(bool triggered, const char * message) {
  if (triggered) {
    Serial.println(message);
  }
  return triggered;
}

//typedef struct _ActionTriggerEntry {
//  uint16_t command;
//  char kbKeyCode;
//  uint8_t mediaKeyCode;
//  uint8_t  mediaKeyDelayms;
//  const char * message;
//} ActionTriggerEntry;
//
//const ActionTriggerEntry actions[] = {
//  {0xCE, KEY_RETURN,  HID_CONSUMER_PLAY, 500, "KEY_RETURN, HID_CONSUMER_PLAY"}, // OK, Enter
//  {0x95, 0,           MEDIA_PLAY_PAUSE,   500, "MEDIA_PLAY_PAUSE"}, // pause
//  {0x81, 0,           MEDIA_VOLUME_DOWN,   50, "MEDIA_VOLUME_DOWN"}, // volume down
//  {0x80, 0,           MEDIA_VOLUME_UP,   50, "MEDIA_VOLUME_UP"}, // volume up
//  {0xCA, KEY_UP_ARROW,  0,   50, "KEY_UP_ARROW, MEDIA_PREV"}, // up, prev
//  {0xD2, KEY_DOWN_ARROW,  0,   50, "KEY_DOWN_ARROW,MEDIA_NEXT"}, // down, next
//  {0xC1, 0, MEDIA_FAST_FORWARD,   100, "MEDIA_FAST_FORWARD"}, // right
//  {0x99, 0,  MEDIA_REWIND,   100, "MEDIA_REWIND"}, // left
//  {0xC5, KEY_ESC, 0, 500, " KEY_ESC, MEDIA_NEXT"},
//};

// IR signal received from this pin
const int IR_RECEIVE_PIN = 2;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);   // Status message will be sent to PC at 9600 baud
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)  || defined(ARDUINO_attiny3217)
  // To be able to connect Serial monitor after reset or power up and before first printout
  delay(4000);
#endif
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver, enable feedback LED, take LED feedback pin from the internal boards definition

  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(IR_RECEIVE_PIN);

  Consumer.begin(); // Begin HID connection
  Keyboard.begin(); // initialize control over the keyboard
}

void loop() {
  if (IrReceiver.decode()) {  // Grab an IR code
    Serial.println();// 2 blank lines between entries
    Serial.println();
    IrReceiver.printIRResultShort(&Serial);
    Serial.println();
    // Output address and data as source code variables
    IrReceiver.printIRResultAsCVariables(&Serial);

    // handle IR command that is decoded from IR signal and send HID command
    //    for (int i = 0;  i < sizeof(actions) / sizeof(ActionTriggerEntry); i++) {
    //      if (actions[i].command == IrReceiver.decodedIRData.command) {
    //        if (uint8_t  kbKeyCode = actions[i].kbKeyCode) {
    //          Serial.println(kbKeyCode, HEX);
    //          Keyboard.press(kbKeyCode);
    //          delay(100);
    //          Keyboard.releaseAll();
    //        }
    //        if (uint8_t mediaKeyCode = actions[i].mediaKeyCode) {
    //          Consumer.write(mediaKeyCode);
    //        }
    //        if (uint8_t  mediaKeyDelayms = actions[i].mediaKeyDelayms) {
    //          delay(mediaKeyDelayms);
    //        }
    //        if (const char *message = actions[i].message) {
    //          //          Serial.print("=> ");
    //          Serial.println(message);
    //        }
    //        break;
    //      }
    //    }


    if (is_cmd_triggered(IrReceiver.decodedIRData.command == CMD_MEDIA_PLAY_PAUSE,
                         "=> MEDIA_PLAY_PAUSE")) {
      Consumer.write(MEDIA_PLAY_PAUSE);
      // delay for a while to avoid pick signal too fast or avoid noise
      delay(500);
    }

    if (is_cmd_triggered(IrReceiver.decodedIRData.command == CMD_KEY_RETURN,
                         "=> KEY_RETURN")) {
      Keyboard.press(KEY_RETURN);
      delay(100);
      Keyboard.releaseAll();
    }

    if (is_cmd_triggered(IrReceiver.decodedIRData.command == CMD_MEDIA_VOLUME_DOWN,
                         "=> MEDIA_VOLUME_DOWN")) {
      Consumer.write(MEDIA_VOLUME_DOWN);
    }

    if (is_cmd_triggered(IrReceiver.decodedIRData.command == CMD_MEDIA_VOLUME_UP,
                         "=> MEDIA_VOLUME_UP")) {
      Consumer.write(MEDIA_VOLUME_UP);
    }

    if (is_cmd_triggered(IrReceiver.decodedIRData.command == CMD_KEY_UP_ARROW,
                         "=> KEY_UP_ARROW")) {
      Keyboard.press(KEY_UP_ARROW);
      delay(100);
      Keyboard.releaseAll();
    }

    if (is_cmd_triggered(IrReceiver.decodedIRData.command == CMD_KEY_DOWN_ARROW,
                         "=> KEY_DOWN_ARROW")) {
      Keyboard.press(KEY_DOWN_ARROW);
      delay(100);
      Keyboard.releaseAll();
    }

    if (is_cmd_triggered(IrReceiver.decodedIRData.command == CMD_KEY_RIGHT_ARROW,
                         "=> KEY_RIGHT_ARROW")) {
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(100);
      Keyboard.releaseAll();
    }

    if (is_cmd_triggered(IrReceiver.decodedIRData.command == CMD_KEY_LEFT_ARROW,
                         "=> KEY_LEFT_ARROW")) {
      Keyboard.press(KEY_LEFT_ARROW);
      delay(100);
      Keyboard.releaseAll();
    }

    if (is_cmd_triggered(IrReceiver.decodedIRData.command == CMD_KEY_ESC,
                         "=> KEY_ESC")) {
      Keyboard.press(KEY_ESC);
      delay(100);
      Keyboard.releaseAll();
    }

    IrReceiver.resume(); // Prepare for the next value
  }
}
