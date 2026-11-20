#include <Wire.h>
#include <LiquidCrystal_I2C.h>

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

// Valores
int ValorHumedad;
int ValorHumedadREAL;

int ValorTemperatura;
int ValorTemperaturaREAL;

int ValorViento;

int HorasLCD = 0;
int MinutosLCD = 0;
int SegundosLCD = 0;

bool teclaHoras = false;
bool teclaSegundos = false;
bool teclaMinutos = false;

String TextoHumedad = "";
String TextoTemperatura = "";
String TextoViento = "";
String TextoLCD = "";

int indiceDesplazamiento = 0;

LiquidCrystal_I2C lcd1(0x27, 16, 2);

unsigned long tiempoAnterior = 0;

void setup()
{
  Serial.begin(9600);
  
  lcd1.init();
  lcd1.backlight();
 
  pinMode(SensorHumedad, INPUT);
  pinMode(SensorTemperatura, INPUT);
  pinMode(SensorViento, INPUT);

  for (int i = 0; i < 4; i++)
  {
    pinMode(filas[i], OUTPUT);
    digitalWrite(filas[i], HIGH);
  }

  for (int j = 0; j < 4; j++)
  {
    pinMode(columnas[j], INPUT_PULLUP);
  }
}

void loop()
{
  DetectarHumedad();
  DetectarTemperatura();
  DetectarViento();
 
  // Construimos el texto antes de mandarlo a mostrar
  TextoLCD = TextoHumedad + " | " + TextoTemperatura + " | " + TextoViento + "         ";
  LCDMostrar(TextoLCD);
  LCDMostrarTiempo();
  
  AjustarHora();
  
  if (millis() - tiempoAnterior >= 1000)
  {
    tiempoAnterior = millis();
    LCDActualizarTiempo();
  }
  
  delay(1000); 
}

void LCDMostrar(String texto)
{
  lcd1.setCursor(0, 0);
  String textoFragmento = texto.substring(indiceDesplazamiento, indiceDesplazamiento + 16);
 
  while (textoFragmento.length() < 16)
  {
    textoFragmento += " ";
  }
  lcd1.print(textoFragmento);
  
  indiceDesplazamiento++;
  if (indiceDesplazamiento >= texto.length() - 15) 
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
  else if (ValorHumedadREAL <= 70 && ValorHumedadREAL > 35)
  {
   TextoHumedad = "Esta humedo.";
  }
  else if (ValorHumedadREAL <= 35)
  {
   TextoHumedad = "Esta seco/despejado.";
  }

  Serial.print("Humedad: ");
  Serial.print(ValorHumedadREAL);
  Serial.print("% | Estado: ");
  Serial.println(TextoHumedad);
}

void DetectarTemperatura()
{
  ValorTemperatura = analogRead(SensorTemperatura);
  ValorTemperaturaREAL = map(ValorTemperatura, 18, 360, -40, 126);
 
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

  Serial.print("Temperatura: ");
  Serial.print(ValorTemperaturaREAL);
  Serial.print("°C | Estado: ");
  Serial.println(TextoTemperatura);
}

void DetectarViento()
{
  ValorViento = analogRead(SensorViento);
  if (ValorViento <= 49)
  {
   TextoViento = "Brisa muy debil.";
  }
  else if (ValorViento > 49 && ValorViento <= 140)
  {
   TextoViento = "Brisa moderada.";
  }
  else if (ValorViento > 140 && ValorViento <= 354)
  {
   TextoViento = "Viento fresco.";
  }
}

void LCDMostrarTiempo()
{
  lcd1.setCursor(0, 1);
 
  if (HorasLCD < 10) lcd1.print("0");
  lcd1.print(HorasLCD);
  lcd1.print(":");
 
  if (MinutosLCD < 10) lcd1.print("0");
  lcd1.print(MinutosLCD);
  lcd1.print(":");
 
  if (SegundosLCD < 10) lcd1.print("0");
  lcd1.print(SegundosLCD);
  lcd1.print("    ");
}

void LCDActualizarTiempo()
{
  SegundosLCD++;
  if(SegundosLCD > 59)
  {
    SegundosLCD = 0;
    MinutosLCD++;
  }
 
  if (MinutosLCD > 59)
  {
    MinutosLCD = 0;
    HorasLCD++;
  }

  if (HorasLCD > 23)
  {
    HorasLCD = 0;
  }
}

void AjustarHora()
{
   for(int i = 0; i < 4; i++)
   {
      digitalWrite(filas[i], LOW);

      if(digitalRead(columnas[3]) == LOW)
      {
         if(i == 0)
         {
            teclaHoras = HIGH;
            teclaMinutos = LOW;  
            teclaSegundos = LOW;  
         }
         else if(i == 1)
         {
            teclaHoras = LOW;  
            teclaMinutos = HIGH;
            teclaSegundos = LOW;  
         }
         else if(i == 2)
         {
            teclaHoras = LOW;  
            teclaMinutos = LOW;  
            teclaSegundos = HIGH;
         }
         delay(250);
      }
      digitalWrite(filas[i], HIGH);
   }
 
   int valores[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

   for(int i = 0; i < 3; i++)
   {
      digitalWrite(filas[i], LOW);

      for(int j = 0; j < 3; j++)
      {
         if(digitalRead(columnas[j]) == LOW)
         {
            int numeroPresionado = valores[i][j];

            Serial.print("Tecla presionada: ");
            Serial.println(numeroPresionado);

            if(teclaHoras == HIGH)
            {
               HorasLCD += numeroPresionado;
               if (HorasLCD >= 24) HorasLCD %= 24;
               teclaHoras = LOW;
            }
            else if (teclaMinutos)
            {
               MinutosLCD += numeroPresionado;
               if (MinutosLCD >= 60) MinutosLCD %= 60;
               teclaMinutos = LOW;
            }
            else if (teclaSegundos)
            {
               SegundosLCD += numeroPresionado;
               if (SegundosLCD >= 60) SegundosLCD %= 60;  
               teclaSegundos = LOW;
            }
            delay(200);
         }
      }
      digitalWrite(filas[i], HIGH);
   }
}