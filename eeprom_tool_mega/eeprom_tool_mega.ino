#define data_pin_cnt 8
#define addr_pin_cnt 19 //Address pins number: SST39SF040 - 19, SST39SF020A - 18, SST39SF010A - 17

#define ce_pin 23 //CE# - chip enable, active low
#define oe_pin 21 //OE# - output enable, active low
#define we_pin 22 //WE# - write enable, active low
#define write_led 43
#define read_led 41

int currentAddress = 0;
int addr_pins[] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}; 
int data_pins[] = {24,25,26,27,28,29,30,31};


byte data[] = {0x1a, 0xa2, 0x15, 0xce, 0x20, 0xbb, 0xcc, 0x97};

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
  readLedOn();
  // read the current eight-bit byte being output by the ROM ...
  byte res = 0;

  for (int i = 0, mask = 1; i < data_pin_cnt; i++, mask = mask << 1) {
    if (digitalRead(data_pins[i]) == 1) {
      res |= mask;
    }
  }
  
  readLedOff();
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
  writeLedOn();
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
  writeLedOff();
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

  pinMode(write_led, OUTPUT);
  pinMode(read_led, OUTPUT);
}

void readLedOn() {
  digitalWrite(read_led, HIGH);
}

void readLedOff() {
  digitalWrite(read_led, LOW);
}

void writeLedOn() {
  digitalWrite(write_led, HIGH);
}

void writeLedOff() {
  digitalWrite(write_led, LOW);
}

void setup() {
  initPins();

  Serial.begin(9600); 
  Serial.println("Welcome to Arduino EEPROM SST39SF0xx programmer!");

  dataOut();
  chipErase();

  delay(1000);

  for (int idx=0; idx<sizeof(data)/sizeof(data[0]); idx++) {
    programByte(idx, data[idx]);
  }
  
  delay(1000);
  initPins();

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
}