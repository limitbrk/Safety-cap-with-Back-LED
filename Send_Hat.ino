#include <RCSwitch.h>
#include <Wire.h>
#define ADXL345_ADDRESS (0xA6 >> 1) 
#define ADXL345_REGISTER_XLSB (0x32)
int accelerometer_data[3];
int turn_stat = 9;
int temp;
void turn_blink(int led){
  digitalWrite(led,!digitalRead(led));
}

void i2c_write(int address, byte reg, byte data) {
 Wire.beginTransmission(address);
 Wire.write(reg);
 Wire.write(data); //low byte
 Wire.endTransmission();
}
void i2c_read(int address, byte reg, int count, byte* data) {
 int i = 0;
 Wire.beginTransmission(address);
 Wire.write(reg);
 Wire.endTransmission();
 Wire.beginTransmission(address);
 Wire.requestFrom(address, count);
 while(Wire.available()){
 char c = Wire.read(); 
 data[i] = c;
 i++;
 }
 Wire.endTransmission();
}
void init_adxl345() {
 byte data = 0;
 i2c_write(ADXL345_ADDRESS, 0x31, 0x0B); // 13-bit mode +_ 16g
 i2c_write(ADXL345_ADDRESS, 0x2D, 0x08); // Power register
 i2c_write(ADXL345_ADDRESS, 0x1E, 0x00); // x
 i2c_write(ADXL345_ADDRESS, 0x1F, 0x00); // Y
 i2c_write(ADXL345_ADDRESS, 0x20, 0x05); // Z
 i2c_read(ADXL345_ADDRESS, 0X00, 1, &data);

 if(data==0xE5)
 Serial.println("it work Success");
 else
 Serial.println("it work Fail");
}
void read_adxl345() {
 byte bytes[6];
 memset(bytes,0,6);
 i2c_read(ADXL345_ADDRESS, ADXL345_REGISTER_XLSB, 6, bytes);
 for (int i=0;i<3;++i) {
 accelerometer_data[i] = (int)bytes[2*i] + (((int)bytes[2*i + 1]) << 8);
 }
}
//----------------------------------------------------------

RCSwitch mySwitch = RCSwitch(); // เริ่มต้นใช้งานไลบารี่

void setup() {
  Serial.begin(9600);
  mySwitch.enableTransmit(10); // กำหนดขาใช้งานเป็นขาที่ 10

  Wire.begin();
 Serial.begin(9600);
 for(int i=0; i<3; ++i) {
 accelerometer_data[i] = 0;
 }
 init_adxl345();
}

void loop() {
read_adxl345();
 
 Serial.print("ACCEL: ");
 float x = (accelerometer_data[0])*3.9/10;
 Serial.print(x);//3.9mg/LSB scale factor in 13-bit mode
 Serial.print("\t");
 float y = (accelerometer_data[1])*3.9/10;
 Serial.print(y);
 Serial.print("\t");
 float z = (accelerometer_data[2])*3.9/10;
 Serial.print(z);
 Serial.print("\n");

 // ---------------------ADDING PART -----------------------
 if(x>30.0)
  turn_stat = 2; //right
 else if(x<-30.0)
  turn_stat = 1; //left
 else if(z>15)
  turn_stat = 9; //no turn
  else if(z<-60.0)
  turn_stat = 8; //pic
 

  Serial.print("Return stat :");
  Serial.print(turn_stat);
  Serial.print("\n");
  turn_blink(A1);

  
 switch(turn_stat){
  case 1 : 
    mySwitch.send(0xAE01, 16); 
    Serial.println("Send Left."); 
    break; 
  
  case 2 :
    mySwitch.send(0xAE02, 16);
    Serial.println("Send Right."); 
    break; 
  
  case 9 : 
    mySwitch.send(0xAE09, 16);
    Serial.println("Send None."); 
    break;

  case 8 : 
    mySwitch.send(0xAE08, 16);
    Serial.println("Send pic."); 
    break;
 }
 // ----------------- END of ADDING PART -------------------
 delay(100);
}
