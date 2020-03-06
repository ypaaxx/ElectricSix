#include <Servo.h>

Servo one, two, three, four, five, six; //выход на сервы

unsigned short c1, c2, c3, c4, c5, c6; //счётчики
bool cs1, cs2, cs3, cs4, cs5, cs6; //индикаторы изменения сигнала на пине
bool ocs1, ocs2, ocs3, ocs4, ocs5, ocs6;
unsigned short ms1, ms2, ms3, ms4, ms5, ms6; // текущее значение микросекунд на сервах
unsigned char message [13];
unsigned char buf[13];

int INITMS = 800;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  //Приём
  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(8, INPUT);
  pinMode(7, INPUT);
  pinMode(4, INPUT);
  pinMode(2, INPUT);

  one.attach(11);
  two.attach(10);
  three.attach(9);
  four.attach(6);
  five.attach(5);
  six.attach(3);

  //Инициализация двигателя

  one.writeMicroseconds(INITMS);
  two.writeMicroseconds(INITMS);
  three.writeMicroseconds(INITMS);
  four.writeMicroseconds(INITMS);
  five.writeMicroseconds(INITMS);
  six.writeMicroseconds(INITMS);
  delay(3000);

  one.writeMicroseconds(2300);
  two.writeMicroseconds(2300);
  three.writeMicroseconds(2300);
  four.writeMicroseconds(2300);
  five.writeMicroseconds(2300);
  six.writeMicroseconds(2300);
  delay(700);

  one.writeMicroseconds(1100);
  two.writeMicroseconds(1100);
  three.writeMicroseconds(1100);
  four.writeMicroseconds(1100);
  five.writeMicroseconds(1100);
  six.writeMicroseconds(1100);
}

void loop() {
  /*  Ищем изменения с прошлой итерации
      Если изменение произошло, то увеличивается счётчик
  */
  ocs1 = cs1; //Значение сигнала на прошлой итерации
  ocs2 = cs2;
  ocs3 = cs3;
  ocs4 = cs4;
  ocs5 = cs5;
  ocs6 = cs6;
  cs1 = digitalRead(13); //Новое значение сигнала
  cs2 = digitalRead(12);
  cs3 = digitalRead(8);
  cs4 = digitalRead(7);
  cs5 = digitalRead(4);
  cs6 = digitalRead(2);
  c1 += cs1 ^ ocs1; //Увеличение счётчика на результат сравнения
  c2 += cs2 ^ ocs2;
  c3 += cs3 ^ ocs3;
  c4 += cs4 ^ ocs4;
  c5 += cs5 ^ ocs5;
  c6 += cs6 ^ ocs6;
}

void serialEvent() {

  if (Serial.available() < 13) return;
  Serial.readBytes(message, 13);
  while ( Serial.available() > 0 ) {
    Serial.read();
  }

  if (message[12] != (byte) crc8(message, 12)) return;

  //Устанавливаются микросекунды на сервах
  ms1 = message[0] << 8;
  ms1 += message[1] & 0xFF;

  ms2 = message[2] << 8;
  ms2 += message[3] & 0xFF;

  ms3 = message[4] << 8;
  ms3 += message[5] & 0xFF;

  ms4 = message[6] << 8;
  ms4 += message[7] & 0xFF;

  ms5 = message[8] << 8;
  ms5 += message[9] & 0xFF;

  ms6 = message[10] << 8;
  ms6 += message[11] & 0xFF;

  //Присваиваем новые значения
  one.writeMicroseconds(ms1);
  two.writeMicroseconds(ms2);
  three.writeMicroseconds(ms3);
  four.writeMicroseconds(ms4);
  five.writeMicroseconds(ms5);
  six.writeMicroseconds(ms6);

  //Передача со счётчиков
  buf[0] = c1 >> 8;
  buf[1] = c1 & 0xFF;

  buf[2] = c2 >> 8;
  buf[3] = c2 & 0xFF;

  buf[4] = c3 >> 8;
  buf[5] = c3 & 0xFF;

  buf[6] = c4 >> 8;
  buf[7] = c4 & 0xFF;

  buf[8] = c5 >> 8;
  buf[9] = c5 & 0xFF;

  buf[10] = c6 >> 8;
  buf[11] = c6 & 0xFF;

  buf[12] = crc8(buf, 12);

  Serial.write(buf, 13);

  //Обнуление счётчиков
  c1 = 0;
  c2 = 0;
  c3 = 0;
  c4 = 0;
  c5 = 0;
  c6 = 0;
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
