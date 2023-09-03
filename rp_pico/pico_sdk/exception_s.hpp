#ifndef CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_EXCEPTION_S_HPP_
#define CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_EXCEPTION_S_HPP_

#include <unordered_map>
#include <iostream>  // cerr, endl
#include <string>  // string()
#include <chrono>  // 処理を再度行うための時間計測

// ゼロ徐算防止
template<class T> inline T not0(T n) {return (a ? a : (T)1);}

/*
エラーを出力します
記録をするだけなので，動作を停止することはありません．
file 部分には ＿FILE＿ と書いてください
line 部分には ＿LINE＿ と書いてください
(コンパイル時に自動でファイル名と行番号に置き換わります)
*/
void RecordError(const char* file, unsigned line, const std::exception& e = std::runtime_error("Unknown error occurred"))
{
    try
    {
        std::string message = "!!ERROR  FILE : " + std::string(file) + "  LINE : " + std::to_string(line) + "\n!!       MESSAGE : " + e.what();  // 出力するメッセージをstringがたの変数に入れる
        std::cerr << message << std::endl;  // cerrでエラーとして出力

        // ここで，エラーをログデータに記録（未実装）
    }
    catch(const std::exception& ee)
    {
        std::cerr << "!!ERROR  FILE : " << __FILE__ << "  LINE : " << __LINE__ << "/n!!       MESSAGE : " << ee.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "!!ERROR  FILE : " << __FILE__ << "  LINE : " << __LINE__ << "/n!!       MESSAGE : Unknown error occurred" << std::endl;
    }
}

/*
処理が成功したか
kSuccess : 成功しました
kFailure : 失敗しました
kError : 実行時エラーが発生しました
*/
enum ProcessResult
{
    kSuccess,  // 成功しました
    kFailure,  // 失敗しました
    kError  // 実行時エラーが発生しました
};

/*
動作状況
正常に稼働中 : kWorking
動かない時も多い : kLittle
ほとんど動かない : kStopped
*/
enum class State
{
    kWorking,  // 正常に稼働中
    kLittle,  // 動かない時も多い
    kStopped  // ほとんど動かない
};

static const uint16_t kMaxStatus = 65535;  // 処理の成功状況を記録する際の最大値
static const uint16_t kDefaultStatus = 60000;  // 処理の成功状況を記録する際の初期値
static const uint16_t kMinStatus = 0;  // 処理の成功状況を記録する際の最小値
static std::unordered_map<const std::string, std::pair<bool, uint16_t>> kStatus;  // 処理の成功状況を記録する変数  処理に成功すると増加し，失敗すると減少する
static auto kStatusIncrementTimerStart = std::chrono::steady_clock::now();  // 一定時間経過ごとに処理の成功状況の値を増加させるためのタイマーです
static const float kStatusIncrementTimeDiff = 30;  // 何秒に一回，成功状況の値を増加さえるのか

// 一定時間の経過を確認したとき，処理の成功状況の値を増加させます
inline static void IncrementStatus()
{
    std::chrono::duration<float> diff = std::chrono::steady_clock::now() - kStatusIncrementTimerStart;
    if (diff.count() > kStatusIncrementTimeDiff)
    {
        for (std::pair<const std::string, std::pair<bool, uint16_t>>& status : kStatus)
        {
            if (status.second.first) continue;
            ++status.second.second;
        }
    }
}

// 処理の結果を保存します．process_nameは，{ } でくくることで複数個まとめて指定できます
struct Result
{
    Result(ProcessResult result, const std::string process_name)
    {
        if (kStatus.count(process_name) == 0) kStatus[process_name] = std::make_pair(false, kDefaultStatus);  // 新たに動作状況の変数を作る場合は，デフォルトの値で初期化
        if (kStatus[process_name].first) return;  // 動作状況の変更にロックがかかっていた場合，変更しない
        uint16_t& this_status = kStatus[process_name].second;
        switch (result)
        {
            case kSuccess:
            {
                if (this_status <= (kMaxStatus - 1)) this_status += 1;  // 関数の処理に成功したため加算
                break;
            }
            case kFailure:
            {
                if (this_status >= (kMinStatus + 10)) this_status -= 30;  // 関数の処理に失敗したため減算
                break;
            }
            case kError:
            {
                if (this_status >= (kMinStatus + 50)) this_status -= 100;  // 関数の処理中にエラーが発生したため大きく減算
                break;
            }
            default:
            {
                RecordError(__FILE__, __LINE__, std::domain_error("A value not defined in ProcessResult was given."));
            }
        }
        if (this_status < kMinStatus) this_status = kMinStatus;
        if (this_status > kMaxStatus) this_status = kMaxStatus;
    }
    Result(ProcessResult result, std::initializer_list<const std::string>& process_name_s)
    {
        for (const std::string& process_neme : process_name_s)
        {
            Result(result, process_neme);
        }
    }
};

static const uint8_t kMinWorkingSuccessRate = 70;  // 何％以上動作しているときに，「動作中」の判定にするか
static const uint8_t kMinLittleSuccessRate = 30;  // 何％以上動作しているときに，「少しだけ動作中」の判定にするか
// ある処理が，今まで正常に実行されてきたかを返します
inline State GetStatus(const std::string process_name)
{
    if (kStatus.count(process_name) == 0) kStatus[process_name] = std::make_pair(false, kDefaultStatus);  // 新たに動作状況の変数を作る場合は，デフォルトの値で初期化
    uint8_t success_rate = (kStatus[process_name].second - kMinStatus) / not0(kMaxStatus - kMinStatus);  // 処理の成功割合 (%)
    if (success_rate > kMinWorkingSuccessRate) return State::kWorking;
    else if (success_rate > kMinLittleSuccessRate) return State::kLittle;
    else return State::kStopped;
}

/*
ある処理が正常に実行されているかを表す値を一定値に固定します
固定しない場合，処理の実行や時間の経過により値が変化します
*/
inline void LockStatus(const std::string process_name, State run_state)
{
    switch(run_state)
    {
        case State::kWorking : kStatus[process_name] = std::make_pair(true, kMaxStatus); break;  // 状態を最高値に固定
        case State::kLittle : kStatus[process_name] = std::make_pair(true, (kMaxStatus) / 100 * ((kMinWorkingSuccessRate + kMinLittleSuccessRate) / 2)); break;  // 状態をLittleの真ん中くらいに固定
        case State::kStopped : kStatus[process_name] = std::make_pair(true, kMinStatus); break;  // 状態を最低値に固定
        default : RecordError(__FILE__, __LINE__, std::domain_error("A value not defined in State was given."));
    }
}

#endif  // CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_EXCEPTION_S_HPP_