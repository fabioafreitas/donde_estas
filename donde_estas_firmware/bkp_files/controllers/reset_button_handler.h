//
// Created by fábio alves on 19/010/2023.
//

#ifndef _RESET_BUTTON_HANDLER_H_
#define _RESET_BUTTON_HANDLER_H_

#define RESET_BUTTON_HANDLER_DEBUG

#ifndef PUSH_BTN_PIN
#define PUSH_BTN_PIN 35
#endif

#ifndef PUSH_REFERENCE_TIMER
#define PUSH_REFERENCE_TIMER 3000 // miliseconds
#endif

volatile bool buttonState = false; // Estado do botão
volatile unsigned long buttonPressStartTime = 0; // Tempo de início do pressionamento do botão

void IRAM_ATTR handleButtonPress() {
  buttonState = !buttonState; // Inverte o estado do botão
  buttonPressStartTime = millis(); // Armazena o tempo atual
}

void initResetButtonTimer() {
  pinMode(PUSH_BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PUSH_BTN_PIN), handleButtonPress, CHANGE);
}

void runResetButtonTimer(void (*func)()) {
  if (buttonState && (millis() - buttonPressStartTime >= PUSH_REFERENCE_TIMER)) {
    // Botão pressionado por mais de 3 segundos, execute a função customizada
    buttonState = false; // Reseta o estado do botão
    func();
  }
}

#endif /* _RESET_BUTTON_HANDLER_H_ */