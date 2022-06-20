//Codigo do Arduino para programar o sistema de monitoramento

//Inclui as bibliotecas
#include <Arduino.h>
#include <multiMedidor.h>

//Definicoes de pinos
#define S_AMPpin A1
#define S_VOLTpin A2
#define RELE1_pin 7
#define RELE2_pin 8

//Unioes e estruturas
typedef union
{
  double valor;
  byte mapeamento[4];
} VarEletrica;

/*                              Variaveis                                   */
//Constantes para processamento das leituras
const double RTC_Amp = 0.100, RTC_Volt = 0.1, RefAnalogica = 5.0 / 1024.0;
const double calibreAmp = 0, calibreVolt = 0, defasagemVolt = 5.5; //constantes de calibracao dos sensores
double tensao = 127.47;
VarEletrica Amp, AmpAntigo, Volt, VoltOld, Potencia; //variaveis da energia
bool modo220V = false;
char *dadosR;
byte nOut, strRsize;

//Funcoes
double LeituraAmp(char pin);
double LeituraVolt(char pin);
double CalcPotencia(double Tensao, double Corrente);
void printVarEletrica(VarEletrica x);
void pinCtrl(char *dadosR);

//instancias e objetos
EnergyMonitor emon;

//Execucao do programa
void setup()
{
  Serial.begin(9600);
  nOut = 2;
  strRsize = nOut + 1;
  dadosR = (char *)malloc(strRsize);
  pinMode(RELE1_pin, OUTPUT);
  pinMode(RELE2_pin, OUTPUT);
  emon.current(S_AMPpin, calibreAmp);
  emon.voltage(S_VOLTpin, calibreVolt, defasagemVolt);
}
void loop()
{
  if (Serial.available() != 0)
  {
    Serial.readBytes(dadosR, strRsize);
    pinCtrl(dadosR);
  }
  emon.calcVI(17, 100);
  Amp.valor = emon.Irms;  //Obtem a medida nova
  Volt.valor = emon.Vrms; //comentar se nao utilizar o sensor de tensao
  //AmpAntigo.valor = Amp.valor;                      //Medida antiga passa a ser a nova base
  //Volt.valor = tensao;                              //comentar se utilizar o sensor de tensao
  Potencia.valor = Amp.valor * Volt.valor;
  //manda os dados para o raspberry
  printVarEletrica(Volt);
  printVarEletrica(Amp);
  printVarEletrica(Potencia);
}

//REFERENCIA DAS FUNCOES
//Recebe a leitura do sensor de corrente e trasforma em corrente, retorna um valor para corrente
/*
double LeituraAmp(char pin)
{
  static double EscAnalogica = (RTC_Amp-calibreAmp) / RefAnalogica;
  
  int leitura, picoAlto = 0, picoBaixo = 1023;
  for(int i=0; i < 300; i++)
  {
    leitura = analogRead(pin);
    if(leitura > picoAlto)
    {
      picoAlto = leitura;
    }
    if(leitura < picoBaixo)
    {
      picoBaixo = leitura;
    }
  }
  leitura = picoAlto - picoBaixo;
  double valor = leitura /  EscAnalogica;
  return valor;
}
*/
//Recebe a leitura do sensor de tensao e transforma em corrente
/*
double LeituraVolt(char pin)
{
  int leitura = analogRead(pin);
  return leitura * RTC_Volt;
}
*/
//Calcula a potencia aparente baseado nos valores dos sensores
/*
double CalcPotencia(double Tensao, double Corrente)
{
  return Tensao * Corrente;
}
*/
//Manda as variaveis lidas para a serial
void printVarEletrica(double x)
{
  for (char i = 3; i >= 0; i++)
  {
    Serial.println(x);
  }
  Serial.println();
  delay(1000);
}
//controla os pinos baseado na entrada serial
void pinCtrl(char *dadosR)
{
  for (char i = 0; i < nOut; i++)
    *(dadosR + i) -= '0';

  digitalWrite(RELE1_pin, *(dadosR + 0));
  digitalWrite(RELE2_pin, *(dadosR + 1));
}