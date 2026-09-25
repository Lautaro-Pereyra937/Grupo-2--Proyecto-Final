#include <Adafruit_LiquidCrystal.h>

#define SensorHumedad A0
#define SensorTemperatura A1
#define SensorViento A2

#define fila1 9
#define fila2 8
#define fila3 7
#define fila4 6

#define columna1 5
#define columna2 4
#define columna3 3
#define columna4 2

const int filas[4] = {fila1, fila2, fila3, fila4};
const int columnas[4] = {columna1, columna2, columna3, columna4};

Adafruit_LiquidCrystal lcd1(32);

int ValorHumedad;
int ValorHumedadREAL;

int ValorTemperatura;
int ValorTemperaturaREAL;

int ValorViento;

int HorasLCD = 0;
int MinutosLCD = 0;
int SegundosLCD = 0;

bool teclaHoras = false;
bool teclaMinutos = false;
bool teclaSegundos = false;

String TextoHumedad = "";
String TextoTemperatura = "";
String TextoViento = "";

unsigned long tiempoAnterior = 0;
unsigned long tiempoLCD = 0;
unsigned long tiempoSensores = 0;

int indiceDesplazamiento = 0;

void setup()
{
  Serial.begin(9600);

  lcd1.begin(16, 2);
  lcd1.setBacklight(1);

  pinMode(SensorHumedad, INPUT);
  pinMode(SensorTemperatura, INPUT);
  pinMode(SensorViento, INPUT);

  pinMode(fila1, OUTPUT);
  pinMode(fila2, OUTPUT);
  pinMode(fila3, OUTPUT);
  pinMode(fila4, OUTPUT);

  digitalWrite(fila1, HIGH);
  digitalWrite(fila2, HIGH);
  digitalWrite(fila3, HIGH);
  digitalWrite(fila4, HIGH);

  pinMode(columna1, INPUT_PULLUP);
  pinMode(columna2, INPUT_PULLUP);
  pinMode(columna3, INPUT_PULLUP);
  pinMode(columna4, INPUT_PULLUP);

  tiempoAnterior = millis();
  tiempoLCD = millis();
  tiempoSensores = millis();
}

void loop()
{
  unsigned long tiempoActual = millis();

  if(tiempoActual - tiempoAnterior >= 1000)
  {
    tiempoAnterior = tiempoActual;
    LCDActualizarTiempo();
    LCDMostrarTiempo();
  }

  if(tiempoActual - tiempoSensores >= 500)
  {
    tiempoSensores = tiempoActual;

    DetectarHumedad();
    DetectarTemperatura();
    DetectarViento();
  }

  char tecla = LeerTecla();

  if(tecla != '\0')
  {
    AjustarHora(tecla);
  }

  if(tiempoActual - tiempoLCD >= 500)
  {
    tiempoLCD = tiempoActual;
    LCDMostrarTexto();
  }

  delay(20);
}

void DetectarHumedad()
{
  ValorHumedad = analogRead(SensorHumedad);

  ValorHumedadREAL = map(ValorHumedad,0,539,0,100);

  if(ValorHumedadREAL > 70)
  {
    TextoHumedad = "Esta Lloviendo.";
  }
  else if(ValorHumedadREAL > 35)
  {
    TextoHumedad = "Esta humedo.";
  }
  else
  {
    TextoHumedad = "Esta seco.";
  }
}

void DetectarTemperatura()
{
  ValorTemperatura = analogRead(SensorTemperatura);

  ValorTemperaturaREAL = map(ValorTemperatura,18,360,-40,126);

  if(ValorTemperaturaREAL >= 35)
  {
    TextoTemperatura = "Muy caluroso.";
  }
  else if(ValorTemperaturaREAL >= 31)
  {
    TextoTemperatura = "Caluroso.";
  }
  else if(ValorTemperaturaREAL >= 25)
  {
    TextoTemperatura = "Moderadamente.";
  }
  else if(ValorTemperaturaREAL >= 20)
  {
    TextoTemperatura = "Templado.";
  }
  else if(ValorTemperaturaREAL >= 0)
  {
    TextoTemperatura = "Un poco frio.";
  }
  else if(ValorTemperaturaREAL >= -9)
  {
    TextoTemperatura = "Frio moderado.";
  }
  else if(ValorTemperaturaREAL >= -26)
  {
    TextoTemperatura = "Mucho frio.";
  }
  else
  {
    TextoTemperatura = "Frio extremo.";
  }
}

void DetectarViento()
{
  ValorViento = analogRead(SensorViento);

  if(ValorViento <= 49)
  {
    TextoViento = "Brisa debil.";
  }
  else if(ValorViento <= 140)
  {
    TextoViento = "Brisa moderada.";
  }
  else
  {
    TextoViento = "Viento fresco.";
  }
}

void LCDMostrarTexto()
{
  String texto = TextoHumedad + " " +
                 TextoTemperatura + " " +
                 TextoViento + "          ";

  if(indiceDesplazamiento >= texto.length())
  {
    indiceDesplazamiento = 0;
  }

  lcd1.setCursor(0, 0);

  for(int i = 0; i < 16; i++)
  {
    int posicion = indiceDesplazamiento + i;

    if(posicion < texto.length())
    {
      lcd1.print(texto[posicion]);
    }
    else
    {
      lcd1.print(" ");
    }
  }

  indiceDesplazamiento++;

  if(indiceDesplazamiento >= texto.length())
  {
    indiceDesplazamiento = 0;
  }
}

void LCDMostrarTiempo()
{
  lcd1.setCursor(0, 1);

  if(HorasLCD < 10)
  {
    lcd1.print("0");
  }

  lcd1.print(HorasLCD);
  lcd1.print(":");

  if(MinutosLCD < 10)
  {
    lcd1.print("0");
  }

  lcd1.print(MinutosLCD);
  lcd1.print(":");

  if(SegundosLCD < 10)
  {
    lcd1.print("0");
  }

  lcd1.print(SegundosLCD);
  lcd1.print("       ");
}

void LCDActualizarTiempo()
{
  SegundosLCD++;

  if(SegundosLCD >= 60)
  {
    SegundosLCD = 0;
    MinutosLCD++;
  }

  if(MinutosLCD >= 60)
  {
    MinutosLCD = 0;
    HorasLCD++;
  }

  if(HorasLCD >= 24)
  {
    HorasLCD = 0;
  }
}

char LeerTecla()
{
  char tecla = '\0';

  digitalWrite(fila1, LOW);

  if(digitalRead(columna1) == LOW)
  {
    tecla = '1';
  }
  else if(digitalRead(columna2) == LOW)
  {
    tecla = '2';
  }
  else if(digitalRead(columna3) == LOW)
  {
    tecla = '3';
  }
  else if(digitalRead(columna4) == LOW)
  {
    tecla = 'A';
  }

  digitalWrite(fila1, HIGH);

  if(tecla != '\0')
  {
    delay(100);
    return tecla;
  }

  digitalWrite(fila2, LOW);

  if(digitalRead(columna1) == LOW)
  {
    tecla = '4';
  }
  else if(digitalRead(columna2) == LOW)
  {
    tecla = '5';
  }
  else if(digitalRead(columna3) == LOW)
  {
    tecla = '6';
  }
  else if(digitalRead(columna4) == LOW)
  {
    tecla = 'B';
  }

  digitalWrite(fila2, HIGH);

  if(tecla != '\0')
  {
    delay(100);
    return tecla;
  }

  digitalWrite(fila3, LOW);

  if(digitalRead(columna1) == LOW)
  {
    tecla = '7';
  }
  else if(digitalRead(columna2) == LOW)
  {
    tecla = '8';
  }
  else if(digitalRead(columna3) == LOW)
  {
    tecla = '9';
  }
  else if(digitalRead(columna4) == LOW)
  {
    tecla = 'C';
  }

  digitalWrite(fila3, HIGH);

  if(tecla != '\0')
  {
    delay(100);
    return tecla;
  }

  digitalWrite(fila4, LOW);

  if(digitalRead(columna1) == LOW)
  {
    tecla = '*';
  }
  else if(digitalRead(columna2) == LOW)
  {
    tecla = '0';
  }
  else if(digitalRead(columna3) == LOW)
  {
    tecla = '#';
  }
  else if(digitalRead(columna4) == LOW)
  {
    tecla = 'D';
  }

  digitalWrite(fila4, HIGH);

  if(tecla != '\0')
  {
    delay(100);
    return tecla;
  }

  return '\0';
}

void AjustarHora(char tecla)
{
  if(tecla == 'A')
  {
    teclaHoras = true;
    teclaMinutos = false;
    teclaSegundos = false;
  }

  else if(tecla == 'B')
  {
    teclaHoras = false;
    teclaMinutos = true;
    teclaSegundos = false;
  }

  else if(tecla == 'C')
  {
    teclaHoras = false;
    teclaMinutos = false;
    teclaSegundos = true;
  }

  else if(tecla >= '0' && tecla <= '9')
  {
    int numero = tecla - '0';

    if(teclaHoras)
    {
      HorasLCD += numero;
      teclaHoras = false;
      LCDMostrarTiempo();
    }

    else if(teclaMinutos)
    {
      MinutosLCD += numero;

      if(MinutosLCD >= 60)
      {
        MinutosLCD = MinutosLCD % 60;
      }

      teclaMinutos = false;
      LCDMostrarTiempo();
    }

    else if(teclaSegundos)
    {
      SegundosLCD += numero;

      if(SegundosLCD >= 60)
      {
        SegundosLCD = SegundosLCD % 60;
      }

      teclaSegundos = false;
      LCDMostrarTiempo();
    }
  }
}