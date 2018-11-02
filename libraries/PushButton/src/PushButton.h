#ifndef PushButton_h
#define PushButton_h

#include "Arduino.h"

class PushButton {
  public:
    PushButton (byte pinBotao , int tempoDebounce = 200 );
    void lerBotao();
    bool pressed();
    bool unpressed();
  private:
    unsigned long debounceBotao;
    bool estadoBotaoAnt = HIGH;
    bool estadoBotao;
    bool apertado;
    bool soltado;
    byte pino;
    byte tempo;  
};

#endif