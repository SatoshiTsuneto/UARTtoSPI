#include <SPI.h>

#define BYTE_SIZE 6

const uint8_t test_data_set[2][BYTE_SIZE] = {
        {
                0b11111111,
                0b10110000,
                0b00000111,
                0b11100011,
                0b11110111,
                0b11111000,
        },
        {
                0b11111111,
                0b10110000,
                0b00000111,
                0b11100010,
                0b00000000,
                0b00001000,
        }
};

void setup() {
    Serial.begin(9600);
    // SPI通信のセットアップ
    pinMode(SS, OUTPUT); //デジタル10番ピンをOUTPUTに設定
    digitalWrite(SS, HIGH); //デジタルの10番ピンからHighを出力
    SPI.setBitOrder(MSBFIRST); // 最上位ビットから送信
    SPI.setClockDivider(SPI_CLOCK_DIV8); //分周器1/8を指定 同期クロック16/8=2MHz
    SPI.setDataMode(SPI_MODE2); //アイドル時HIGH、立上がりエッジでデータ取得
    SPI.begin(); //SPI通信の初期化、有効化
}

bool Flg = true;
void loop() {
    // １回のみ実行
    if (Flg) {
        Flg = false;
        for (auto send_data:test_data_set) {
            digitalWrite(SS, LOW);
            for (auto i = 0; i < BYTE_SIZE; i++) {
                uint8_t recv_data = SPI.transfer(send_data[i]);
                Serial.println(recv_data, BIN);
            }
            digitalWrite(SS, HIGH);
            delay(10);
        }
    }
}