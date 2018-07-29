/*  TCC Curso de Arduino - BrincandoComIdeias
 *  Link do curso - https://go.hotmart.com/W8213658G
 *  Aluno - Thiago Vilella
 *  Professor - Flávio guimarães
 *  Projeto proposto - Timer estilo microondas
 *  
 *  Solicitações:  
 *  - Contagem de tempo de forma crescente com acionamento de carga ; 
 *  - Contagem de tempo de forma decrescente com acionamento de carga durante contagem ;
 *  - Valores pré determinados para solicitação do usuário ;
 *  - Menu para interação do usuário
 *  
 *  Limitações:
 *  - Máx 4 botões ;
 *  
 *  Otimizações pessoais:
 *  -Contagem regressiva sem acionamento seguida de contagem progressiva com acionamento;
 *  -Interrupção caso botão pressionado durante operação
 * 
 *  Hardware:
 *  - 3 pushbutton
 *  - 1 display oled
 *  - Arduino Uno
 *  - modulo relay
 */

 //Incluindo Bibliotecas
#include <Arduino.h>
#include <Wire.h>
#include <MicroLCD.h>

//Definindo pinos dos botões e Relé
#define pinBot1 8
#define pinBot2 9
#define pinBot3 10
#define pinBot4 11

#define pinRelay 7

LCD_SSD1306 displayoLed; // Instanciando meu display Oled process SSD1306

// String com logo do BrincandoComIdeias
const PROGMEM uint8_t logo[48 * 48 / 8] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xE0,
0xF0, 0xF0, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0x18, 0x18, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF0, 0xF0,
0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xF8,
0xF1, 0x73, 0x1F, 0x0F, 0x0F, 0x07, 0x07, 0x06, 0x04, 0x07, 0x07, 0x0F, 0x0F, 0x1F, 0x63, 0xF1,
0xF8, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xF9, 0xF9, 0xF9, 0xFF,
0xFF, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0xFF,
0xFF, 0xF9, 0xF9, 0xF9, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
0x7F, 0x00, 0x00, 0x40, 0x7F, 0x4F, 0x0F, 0x03, 0x00, 0x0F, 0x4F, 0x7F, 0x7F, 0x00, 0x00, 0x7F,
0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x07, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC,
0xF8, 0xF8, 0x08, 0x04, 0x00, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0x00, 0x04, 0x08, 0xF8, 0xF8,
0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03,
0x07, 0x07, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x1F, 0x1F, 0x0F, 0x0F, 0x08, 0x08, 0x0C, 0x07, 0x07,
0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Criando classe para pushbutton 
class PushButton {
  public:
    PushButton (byte pinBotao, byte tempoDebounce = 200); // Método construtor
    void button_loop(); // Métodolo para leitura dos botões
    bool pressed();  // Método que retorna valor da propriedade apertado
    bool estadoBotao; // propriedade com o estado do botão
  private:
    unsigned long debounceBotao;
    bool estadoBotaoAnt = HIGH;
    bool apertado = false;
    byte pino;
    byte tempo;
};

// Método construtora é o método que tem o mesmo nome que a classe e não recebe tipo é acionado ao instanciar o objeto
PushButton::PushButton (byte pinBotao, byte tempoDebounce) {
  pinMode(pinBotao,INPUT_PULLUP);
  pino = pinBotao;
  tempo = tempoDebounce;
}

// Declarando os métodos da classe PushButton, demais métodos recebem tipo
void PushButton::button_loop() {
  estadoBotao = digitalRead(pino);
  apertado = false;
  if( (millis() - debounceBotao) > tempo) if(!estadoBotao && estadoBotaoAnt){
      apertado = true;
      debounceBotao = millis();
    }
  
  estadoBotaoAnt = estadoBotao;
}

bool PushButton::pressed(){
  return apertado;
}

//Instanciando objetos
PushButton botao1(pinBot1);
PushButton botao2(pinBot2);
PushButton botao3(pinBot3);
PushButton botao4(pinBot4);

//Declaração de variáveis globais
int valorTempo1 = 30; //Variável para guardar o valor do timer
int valorTempo2 = 30; //Variável para guardar o valor de espera para ativar o timer
byte contadorFuncao = 0; // contador para saber qual valor estamos
bool executar = false; // Aciona execucao do funcao selecionada
unsigned long controleTempo = millis(); //Variável para controlar o tempo

void setup() {

  displayoLed.begin(); //Iniciando o objeto display
  pinMode(pinRelay,OUTPUT); // Definindo pino do Relé como OUTPUT

  displayoLed.clear(); // Limpando o display
  displayoLed.setCursor(40,1); // Posicionando cursor na coluna 40 linha 1
  displayoLed.draw(logo,48,48); // Desenhando no display parametros (imagem,largura,altura)

  digitalWrite(pinRelay,HIGH); // Saída em HIGH para manter Relé desligado e ligar o LED de estado
  
}

void loop() {
    
  botao1.button_loop(); // Método para ler estado do objeto botao1
  botao2.button_loop(); // Método para ler estado do objeto botao2
  botao3.button_loop(); // Método para ler estado do objeto botao3

  if(botao2.pressed() && contadorFuncao < 4) contadorFuncao++; // Condição para alterar contadorFuncao caso botao2 seja apertado
  if( contadorFuncao >= 4) contadorFuncao = 0; // Caso o contador função == a 4 volta para 0, Paraq contadorFuncao maior que 4 apenas usando botao2 durante a execução da funcaoMenu1() dentro de funcaoMenu2() 

  //Utilizando switch() ... case para substituir série de if()
  switch(contadorFuncao){
    case 0: //Case 0 para apresentação do logo, não deve executar carga é uma tela de "descanço"
      displayoLed.setCursor(40,1);
      displayoLed.draw(logo,48,48);
      break;
    
    case 1: //Entra no menu para executar a contagem de tempo de forma crescente com acionamento de carga
      valorTempo1 = 30; //Define um tempo padrão inicial, caso a funcaoMenu1() receba executar irá utilizar o tempo padrão independente do que tenha sido escolhido em outro case
      funcaoMenu1(); //Chama funcaoMenu1() para escolher o valor do timer
      if (executar) funcaoExec1(); //Executa a funcao do modo escolhido 
      break;
      
    case 2:
      valorTempo1 = 30; //Define um tempo padrão inicial, caso a funcaoMenu1() receba executar irá utilizar o tempo padrão independente do que tenha sido escolhido em outro case
      funcaoMenu1(); //Chama funcaoMenu1() para escolher o valor do timer
      if (executar) funcaoExec2(); //Executa a funcao do modo escolhido 
      break;
      
    case 3:
      valorTempo1 = 10; //Define um tempo padrão inicial, caso a funcaoMenu1() receba executar irá utilizar o tempo padrão independente do que tenha sido escolhido em outro case
      valorTempo2 = 30; //Define um tempo padrão inicial, caso a funcaoMenu2() receba executar irá utilizar o tempo padrão independente do que tenha sido escolhido em outro case
      funcaoMenu2(); //Chama funcaoMenu2() para escolher os valores dos "timers"
      if (executar) funcaoExec3(); //Executa a funcao do modo escolhido     
      displayoLed.clear(); //Limpa o Display   
      executar = false; //Define executar como false para que o proximo case não entre executando
      break;
  }

}

//Funcao de execução do menu de seleção de tempo tipo 1
void funcaoMenu1(){
  int valorTimer = funcaoTimer(); // iniciando valores para while
  int valorAnterior;   // iniciando valores para while
  executar = false; // Sempre que entrar no menu executar recebe false
  
  
  displayoLed.clear();// limpando display e levando cursor para posição 0,0
  displayoLed.setFontSize(FONT_SIZE_LARGE); // Setando tamanho da fonte
  displayoLed.print("Timer: ");
  displayoLed.printLong(valorTimer); // imprimindo o valor armazenado para Timer
  displayoLed.setCursor(0,4);
  displayoLed.print("Menu: ");
  displayoLed.printLong(contadorFuncao);

  
  while( executar == false ){ /* Para sair do While precisa acionar o botao 4 para executar = true */

        
    if(valorAnterior != valorTimer){
      displayoLed.clear();// limpando display e levando cursor para posição 0,0
      displayoLed.setFontSize(FONT_SIZE_LARGE); // Setando tamanho da fonte
      displayoLed.print("Timer: ");
      displayoLed.printLong(valorTimer); // imprimindo o valor armazenado para Timer
      displayoLed.setCursor(0,4);
      displayoLed.print("Menu: ");
      displayoLed.printLong(contadorFuncao);
    }

    valorAnterior = valorTimer;
    valorTimer = funcaoTimer();
    
    botao2.button_loop();
    if (botao2.pressed()) {
      contadorFuncao++; // muda o contador da função para ir para o próximo menu
      break; // Sai do While caso aperto o botao 2, mudando de case sem executar a função do case atual.
    }
    
    botao4.button_loop(); //Leitura do estado do botao 4   
    if (botao4.pressed()) executar = true; //Executar sai do while e aciona a funcaoExec1()
    
  }
    
}

//Funcao de execução do menu de seleção de tempo tipo 2
void funcaoMenu2(){
  while((contadorFuncao == 3) && !executar){
    funcaoMenu1();
    int valorTimer = funcaoTimer2();
    int valorAnterior;
    executar = false;

      while( executar == false ){ /* Para sair do While precisa acionar o botao 4 para executar ir para true */
          
      if(valorAnterior != valorTimer){
        displayoLed.clear();
        displayoLed.setFontSize(FONT_SIZE_LARGE); // Setando tamanho da fonte
        displayoLed.print("Timer2: ");
        displayoLed.printLong(valorTimer); // imprimindo o valor armazenado para Timer
        displayoLed.setCursor(0,4);
        displayoLed.print("Menu: ");
        displayoLed.printLong(contadorFuncao); //Vantagem de imprimir o contadorFuncao ao invés de colocar número é poder ver o valor do contadorFuncao serve como debug
      }
  
      valorAnterior = valorTimer; 
      valorTimer = funcaoTimer2();
      
      botao2.button_loop();
      if (botao2.pressed()) {
        contadorFuncao++; // muda o contador da função para ir para o próximo menu
        break; // Sai do While caso aperto o botao 2.
      }
      
      botao4.button_loop();    
      if (botao4.pressed()) executar = true; 
      
    }    
    
  }
        
}

//Funcao que executa a contagem progressiva
void funcaoExec1(){
  displayoLed.clear();
  displayoLed.setCursor(38,1);
  displayoLed.draw(logo,48,48);
  controleTempo = millis();
  int valorDisplay = 0;  
  delay(500);
  digitalWrite(pinRelay, LOW);
  displayoLed.clear();
  displayoLed.setFontSize(FONT_SIZE_LARGE);
  displayoLed.print("de:");
  displayoLed.printLong(valorDisplay);
  displayoLed.setCursor(0, 4);
  displayoLed.print("ate: ");
  displayoLed.printLong(valorTempo1);
  
  
  while( ( executar == true ) && ( ( valorTempo1 - valorDisplay ) > 0) ){
    int valorAnterior = valorDisplay;
             
    if( ( millis() - controleTempo ) > 1000){
      controleTempo = millis();
      valorDisplay++;
    }
    
    if (valorAnterior != valorDisplay){
      displayoLed.clear();
      displayoLed.setFontSize(FONT_SIZE_LARGE);
      displayoLed.print("de:");
      displayoLed.printLong(valorDisplay);
      displayoLed.setCursor(0, 4);
      displayoLed.print("ate: ");
      displayoLed.printLong(valorTempo1);
    }
    
    botao1.button_loop();
    botao2.button_loop();
    botao3.button_loop();   
    botao4.button_loop();
    
    if (botao1.pressed() && (valorTempo1>30) ) valorTempo1 -= 30; 
    else if (botao3.pressed() && (valorTempo1<300)) valorTempo1 += 30;
    else if (botao4.pressed()) executar = false;
    else if (botao2.pressed()){
      executar = false;
      contadorFuncao++;
      break;      
    }
        
  }
  digitalWrite(pinRelay, HIGH);
  
}

//Funcao que executa a contagem regressiva 
void funcaoExec2(){
  displayoLed.clear();
  displayoLed.setCursor(38,1);
  displayoLed.draw(logo,48,48);
  controleTempo = millis();   
  delay(500);
  digitalWrite(pinRelay, LOW);
  displayoLed.clear();
  displayoLed.setFontSize(FONT_SIZE_LARGE);
  displayoLed.print("de: ");
  displayoLed.printLong(valorTempo1);
  displayoLed.setCursor(0, 4);
  displayoLed.print("ate: ");
  displayoLed.printLong(0);
  
  while( ( executar == true ) && ( valorTempo1 > 0 ) ){
    int valorAnterior = valorTempo1;
    
    if((millis() - controleTempo) > 1000){
      controleTempo = millis();
      valorTempo1--;
    }
    
    if (valorAnterior != valorTempo1){
      displayoLed.clear();
      displayoLed.setFontSize(FONT_SIZE_LARGE);
      displayoLed.print("de: ");
      displayoLed.printLong(valorTempo1);
      displayoLed.setCursor(0, 4);
      displayoLed.print("ate: ");
      displayoLed.printLong(0);  
    }
    
    botao1.button_loop();
    botao2.button_loop();
    botao3.button_loop();   
    botao4.button_loop();
    if (botao1.pressed() && (valorTempo1 > 30) ) valorTempo1 -= 30; 
    else if (botao3.pressed() && (valorTempo1 < 300)) valorTempo1 += 30;
    else if (botao4.pressed()) executar = false;
    else if (botao2.pressed()){
      executar = false;
      contadorFuncao++;
      break;
    }
    
  }
  digitalWrite(pinRelay, HIGH);
  delay(500);
  
}

//Funcao executa contagem regressiva sem acionamento
void funcaoNExec(){
  displayoLed.clear();
  displayoLed.setCursor(38,1);
  displayoLed.draw(logo,48,48);
  controleTempo = millis();  
  delay(500);
  digitalWrite(pinRelay, HIGH);
  displayoLed.clear();
  displayoLed.setFontSize(FONT_SIZE_LARGE);
  displayoLed.print("de: ");
  displayoLed.printLong(valorTempo2);
  displayoLed.setCursor(0, 4);
  displayoLed.print("ate: ");
  displayoLed.printLong(0);
  
  while( ( executar == true ) && ( valorTempo2 > 0 ) ){
    int valorAnterior = valorTempo2;
    
    if((millis() - controleTempo) > 1000){
      controleTempo = millis();
      valorTempo2--;
    }
    
    if (valorAnterior != valorTempo2){
      displayoLed.clear();
      displayoLed.setFontSize(FONT_SIZE_LARGE);
      displayoLed.print("de: ");
      displayoLed.printLong(valorTempo2);
      displayoLed.setCursor(0, 4);
      displayoLed.print("ate: ");
      displayoLed.printLong(0);  
    }
    
    botao1.button_loop();
    botao2.button_loop();
    botao3.button_loop();   
    botao4.button_loop();
    if (botao1.pressed() && (valorTempo2 > 30) ) valorTempo2 -= 30; 
    else if (botao3.pressed() && (valorTempo2 < 300)) valorTempo2 += 30;
    else if (botao4.pressed()) executar = false;
    else if (botao2.pressed()){
      executar = false;
      contadorFuncao++;
      break;
    }
    
  }
    
}

//Funcao Mista, executa contagem regressiva sem acionamento e contagem progressiva com acionamento
void funcaoExec3(){
  funcaoNExec();
  funcaoExec1(); 
  
}

//Função para incremento do tempo de 30 em 30
int funcaoTimer(){
    botao1.button_loop();
    botao3.button_loop(); 
  if(botao1.pressed() && botao3.pressed()) return valorTempo1;
    else if(valorTempo1 < 300 && botao3.pressed()) valorTempo1 += 30;
    else if(valorTempo1 > 30 && botao1.pressed()) valorTempo1 -= 30;
  return valorTempo1;
}

//Função para incremento do tempo2 de 30 em 30
int funcaoTimer2(){  
    botao1.button_loop();
    botao3.button_loop();
  if(botao1.pressed() && botao3.pressed()) return valorTempo2;
    else if(valorTempo2 < 300 && botao3.pressed()) valorTempo2 += 30;
    else if(valorTempo2 > 30 && botao1.pressed()) valorTempo2 -= 30;
  return valorTempo2;
}
