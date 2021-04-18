/**
 * Use Arduino and IR receiver sensor to accept TV remote signal and send
 * keyboard instruction to host computer to simulate keystoke, hence, instruct
 * host computer to do something - content navigation using Chome TV mode.
 * 
 *  # This project requires the following libaries
 * - IRemote v3.1, https://github.com/Arduino-IRremote/Arduino-IRremote
 *  - HID-project v2.8, https://github.com/NicoHood/HID
 *  The above libraries can be installed from Tools->Library Manager
 *
 *  # Reference
 *  - https://uboopenfactory.univ-brest.fr/Les-Labs/MusicLab/Projets/Arduino-Media-Keys
 *
 * # Author
 * Nick Jiang <congtao.jiang@outlook.com>
 *  2021/04/27
 *
*/
#include <IRremote.h>
#include <HID-Project.h>
#include <HID-Settings.h>
//#include <Keyboard.h>

// IR signal received from this pin
const int IR_RECEIVE_PIN = 2;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);   // Status message will be sent to PC at 9600 baud
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)  || defined(ARDUINO_attiny3217)
  delay(4000); // To be able to connect Serial monitor after reset or power up and before first printout
#endif
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver, enable feedback LED, take LED feedback pin from the internal boards definition

  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(IR_RECEIVE_PIN);

  Consumer.begin(); // Begin HID connection
  Keyboard.begin(); // initialize control over the keyboard
}

void loop() {
  if (IrReceiver.decode()) {  // Grab an IR code
    Serial.println();                               // 2 blank lines between entries
    Serial.println();
    IrReceiver.printIRResultShort(&Serial);
    Serial.println();
    IrReceiver.printIRResultAsCVariables(&Serial);  // Output address and data as source code variables

    // handle IR command that is decoded from IR signal and send HID command
    if (IrReceiver.decodedIRData.command == 0x95) {
      Consumer.write(MEDIA_PLAY_PAUSE);
      Serial.println("Sent MEDIA_PLAY_PAUSE");
      // delay for a while to avoid pick signal too fast or avoid noise
      delay(500);
    }

    // OK/enter button
    if (IrReceiver.decodedIRData.command == 0xCE) {
      Keyboard.press(KEY_RETURN);
      delay(100);
      Keyboard.releaseAll();
      Consumer.write(HID_CONSUMER_PLAY);
      Serial.println("Sent HID_CONSUMER_PLAY");
      // delay for a while to avoid pick signal too fast or avoid noise
      delay(500);
    }
    if (IrReceiver.decodedIRData.command == 0x81) {
      Consumer.write(MEDIA_VOLUME_DOWN);
      Serial.println("Sent MEDIA_VOLUME_DOWN");
    }

    if (IrReceiver.decodedIRData.command == 0x80) {
      Consumer.write(MEDIA_VOLUME_UP);
      Serial.println("Sent MEDIA_VOLUME_UP");
    }

    if (IrReceiver.decodedIRData.command == 0xCA) {
      Keyboard.press(KEY_UP_ARROW);
      delay(100);
      Keyboard.releaseAll();
//      Consumer.write(MEDIA_PREV);
      Serial.println("Sent MEDIA_PREV");
      delay(500);
    }

    if (IrReceiver.decodedIRData.command == 0xD2) {
      Keyboard.press(KEY_DOWN_ARROW);
      delay(100);
      Keyboard.releaseAll();
//      Consumer.write(MEDIA_NEXT);
      Serial.println("Sent MEDIA_NEXT");
      delay(500);
    }

    if (IrReceiver.decodedIRData.command == 0xC1) {
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(100);
      Keyboard.releaseAll();
//      Consumer.write(MEDIA_FAST_FORWARD);
      Serial.println("Sent MEDIA_FAST_FORWARD");
      delay(500);
    }

    if (IrReceiver.decodedIRData.command == 0x99) {
      Keyboard.press(KEY_LEFT_ARROW);
      delay(100);
      Keyboard.releaseAll();
//      Consumer.write(MEDIA_REWIND);
      Serial.println("Sent MEDIA_REWIND");
      delay(500);
    }

    if (IrReceiver.decodedIRData.command == 0xC5) {
      Keyboard.press(KEY_ESC);
      delay(100);
      Keyboard.releaseAll();
//      Consumer.write(MEDIA_STOP);
      Serial.println("Sent MEDIA_STOP");
      delay(500);
    }
    IrReceiver.resume();                            // Prepare for the next value
  }
}
