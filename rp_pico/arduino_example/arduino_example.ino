#include "arduino_example.hpp"

// Arduinoではエラー処理を行えないため，エラーが発生しないように慎重に設計してください

void setup()
{
    bme.init(&i2c, 0x76);  // BMEを初期化

    bme.measure();  // BMEで1回測定
    sc::Altitude0(bme.pressure(), bme.temperature(), 0.0);  // 標高を計算するための基準点の気圧，気温を設定
}

void loop()
{
    // センサで計測
    measure();

    // 測定結果を記録
    sc::save_log("temperature:%f`C, pressure:%fhPa, humidity:%f%, altitude:%fm\n", temperature, pressure, humidity, altitude);

    // 1s待機
    sleep_ms(1000);
}