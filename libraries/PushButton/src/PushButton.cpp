#include "PushButton.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

PushButton::PushButton (byte pinBotao , int tempoDebounce){
  pinMode( pinBotao , INPUT_PULLUP ); //Indicando modo do pino como input usando o resistor de pullup interno
  debounceBotao = millis();
  pino = pinBotao;
  tempo = tempoDebounce;
}

void PushButton::lerBotao() {
  estadoBotao = digitalRead(pino);
  apertado = false;
  soltado = false;

  if ( ( millis() - debounceBotao ) > tempo ){
    if (!estadoBotao && estadoBotaoAnt) {
      apertado = true;
      debounceBotao = millis();
    } else if (estadoBotao && !estadoBotaoAnt) {
      soltado = true;
      debounceBotao = millis();
    }
  }
  estadoBotaoAnt = estadoBotao;
}

bool PushButton::pressed(){
  return apertado;
}

bool PushButton::unpressed(){
  return soltado;
}