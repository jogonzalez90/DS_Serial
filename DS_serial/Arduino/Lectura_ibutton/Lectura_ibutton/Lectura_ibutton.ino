#include <OneWire.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>                      //Librería para escribir leer EEROM

//Pin para habilitar el transceptor del SN75176 escritura en el bus AB
#define DE 5

//Pines lectura de chips DS1990 4 unidades
OneWire  iB1(11);                            // Cara a corresponde a D11
OneWire  iB2(12);                            // Cara b corresponde a D12
OneWire  iB3(2);                             // Cara c corresponde a D2
OneWire  iB4(3);                             // Cara d corresponde a D3

char X;                                      //Variabe que deternima la cara de lectura surtidor
int X1 = 0;                                  //Para lectura de iButton guarda el caracter igresado en numérico
char inChar;                                 //Guarda valor ingresado por teclado parq configuración ID's
char inChar_2;                               //Guarda segundo valor (decenas)
int contador = 0;                            //Contador para controlar a que ID se va a ingresar dato
int numero = 0;                              //Almacena el valor ingresado a numérico

//Varaibles para valor ingresado en decenas
int numero_dec = 0;                          //Almacena valor en decenas
int numero_und = 0;                          //Almacena valor en unidades

//Recuperacion de informacion desde EEPROM
int ID1 = EEPROM.read(1);                       
int ID2 = EEPROM.read(2);                      
int ID3 = EEPROM.read(3);                          
int ID4 = EEPROM.read(4); 
                
byte addr1[] = {1 , 2, 3, 4, 5, 6, 7, 8};    //Array para guardar direccion de iButton
byte Longitud = 7;                           //Longitud array que contiene el numero de caracterres iButton

SoftwareSerial mySerial(4, 6); // RX, TX

void setup(void) {
  mySerial.begin(9600);
  pinMode(DE, OUTPUT);
  digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
  mySerial.println("Si desea configurar ID's, ingresar 'p' para programar.");              //Inicio programa
  mySerial.println("Espere un momento, por favor...");            
  delay(1500);
  mySerial.println("Dispositivo activo.");
  digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
}

void loop(void) 
{
    if (mySerial.available() > 0)//Validación de datos recibidido en puerto serial, cara de surtidor a leer
    {
    delay(100);
    X = mySerial.read();
    inChar_2 = mySerial.read();

    limpiarBuffermySerial();
    while (X=='p')                                //Programacion de aplicacion, se asignan los ID a cada puerto del arduino
     {  
     if (mySerial.available() > 1) 
     {
      delay(100);
      inChar = mySerial.read(); // Lee el caracter recibido
      inChar_2 = mySerial.read();
      limpiarBuffermySerial();
      if (isdigit(inChar) && isdigit(inChar_2)) 
      {
        numero_dec = inChar - '0';
        numero_und = inChar_2 - '0';

        numero = (numero_dec * 10) + numero_und;          //Se calcula el valor de los dos digitos
      }
      contador = contador + 1;
      if (contador == 1)
          {
            EEPROM.write(contador, numero);              //Escritura en EEPROM
            ID1 = EEPROM.read(contador);                 //Lectura de EEPROM
            digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
            mySerial.print("Caracter ingresado para M1 = ");
            mySerial.println(ID1);
            digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
         }
         else if (contador == 2)
       {
          EEPROM.write(contador, numero);                //Escritura en EEPROM
          ID2 = EEPROM.read(contador);                   //Lectura de EEPROM
          digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
          mySerial.print("Caracter ingresado para M2 = ");
          mySerial.println(ID2);
          digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
       }
       else if (contador == 3)
     {
        EEPROM.write(contador, numero);                  //Escritura en EEPROM
        ID3 = EEPROM.read(contador);                     //Lectura de EEPROM
        digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
        mySerial.print("Caracter ingresado para M3 = ");
        mySerial.println(ID3);
        digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
     }
       else if (contador == 4)
    {
      EEPROM.write(contador, numero);                    //Escritura en EEPROM
      ID4 = EEPROM.read(contador);                       //Lectura de EEPROM
      digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
      mySerial.print("Caracter ingresado para M4 = ");
      mySerial.println(ID4);
      digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
      contador = 0;
      X = 'o';                                //Finaliza programacion de ID sale de while e ingresa a lectura de iButton
     }
     
      limpiarBuffermySerial();
     }
     }

      if (isdigit(X) && isdigit(inChar_2)) 
      {
        numero_dec = X - '0';
        numero_und = inChar_2 - '0';

        X1 = (numero_dec * 10) + numero_und;          //Se calcula el valor de los dos digitos
      }

      if (X == 'l')                                   //Visualiza el ID de cada puerto
    { 
        digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
        mySerial.print("Caracter ingresado para M1 = ");
        mySerial.println(ID1);
        mySerial.print("Caracter ingresado para M2 = ");
        mySerial.println(ID2);
        mySerial.print("Caracter ingresado para M3 = ");
        mySerial.println(ID3);
        mySerial.print("Caracter ingresado para M4 = ");
        mySerial.println(ID4);
        digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
    }
     
    if ((iB1.search(addr1) != 0) && (X1==ID1))  //Lectura cara a
    {
      digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
      mySerial.println("Enviando M1 =");
      adicion (addr1, Longitud, X);               //Funcion para leer iButton
      end_trama (X);
      digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
      
      if ((iB1.search(addr1) != 0) && (X1==ID1)) //Corrección lectura
      {
      }
    }
    else if (X1==ID1)                           //Falla de lectura por ausencia de dispositivo en cara a
    
    {
      digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
      mySerial.print("M1 no detectado.\n");
      digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
    }

    if ((iB2.search(addr1) != 0) && (X1==ID2))  //Lectura cara b
    {
      digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
      mySerial.println("Enviando M2 =");
      adicion (addr1, Longitud, X);               //Funcion para leer iButton
      end_trama (X);
      digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
     
      if ((iB2.search(addr1) != 0) && (X1==ID2)) //Corrección lectura
      {
      }
    }
    else if (X1==ID2)                           //Falla de lectura por ausencia de dispositivo en cara b
    {
      digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
      mySerial.print("M2 no detectado.\n");
      digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
    }
    
    if ((iB3.search(addr1) != 0) && (X1==ID3)) //Lectura cara c
    {
      digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
      mySerial.println("Enviando M3 =");
      adicion (addr1, Longitud, X);             //Funcion para leer iButton
      end_trama (X);
      digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
      
      if ((iB3.search(addr1) != 0) && (X1==ID3)) //Corrección lectura
      {
      }
    }
    else if (X1==ID3)                          //Falla de lectura por ausencia de dispositivo en cara c
    {
      digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
      mySerial.print("M3 no detectado.\n");
      digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
    }

    if ((iB4.search(addr1) != 0) && (X1==ID4)) //Lectura cara d
    {
      digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
      mySerial.println("Enviando M4 =");
      adicion (addr1, Longitud, X);              //Funcion para leer iButton
      end_trama (X);
      digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
      
      if ((iB4.search(addr1) != 0) && (X1==ID4)) //Corrección lectura
      {
      }
    }
    else if (X1==ID4)                          //Falla de lectura por ausencia de dispositivo en cara c
    {
      digitalWrite(DE, HIGH);         //Se habilita para escritura el SN75176
      mySerial.print("M4 no detectado.\n");
      digitalWrite(DE, LOW);           ////Se habilita para lectura el SN75176
    }
    }
}


void limpiarBuffermySerial() { //Funcion para borrar dato adional del Buffer
  while (mySerial.available() > 0) {
    char datoDescartado = mySerial.read(); // Leer y descartar el dato adicioanl 
  }
}
  
/***********************************************************************
************Funcion encargada de agregar un 0X00 a los números**********
**********inferiores a 0X10, para cumplimentar los 16 caracteres********
*************del iButton e imprimir  dato por puerto serial ************
***********************************************************************/

void adicion(byte addr1[],byte Longitud, char X){    //Imprime inicio de trama dirrección iButton
  mySerial.print("@");
  for( Longitud = Longitud; Longitud > 0; Longitud--)
      {
         if (addr1[Longitud] < 0x10) {//Agrega 0X00
           mySerial.print("0");
         }
        mySerial.print(addr1[Longitud], HEX);
      }
      if (addr1[Longitud] < 0x10) {                    //Agrega 0X00
           mySerial.print("0");
         }
      mySerial.print(addr1[0], HEX);
      mySerial.print(":");
    }

void end_trama(char X){ //Imprime final de trama
      mySerial.print(X1);
      mySerial.print("-");
      mySerial.println();
  }
