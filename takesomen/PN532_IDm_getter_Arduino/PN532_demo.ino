
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  nfc.begin();

  uint32_t version = nfc.getFirmwareVersion();

  if (!version) {
    Serial.println("PN532 not found!");
    while (1) delay(10);
  }

  Serial.println("PN532 found.");
  nfc.SAMConfig();

  Serial.println("Waiting for FeliCa...");
}

void loop() {
  uint8_t idm[8] = {};
  uint8_t pmm[8] = {};
  uint16_t systemCodeResponse = 0;

  // 全システムを対象にし、追加情報を要求しない
  int8_t ret = nfc.felica_Polling(
    0xFFFF,
    0x00,
    idm,
    pmm,
    &systemCodeResponse,
    1000
  );

  Serial.print("Polling result: ");
  Serial.println(ret);

  if (ret == 1) {
    Serial.print("IDm: ");
    nfc.PrintHex(idm, 8);
    Serial.println();

    Serial.print("PMm: ");
    nfc.PrintHex(pmm, 8);
    Serial.println();
  } else if (ret == 0) {
    Serial.println("No FeliCa card detected.");
  } else {
    Serial.println("PN532 communication or polling error.");
  }

  delay(500);
}
