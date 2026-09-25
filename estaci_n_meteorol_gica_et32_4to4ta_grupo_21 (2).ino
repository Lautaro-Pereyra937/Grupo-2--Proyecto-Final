//Componentes
#include <Adafruit_LiquidCrystal.h>
#define SensorHumedad A0
#define SensorTemperatura A1
#define SensorViento A2

#define HorasMas 2
#define MinutosMas 3
#define SegundosMas 4
#define HorasMenos 5
#define MinutosMenos 6
#define SegundosMenos 7

//Valores
int ValorHumedad;
int ValorHumedadREAL;

int ValorTemperatura;
int ValorTemperaturaREAL;

int ValorViento;

int HorasLCD;
int MinutosLCD;
int SegundosLCD;

bool ValorHorasMas;
bool ValorMinutosMas;
bool ValorSegundosMas;
bool ValorHorasMenos;
bool ValorMinutosMenos;
bool ValorSegundosMenos;

String TextoHumedad = "";
String TextoTemperatura = "";
String TextoViento = "";
String TextoLCD = "";

int indiceDesplazamiento = 0;

Adafruit_LiquidCrystal lcd1(32);

void setup()
{
  Serial.begin(9600);
  lcd1.begin(16, 2);
  lcd1.setBacklight(1);
  pinMode(SensorHumedad, INPUT);
  pinMode(SensorTemperatura, INPUT);
  pinMode(SensorViento, INPUT);
}

void loop()
{
  DetectarHumedad();
  DetectarTemperatura();
  DetectarViento();
  LCDMostrar(TextoLCD);
  LCDMostrarTiempo();
  LCDActualizarTiempo();
  AjustarBotones();
}

void LCDMostrar(String texto)
{
   TextoLCD = TextoHumedad + " | " + TextoTemperatura + " | " + TextoViento + "          ";
  lcd1.setCursor(0, 0);
  String textoFragmento = texto.substring(indiceDesplazamiento, indiceDesplazamiento + 16);
 
  while (textoFragmento.length() < 16)
  {
    textoFragmento += " ";
  }
  lcd1.print(textoFragmento);
  indiceDesplazamiento++;
  if (indiceDesplazamiento >= texto.length())
  {
    indiceDesplazamiento = 0;
  }
}

void DetectarHumedad()
{
  ValorHumedad = analogRead(SensorHumedad);
  ValorHumedadREAL = map(ValorHumedad, 0, 539, 0, 100);
  if (ValorHumedadREAL > 70)
  {
   TextoHumedad = "Esta Lloviendo.";
  }
  else if (ValorHumedadREAL < 70 && ValorHumedadREAL > 35)
  {
   TextoHumedad = "Esta humedo.";
  }
  else if (ValorHumedadREAL < 35)
  {
   TextoHumedad = "Esta seco/despejado.";
  }
}

void DetectarTemperatura()
{
  ValorTemperatura = analogRead(SensorTemperatura);
  ValorTemperaturaREAL =  map(ValorTemperatura, 18,360,-40,126);
 
  if (ValorTemperaturaREAL >= 35)
  {
   TextoTemperatura = "Peligrosamente caluroso.";
  }
  else if (ValorTemperaturaREAL >= 31)
  {
   TextoTemperatura = "Muy caluroso.";
  }
  else if (ValorTemperaturaREAL >= 25)
  {
   TextoTemperatura = "Moderadamente caluroso.";
  }
  else if (ValorTemperaturaREAL >= 20)
  {
   TextoTemperatura = "Templado.";
  }
  else if (ValorTemperaturaREAL <= 10 && ValorTemperaturaREAL >= 0)
  {
   TextoTemperatura = "Un poco de frio.";
  }
  else if (ValorTemperaturaREAL <= -1 && ValorTemperaturaREAL >= -9)
  {
   TextoTemperatura = "Frio moderado.";
  }
  else if (ValorTemperaturaREAL <= -10 && ValorTemperaturaREAL >= -26)
  {
   TextoTemperatura = "Mucho frio, tome precauciones.";
  }
  else if (ValorTemperaturaREAL <= -27)
  {
   TextoTemperatura = "Frio extremo, no salga.";
  }
}

void DetectarViento()
{
   ValorViento = analogRead(SensorViento);
  if (ValorViento <= 49)
  {
   TextoViento = "Brisa muy debil.";
  }
  else if (ValorViento >= 49 && ValorViento <= 140)
  {
   TextoViento = "Brisa moderada.";
  }
  else if (ValorViento >= 140 && ValorViento <= 354)
  {
   TextoViento = "Viento fresco.";
  }
}


void LCDMostrarTiempo()
{
  lcd1.setCursor(0,1);
 
  if (HorasLCD < 10) lcd1.print("0");
  {
  lcd1.print(HorasLCD);
  lcd1.print(":");
  }
 
  if (MinutosLCD < 10) lcd1.print("0");
  {
  lcd1.print(MinutosLCD);
  lcd1.print(":");
  }
 
  if (SegundosLCD < 10) lcd1.print("0");
  {
  lcd1.print(SegundosLCD);
  lcd1.print("   ");
  }
}


void LCDActualizarTiempo()
{
  SegundosLCD++;
  if (SegundosLCD > 59)
  {
    SegundosLCD = 0;
    MinutosLCD++;
  }
 
  if (MinutosLCD > 59)
  {
    MinutosLCD = 0;
    HorasLCD++;
  }
  delay(1000);
}

void AjustarBotones()
{
  ValorHorasMas = digitalRead(HorasMas);
  ValorMinutosMas = digitalRead(MinutosMas);
  ValorSegundosMas = digitalRead(SegundosMas);
  
  ValorHorasMenos = digitalRead(HorasMenos);
  ValorMinutosMenos = digitalRead(MinutosMenos);
  ValorSegundosMenos = digitalRead(SegundosMenos);
 
  if (ValorHorasMas == LOW)
  {
   HorasLCD+=2;
  }
 
  if (ValorMinutosMas == LOW)
  {
   MinutosLCD+=2;
  }
 
  if (ValorSegundosMas == LOW)
  {
   SegundosLCD+=2;
  }
 
  if (ValorHorasMenos == LOW)
  {
   HorasLCD--;
  }
 
  if (ValorMinutosMenos == LOW)
  {
   MinutosLCD--;
  }
 
  if (ValorSegundosMenos == LOW)
  {
   SegundosLCD--;
  }
}