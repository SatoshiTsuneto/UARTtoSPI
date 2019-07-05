#include <SPI.h>
#include <stdint.h>
#include <Arduino.h>

void write_spi(struct MotorCmd cmd);

// PC側と取り決めた値を定義
enum UART_CMD {
    CIRCLE_CLOCKWISE = 0x10,
    CIRCLE_COUNTERCLOCKWISE = 0x11,
    SWIPE_TO_RIGHT = 0x12,
    SWIPE_TO_LEFT = 0x13
};

// モーターのコマンドを保持する構造体
// 配列は定数かつ値渡しで関数に渡したいため、ラップする構造体を用意
struct MotorCmd {
    uint64_t data_set[2];
};

// TODO : 以下配列の中身はダミーデータなので正しく対応するコマンド(bit列)を組み立てる
const struct MotorCmd motor_up{
        0b111111111011000000000111111000111111011111111000,
        0b111111111011000000000111111000100000000000001000
};

const struct MotorCmd motor_down{
        0b111111111011000000000111111000111111011111111000,
        0b111111111011000000000111111000100000000000001000
};

const struct MotorCmd motor_clockwise{
        0b111111111011000000000111111000111111011111111000,
        0b111111111011000000000111111000100000000000001000
};

const struct MotorCmd motor_counterclockwise{
        0b111111111011000000000111111000111111011111111000,
        0b111111111011000000000111111000100000000000001000
};

// 初期化要コマンドは関数に渡したり刷ることが無いのでそのままconstで定義
// 検討: 上の構造体で統一すべき？(現状は配列のサイズが違うため分けたまま)
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

void setup() {
    //UART通信のセットアップ
    Serial.begin(9600);

    // SPI通信のセットアップ
    pinMode(SS, OUTPUT); //デジタル10番ピンをOUTPUTに設定
    digitalWrite(SS, HIGH); //デジタルの10番ピンからHighを出力
    SPI.setBitOrder(MSBFIRST); // 下位ビットから送信
    SPI.setClockDivider(SPI_CLOCK_DIV8); //分周器1/8を指定 同期クロック16/8=2MHz
    SPI.setDataMode(SPI_MODE3); //アイドル時Low、立上がりエッジでデータ取得
    SPI.begin(); //SPI通信の初期化、有効化
}

void write_spi(struct MotorCmd cmd) {

    // 制御するデバイスに通信の開始を通知する
    digitalWrite(SS, LOW);
    for (auto data:InitData) {
        SPI.transfer(&data, 6);
        delay(10); //必要に応じて設定
    }

    for (auto data:cmd.data_set) {
        SPI.transfer(&data, 6);
        delay(10); //必要に応じて設定
    }

    // 制御するデバイスに通信の終了を通知する
    digitalWrite(SS, HIGH);
}


void loop() {

    switch (Serial.read()) {

        case CIRCLE_CLOCKWISE:
            write_spi(motor_clockwise);
            break;

        case CIRCLE_COUNTERCLOCKWISE:
            write_spi(motor_counterclockwise);
            break;

        case SWIPE_TO_LEFT:
            write_spi(motor_down);
            break;

        case SWIPE_TO_RIGHT:
            write_spi(motor_up);
            break;

        case -1:
            // TODO : 接続のエラーの場合-1なので適切な例外処理を記述する
            break;

        default:
            // TODO: 取り決めた上で定義したenum以外の値が飛んできた場合の例外処理
            break;
    }
}




