#include <SPI.h>
#include <stdint.h>

// モータのビットデータ
uint64_t InitData[]{
        0b111111111011000000010000000001100000000000000000,
        0b111111111011000000011000000001100000000000000000,
        0b111111111011000000010111100000000000000000000000,
        0b111111111011000000010000100000000000011110000000,
        0b111111111011000000011000100000000000011110000000,
        0b111111111011000000010010000000000000000000000000,
        0b111111111011000000011010000000000000000000000000,
};

uint64_t MoveData[]{
        0b111111111011000000000110011001100001011110100000,
        0b111111111011000000010011100000000000000000010000,
        0b111111111011000000000110101000001111011111111000,
};

void setup() {
    Serial.begin(9600);
    SPI.setBitOrder(MSBFIRST);  //最上位ビット(MSB)から送信
    SPI.setClockDivider(SPI_CLOCK_DIV4);  //通信速度をデフォルト
    SPI.setDataMode(SPI_MODE2);   //アイドル5Vで0V→5Vの変化で送信する
    SPI.begin();  //開始
}

void loop() {

    for (uint64_t data:MoveData) {
        unsigned char b = data >> 18;
        Serial.println(b, BIN);
        Serial.print("\n");
    }

    delay(1000);
    
    if (Serial.available() > 0) {
        Serial.println(1);
        // 制御するデバイスに通信の開始を通知する
        digitalWrite(SS, LOW);

        for (uint64_t data:InitData) {
            SPI.transfer(data, 6);
        }

        for (uint64_t data:MoveData) {
            SPI.transfer(data, 6);
        }

        // 制御するデバイスに通信の終了を通知する
        digitalWrite(SS, HIGH);
    }
}