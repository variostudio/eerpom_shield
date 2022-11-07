#define data_pin_cnt 8
#define addr_pin_cnt 16

#define ce_pin 23 //CE# - chip enable, active low
#define oe_pin 20 //OE# - output enable, active low
#define we_pin 22 //WE# - write enable, active low

int currentAddress = 0;
int addr_pins[] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
int data_pins[] = {24,25,26,2ww7,28,29,30,31};

void toAddressPins(int value) {
  for (int i = 0, mask = 1; i < addr_pin_cnt; i++, mask = mask << 1) {
    if (value & mask) {
      digitalWrite(addr_pins[i], HIGH);
    } else {
      digitalWrite(addr_pins[i], LOW);
    }
  }
}

byte readByte() {
  // read the current eight-bit byte being output by the ROM ...
  byte res = 0;x

  for (int i = 0, mask = 1; i < data_pin_cnt; i++, mask = mask << 1) {
    if (digitalRead(data_pins[i]) == 1) {
      res |= mask;
    }
  }
  
  return(res);
}

void dataOut() {
  for (int i = 0; i < data_pin_cnt; i++) {  //Initialize data pins
    pinMode(data_pins[i], OUTPUT);
  }  
}

void dataIn() {
  for (int i = 0; i < data_pin_cnt; i++) {  //Initialize data pins
    pinMode(data_pins[i], INPUT);
  }  
}

void writeByte(byte value) {
  digitalWrite(oe_pin, HIGH);
  digitalWrite(ce_pin, LOW);

  for (int i = 0, mask = 1; i < data_pin_cnt; i++, mask = mask << 1) {
    if (value & mask) {
      digitalWrite(data_pins[i], HIGH);
    } else {
      digitalWrite(data_pins[i], LOW);
    }
  }

  delay(10);
  digitalWrite(we_pin, LOW);
  delay(10);
  digitalWrite(we_pin, HIGH);  
  delay(10);
}

void programByte(int addr, byte val) {
  int addr55 = 0x5555;
  int addr2a = 0x2aaa;

  toAddressPins(addr55);
  writeByte(0xAA); 

  toAddressPins(addr2a);
  writeByte(0x55);

  toAddressPins(addr55);
  writeByte(0xA0);   

  toAddressPins(addr);
  writeByte(val);  
}


void getChipId() {
  int addr55 = 0x5555;
  int addr2a = 0x2aaa;

  dataOut();

  toAddressPins(addr55);
  writeByte(0xAA);

  toAddressPins(addr2a);
  writeByte(0x55);

  toAddressPins(addr55);
  writeByte(0x90);

  dataIn();

  delay(500);
  toAddressPins(0);

  byte res = readByte();

  Serial.print("ID: ");
  Serial.println(res, HEX);


  toAddressPins(1);

  res = readByte();

  Serial.print("ID: ");
  Serial.println(res, HEX);  

  dataOut();

  toAddressPins(addr55);
  writeByte(0xf0);
}

void sectorErase() {
  int addr55 = 0x5555;
  int addr2a = 0x2aaa;

  toAddressPins(addr55);
  writeByte(0xAA);

  toAddressPins(addr2a);
  writeByte(0x55);

  toAddressPins(addr55);
  writeByte(0x80);

  toAddressPins(addr55);
  writeByte(0xAA);

  toAddressPins(addr2a);
  writeByte(0x55);

  toAddressPins(0);
  writeByte(0x30); 
}

void chipErase() {
  int addr55 = 0x5555;
  int addr2a = 0x2aaa;

  toAddressPins(addr55);
  writeByte(0xAA);

  toAddressPins(addr2a);
  writeByte(0x55);

  toAddressPins(addr55);
  writeByte(0x80);

  toAddressPins(addr55);
  writeByte(0xAA);

  toAddressPins(addr2a);
  writeByte(0x55);

  toAddressPins(addr55);
  writeByte(0x10);
}

void initPins() {
  pinMode(ce_pin, OUTPUT);
  digitalWrite(ce_pin, HIGH);  //Active low

  pinMode(oe_pin, OUTPUT);
  digitalWrite(oe_pin, HIGH);  //Active low

  pinMode(we_pin, OUTPUT);
  digitalWrite(we_pin, HIGH);  //Active low

  for (int i = 0; i < addr_pin_cnt; i++) {  //Initialize address pins
    pinMode(addr_pins[i], OUTPUT);
    digitalWrite(addr_pins[i], LOW);
  }

  digitalWrite(ce_pin, LOW);  //Activate the chip
  digitalWrite(oe_pin, HIGH);  //Deactivate output
}

void setup() {
  initPins();

  Serial.begin(9600); 

  // getChipId();
  // dataOut();
  // sectorErase();
  // chipErase();

  // delay(1000);

  // programByte(0, 0x15);
  // programByte(1, 0x11);
  // programByte(2, 0x12);
  // programByte(3, 0x13);
  
  // delay(1000);
  // initPins();

  dataIn();

  for (int addr=0; addr<8; addr++) {
    toAddressPins(addr);
    digitalWrite(oe_pin, LOW);
    byte res = readByte();

    Serial.print("Address : ");
    Serial.print(addr);
    Serial.print(", value : ");
    Serial.println(res, HEX);
  }

  //digitalWrite(addr_pins[10], HIGH);

  // for (int i=0; i<addr_pin_cnt; i++) {
  //   digitalWrite(addr_pins[i], HIGH);
  // }

  //digitalWrite(data_pins[7], HIGH);

  // getChipId();
}


void loop() {
  // toAddressPins(currentAddress); //Set pin address 

  // digitalWrite(oe_pin, LOW);  //Active output

  // byte res = readByte(); //Read byte

  // digitalWrite(oe_pin, HIGH);  //Deactivate output

  // Serial.print("Address : ");
  // Serial.print(currentAddress);
  // Serial.print(", value : ");
  // Serial.println(res);

  // currentAddress++;

  // if (currentAddress > 32) {
  //   currentAddress = 0;
  // }

  // delay(100);
}