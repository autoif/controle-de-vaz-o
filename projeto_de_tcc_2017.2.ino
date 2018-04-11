#include <LiquidCrystal.h>
LiquidCrystal lcd(53, 51, 49, 47, 45, 43); // diz quais pinos estão sendo usados pelo lcd
const int Trigger = 9;
const int Sensor_1 = 11; // cisterna
const int Sensor_2 = 5; // caixa

const int bomba = 52; // bomba

float volume1 = 0;  // referente a cisterna
float duracao1, cm1;
float altura1 = 0;

float volume2 = 0;  // referente a caixa d'água
float duracao2, cm2;
float altura2 = 0;

int motorfecha [] = {18, 19, 20, 21}; // referente a valula de controle de vazão (fechando)
int motorabre [] = {21, 20, 19, 18}; // referente a valula de controle de vazão (abrindo)
int timer = 10;
int contador1 = 0;
int contador2 = 0;
int passo2 = 420;
int passo1 = 420;
int volta1 = 0;
int volta2 = 0;


void setup() {
  pinMode(Trigger, OUTPUT); // configura as entradas e saidas do ultrassom
  pinMode(Sensor_1, INPUT);
  pinMode(Sensor_2, INPUT);

  lcd.begin(16, 2);  //inicia lcd

  pinMode(bomba, OUTPUT);


  for (contador1 = 0; contador1 < 4; contador1++) {
    pinMode(motorfecha[contador1], OUTPUT);
  }

  for (contador2 = 0; contador2 < 4; contador2++) {
    pinMode(motorabre[contador2], OUTPUT);
  }


}
void loop()
{
  lcd.clear();  // limpa lcd
  lcd.print("cisterna  caixa");
  lcd.setCursor(0, 1);  // diz coluna zero e linha 1
  lcd.print( volume1 ); // imprime valor da cisterna
  lcd.setCursor(4, 1);  // diz coluna 4 e linha 1
  lcd.print("Lt"); // unidade de volume em litros
  lcd.setCursor(10, 1);  // diz coluna 8 e linha 1
  lcd.print( volume2 ); // imprime valor da caixa
  lcd.setCursor(14, 1);  // diz coluna 12 e linha 1
  lcd.print("Lt"); // unidade de medida em litros
  lcd.setCursor(16, 1);  // diz coluna 12 e linha 1


  //SENSOR 1-- cisterna

  digitalWrite(Trigger, LOW); // ativa eco do sensor cisterna
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(5);
  digitalWrite(Trigger, LOW);

  duracao1 = pulseIn(Sensor_1, HIGH);
  // convert the time into a distance microsegundo
  altura1 = (0.0170 * duracao1); // velocidade do som convertida em mm/microseg multiplicada pelo o tempo de resposta    x=0.0170t
  cm1 = 76 - altura1;       // altura total do reservatorio utilizado menos a altura medida
  volume1 = (1320 * cm1) / 1000; // converte em litros

  delay(300);

  // SENSOR 2 -- caixa

  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(5);
  digitalWrite(Trigger, LOW);

  duracao2 = pulseIn(Sensor_2, HIGH);
  // convert the time into a distance microsegundo
  altura2 = (0.0170 * duracao2); // cm x=0.0170t
  cm2 = 76 - altura2; // cm
  volume2 = (1320 * cm2) / 1000; // converte em litros

  if (volume1/*cisterna*/ >= 10 && volume2/*caixa*/ <= 10 )
  {
    digitalWrite(bomba, HIGH);  // liga bomba
  }
  if (volume1 /*cisterna*/ <= 10 || volume2/*caixa*/ >= 90)
  {
    digitalWrite(bomba, LOW); //desliga bomba
  }

  if (volume1 /*cisterna*/ <= 10 && volume2/*caixa*/ <= 70) // condição de fechar a valvula na metade
  {

    if ( passo1 < volta1)
    {
      passo1 = passo1 + 1 ;
      for (contador1 = 0; contador1 < 4; contador1++) {
        digitalWrite(motorfecha[contador1], HIGH);
        delay(timer);
        digitalWrite(motorfecha[contador1], LOW);
        delay(timer);
      }
    }
  }

  if ( volume2/*caixa*/ >= 70)  // condição de abrir a valvula
  {
    if ( passo2 < volta2) {
      passo2 = passo2 + 1 ;
      for (contador2 = 0; contador2 < 4; contador2++) {
        digitalWrite(motorabre[contador2], HIGH);
        delay(timer);
        digitalWrite(motorabre[contador2], LOW);
        delay(timer);
      }
    }
  }
  delay(50);
}

