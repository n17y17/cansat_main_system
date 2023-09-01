#ifndef CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_EXCEPTION_HANDRING_S_HPP_
#define CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_EXCEPTION_HANDRING_S_HPP_

#include <queue>
#include <unordered_map>
#include <utility>

#include "sensor_s.hpp"

/*
処理の重要さ
重要でない処理でエラーが起きた時に，直ちにスキップするために使う
*/
enum ProcessImportance
{
    kOptional,  // 使わなくても大きな問題はありません
    kNormal,  // 行ったほうがよい処理です
    kEssential  // 絶対に必要です
};

// 処理が成功したか
enum ProcessResult
{
    kFailure,  // 失敗しました
    kSuccess  // 成功しました
};

// 動作状況
enum RunState
{
    kStoppedAll,  // 完全に動作を停止しています
    kWorkingLittle,  // 少しだけ動いています
    kWorkingNormally  // おおむね問題なく動作しています
};

// センサの値を正常に処理できたかを記録し，センサが正常に作動しているかを判断します
class SensorErrorManagement
{
    // センサが正常に動作したかどうかのログを過去何回まで残すか
    static const unsigned kMaxSensorFunctionLogSize = 100U;

    // センサの値を処理する関数の動作結果
    static std::unordered_map<Sensor*, std::unordered_map<unsigned, std::pair<ProcessImportance, std::queue<ProcessResult>>>> kSensorFunctionStatus;

    // 各センサが正常に動作しているかをセンサごとに保存
    static std::unordered_map<Sensor*, bool> kIsSensorNormally;

    // センサの値を処理する関数の動作結果を記録
    static void RecordFunctionResult(Sensor* sensor_p, unsigned function_id, ProcessImportance importance, ProcessResult is_success)
    {
        std::pair<ProcessImportance, std::queue<ProcessResult>> received_function = kSensorFunctionStatus[sensor_p][function_id];
        received_function.first = importance;
        received_function.second.push(is_success);
        if (received_function.second.size() > kMaxSensorFunctionLogSize) received_function.second.pop();
    }

    // センサ処理の関数を実行するかどうかを判断して返す
    static bool
};

#endif  // CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_EXCEPTION_HANDRING_S_HPP_