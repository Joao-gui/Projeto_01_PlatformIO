#include <Arduino.h>

#define MOTOR 6
#define BUZZER 5
#define POT A1
#define BT1 4

//constantes
#define PRESSIONADO HIGH
#define SOLTO LOW

//Variáveis auxiliares
char debouncingBT1 = 100;
bool statusBT1;

bool statusMotor = 0;

//cosntante para conversão
const byte tabela_7seg[11][7] =
{//A B C D E F G
  {1,1,1,1,1,1,0},  //0
  {0,1,1,0,0,0,0},  //1
  {1,1,0,1,1,0,1},  //2
  {1,1,1,1,0,0,1},  //3
  {0,1,1,0,0,1,1},  //4
  {1,0,1,1,0,1,1},  //5
  {1,0,1,1,1,1,1},  //6
  {1,1,1,0,0,0,0},  //7
  {1,1,1,1,1,1,1},  //8
  {1,1,1,1,0,1,1},  //9
  {1,1,0,0,1,1,1},  //P
};

//Protótipo de funções
void beep(void);

void converteValorDisplay(byte valor);

void setup() {
  pinMode(BT1, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MOTOR, OUTPUT);

  // Configurando pinos do display 7seg
  for (int i = 7; i < 14; i++)
  {
    pinMode(i, OUTPUT);
  }  
}

void loop() {

  int valor = analogRead(POT);

  // Pushbuttons
  if(digitalRead(BT1)==LOW){  //se botão pressionado
    debouncingBT1--;          //decrementa variável de Debouncing
    if(debouncingBT1 == 0 && statusBT1 == SOLTO){ //se Debouncing chegou a zeero e botão estava solto
      beep();                                 //faz beep sonoro
      statusMotor ^= 1;
      if (statusMotor)
      {
        for (int i = 0; i < valor/4; i++)
        {
          analogWrite(MOTOR,i);
          delay(10);
        }        
      }
      else
      {
        for (int i = valor/4; i > 0; i--)
        {
          analogWrite(MOTOR, i);
          delay(10);
        }        
      }         
      statusBT1 = PRESSIONADO;                //sinaliza botão pressionado
    }
  }
  else{                   //se não
    debouncingBT1 = 100;  //reinicia variável debouncing
    statusBT1 = SOLTO;    //sinaliza botão solto
  }

  if (statusMotor)
  {
    converteValorDisplay(map(valor, 0, 1023, 0, 9));
    analogWrite(MOTOR, map(valor, 0, 1023, 0 ,255));
  }

  else
  {
    analogWrite(MOTOR, 0);
    converteValorDisplay(10);
  } 
}

/*
  Função beep: Faz sinal sonoro
  retorno: nada
  parâmetros:nada
*/
void beep(void){
  digitalWrite(BUZZER,HIGH);  //liga buzzer 
  delay(100);                 //espera 0.1 s
  digitalWrite(BUZZER,LOW);   //desliga buzzer
  delay(100);                 //espera 0.1 s
}

//funão para conversão de valor em 7 segmentos
void converteValorDisplay(byte valor){

  byte pino = 13; //variável para varredura dos pinos

  for(byte x = 0;x<7;x++){  //varre tabela
    digitalWrite(pino,tabela_7seg[valor][x]);//atualiza segmento
    pino--; //decrementa pino
    if(pino == 10) pino = 5;  //se chegou em 10 pula para 5
  }
}