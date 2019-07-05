#include <SPI.h>
#include <stdint.h>
#include <Arduino.h>

void uart_setup();

void spi_setup();

void write_spi();

enum UART_CMD {
    CIRCLE_CLOCKWISE = 0x10,
    CIRCLE_COUNTERCLOCKWISE = 0x11,
    SWIPE_TO_RIGHT = 0x12,
    SWIPE_TO_LEFT = 0x13
};

// モータのビットデータ
const uint64_t InitData[]{
        0b111111111011000000010111100000000000000000000000,
        0b111111111011000000010000000001100000000000000000,
        0b111111111011000000010000100000000001011110000000,
        0b111111111011000000010001000001010000000010010000,
        0b111111111011000000010001100000000000000100111000,
        0b111111111011000000010001100001000000000001001000,
        0b111111111011000000010001100010000000000100111000,
        0b111111111011000000010001100011000000000001001000,
        0b111111111011000000010010000001010000000000001000,
};

const uint64_t MoveData[]{
        0b111111111011000000000111111000111111011111111000,
        0b111111111011000000000111111000100000000000001000,
};

void setup() {
    uart_setup();
    spi_setup();
}

void spi_setup() {
    pinMode(SS, OUTPUT); //デジタル10番ピンをOUTPUTに設定
    digitalWrite(SS, HIGH); //デジタルの10番ピンからHighを出力
    SPI.setBitOrder(MSBFIRST); // 下位ビットから送信
    SPI.setClockDivider(SPI_CLOCK_DIV8); //分周器1/8を指定 同期クロック16/8=2MHz
    SPI.setDataMode(SPI_MODE3); //アイドル時Low、立上がりエッジでデータ取得
    SPI.begin(); //SPI通信の初期化、有効化
}

void uart_setup() {
    Serial.begin(9600);
}

bool Flg{true}; //なんのフラグだろ...

void write_spi() {
    if (Flg) {
        Flg = false;
        // 制御するデバイスに通信の開始を通知する
        digitalWrite(SS, LOW);
        for (uint64_t data:InitData) {
            SPI.transfer(&data, 6);
            delay(10); //必要に応じて設定
        }
        for (uint64_t data:MoveData) {
            SPI.transfer(&data, 6);
            delay(10); //必要に応じて設定
        }
        // 制御するデバイスに通信の終了を通知する
        digitalWrite(SS, HIGH);
    }
}


void loop() {

    switch (Serial.read()) {

        case CIRCLE_CLOCKWISE:
            write_spi();
            break;

        case CIRCLE_COUNTERCLOCKWISE:
            write_spi();
            break;

        case SWIPE_TO_LEFT:
            write_spi();
            break;

        case SWIPE_TO_RIGHT:
            write_spi();
            break;

        case -1:
            // TODO : 接続のエラー処理
            break;

        default:
            // TODO: 例外処理
            break;
    }
}




