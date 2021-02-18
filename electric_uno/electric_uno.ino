#include <Servo.h>

Servo one; //выход на сервы

unsigned short c; //счётчики
bool cs; //индикаторы изменения сигнала на пине
bool ocs;
unsigned short ms; // текущее значение микросекунд на сервах
unsigned char message [13];
unsigned char buf[13];

int INITMS = 800;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  //Приём
  pinMode(6, INPUT);
  pinMode(5, OUTPUT); //питание датчика rpm
  digitalWrite(5, HIGH);
  pinMode(4, OUTPUT);// земля датчика rpm
  digitalWrite(4, LOW);
  pinMode(2, OUTPUT); //питание bec
  digitalWrite(2, HIGH);


  one.attach(3);
}

void loop() {
  /*  Ищем изменения с прошлой итерации
      Если изменение произошло, то увеличивается счётчик
  */
  ocs = cs; //Значение сигнала на прошлой итерации
  cs = digitalRead(6); //Новое значение сигнала
  c += cs ^ ocs; //Увеличение счётчика на результат сравнения
}

void serialEvent() {

  if (Serial.available() < 13) return;
  Serial.readBytes(message, 13);
  while ( Serial.available() > 0 ) {
    Serial.read();
  }

  if (message[12] != (byte) crc8(message, 12)) return;

  //Устанавливаются микросекунды на сервах
  ms = message[0] << 8;
  ms += message[1] & 0xFF;


  //Присваиваем новые значения
  one.writeMicroseconds(ms);

  //Передача со счётчиков
  buf[0] = c >> 8;
  buf[1] = c & 0xFF;

  buf[12] = crc8(buf, 12);

  Serial.write(buf, 13);

  //Обнуление счётчиков
  c = 0;

}

/* Расчёт CRC-8 */
unsigned char crc8(unsigned char *pcBlock, unsigned char len)
{
  unsigned char crc = 0x00;
  unsigned char i;

  while (len--)
  {
    crc ^= *pcBlock++;

    for (i = 0; i < 8; i++)
      crc = crc & 0x80 ? (crc << 1) ^ 0x07 : crc << 1;
  }

  return crc;
}
