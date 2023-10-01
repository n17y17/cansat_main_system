#include "sc00_cpp_example.hpp"

void setup()
{
    try
    {
        stdio_init_all();  // pico-sdkの初期化

        bme.init(&i2c, 0x52);  // BMEを初期化

        bme.measure();  // BMEで1回測定
        sc::Altitude0(bme.pressure(), bme.temperature(), 0.0);  // 標高を計算するための基準点の気圧，気温を設定
    }
    catch(const std::exception& e)
    {
        // エラー時の処理
        sc::Error(__FILE__, __LINE__, "An error occurred during setup.", e.what());  // セットアップ時にエラーが発生しました
    }
}

void loop()
{
    try
    {
        // センサで計測
        measure();

        // 測定結果を記録
        sc::save_log("temperature:%f`C, pressure:%fhPa, humidity:%f%, altitude:%fm\n", temperature, pressure, humidity, altitude);

        // 1s待機
        sleep_ms(1000);
    }
    catch(const std::exception& e)
    {
        // エラー時の処理
        sc::Error(__FILE__, __LINE__, "An error occurred in the loop.", e.what());  // ループ内でエラーが発生しました
    }
}

int main()
{
    setup();

    while(true) loop();
}