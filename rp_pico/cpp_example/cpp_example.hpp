#ifndef SC_PROJECT_RP_PICO_PICO_SDK_SC00_EXAMPLE_HPP_
#define SC_PROJECT_RP_PICO_PICO_SDK_SC00_EXAMPLE_HPP_

#include <string>

#include "sc.hpp"
#include "bme280.hpp"

sc::I2C i2c(0, sc::Pin(4), sc::Pin(5), 9600);  // I2Cのオブジェクト
sc::BME280 bme;  // BME280のオブジェクト

sc::Temperature temperature;  // 測定した気温
sc::Pressure pressure;  // 測定した気圧
sc::Humidity humidity;  // 測定した湿度
sc::Altitude altitude;  // 計算した標高

// センサの値を測定
void measure()
{
    // BME280で測定
    bme.measure();

    // 測定値を保存
    temperature = bme.temperature();
    pressure = bme.pressure();
    humidity = bme.humidity();

    // 標高を計算
    altitude = sc::to_altitude(pressure, temperature);
}

// ログを記録する関数を定義します
// SDカードなどを使う場合は，この関数に追記することでログがSDカードにも保存されるようになります
void sc::save_log(const std::string &log) noexcept
{
    printf(log.c_str());
}

#endif  // SC_PROJECT_RP_PICO_PICO_SDK_SC00_EXAMPLE_HPP_