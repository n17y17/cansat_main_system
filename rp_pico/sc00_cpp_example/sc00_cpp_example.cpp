#include "sc00_cpp_example.hpp"

void setup()
{
    stdio_init_all();

    bme.init(&i2c, 0x52);  // BMEを初期化

    bme.measure();  // 1回測定
    sc::Altitude0(bme.pressure(), bme.temperature(), 0.0);  // 標高を計算するための基準点の気圧，気温を設定
}

int main()
{
    setup();
}