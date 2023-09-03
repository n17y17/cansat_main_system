#ifndef CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_SENSOR_S_HPP_
#define CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_SENSOR_S_HPP_

#include <stdio.h>

#include "communication_method_s.hpp"

const double kDefaultErrorSensorValue = -1024.0;  // 測定しなかった，できなかった時のセンサの値

/*
センサのデータを記録するクラス(実質的に変数)
代入時などの処理に変更を加える可能性を考え，クラスにした
このオブジェクトを参照すること，ポインタ経由で値を代入することは禁止
*/
class FloatSensorValue
{
    float f_;
public:
    FloatSensorValue() : f_(kDefaultErrorSensorValue) {}
    FloatSensorValue(float f) : f_(f) {}
    void operator= (float f) {f_ = f;}
    operator float() const {return f_;}
};

/*
センサのデータを記録するクラス(実質的に変数)
代入時などの処理に変更を加える可能性を考え，クラスにした
このオブジェクトを参照すること，ポインタ経由で値を代入することは禁止
*/
class DoubleSensorValue
{
    double d_;
public:
    DoubleSensorValue() : d_(kDefaultErrorSensorValue) {}
    DoubleSensorValue(double d) : d_(d) {}
    void operator= (double d) {d_ = d;}
    operator double() const {return d_;}
};

/*
x, y, z の値と絶対値absをまとめたもの
初期のセンサ値は，FloatSensorValueのデフォルトコンストラクタが呼ばれて，kDefaultErrorSensorValue(=-1024)になる
オブジェクト名.x のように . をつけてアクセスする
*/
struct XYZAbs
{
    FloatSensorValue x;
    FloatSensorValue y;
    FloatSensorValue z;
    FloatSensorValue abs;
};

/*
センサの値をまとめたもの
初期のセンサ値は，FloatSensorValueのデフォルトコンストラクタが呼ばれて，kDefaultErrorSensorValue(=-1024)になる
オブジェクト名.temperatureのように . をつけてアクセスする
*/
struct Measurement
{
    FloatSensorValue temperature;  // 気温 (℃)
    FloatSensorValue pressure;  // 気圧 (hPa)
    FloatSensorValue humidity;  // 湿度 (%)
    FloatSensorValue altitude;  // 高度 (m)
    DoubleSensorValue latitude;  // 緯度 (dd.dd...°)
    DoubleSensorValue longitude;  // 経度 (dd.dd...°)
    FloatSensorValue distance;  // 距離 (距離センサ) (m)
    FloatSensorValue azimuth;  // 方位角 (N:0°->E:90°->S:180°->W:270°->N:360°)
    FloatSensorValue elevation_angle;  // 仰角 (下:-90°->水平:0°->上:90°)
    FloatSensorValue area;  // 面積 (カメラを想定)
    FloatSensorValue goal_distance;  // ゴールまでの距離 (m)
    FloatSensorValue goal_direction;  // ゴールの方向 (後:-180°->左:-90°->前:0°->右:90°->後:180°)
    XYZAbs acceleration;  // 加速度 (x, y, z, abs)
    XYZAbs gyro;  // 角速度 (x, y, z, abs)
    XYZAbs magnetism;  // 磁気 (x, y, z, abs)
};

/*
これは抽象クラスです
センサに関するクラスの継承元クラスとして利用します
ここで "= 0" と定義されている関数を継承先のクラスでオーバーライド(再定義して上書き)しないとコンパイルエラーになります
*/
class Sensor
{
    virtual bool AllowExecution() {
        
    }
};

#endif  // CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_SENSOR_S_HPP_