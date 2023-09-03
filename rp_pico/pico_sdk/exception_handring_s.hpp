#ifndef CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_EXCEPTION_HANDRING_S_HPP_
#define CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_EXCEPTION_HANDRING_S_HPP_

#include <queue>
#include <unordered_map>
#include <iostream>  // cerr, endl
#include <string>  // to_string, string(), c_str()

// ゼロ徐算防止
template<class T> inline T not0(T n) {return (a ? a : (T)1);}

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
enum RunState
{
    kWorking,  // 正常に稼働中
    kLittle,  // 動かない時も多い
    kStopped  // ほとんど動かない
};

/*
プログラムの実行状況やエラーを管理します
*/
class Exception
{
    static const uint16_t kMaxStatus = 65535;  // 処理の成功状況を記録する際の最大値
    static const uint16_t kDefaultStatus = 60000;  // 処理の成功状況を記録する際の初期値
    static const uint16_t kMinStatus = 0;  // 処理の成功状況を記録する際の最小値
    static std::unordered_map<Exception*, std::unordered_map<uint8_t, uint16_t>> kStatus;  // 処理の成功状況を記録する変数です
protected:
    /*
    処理の結果を記録します
    */
    void RecordResult(uint8_t process_id, ProcessResult result)
    {
        uint16_t& this_status = kStatus[this][process_id];
        switch (result)
        {
            case kSuccess:
            {
                if (this_status <= (kMaxStatus - 1)) this_status += 1;  // 関数の処理に成功したため加算
                break;
            }
            case kFailure:
            {
                if (this_status >= (kMinStatus + 10)) this_status -= 10;  // 関数の処理に失敗したため減算
                break;
            }
            case kError:
            {
                if (this_status >= (kMinStatus + 50)) this_status -= 50;  // 関数の処理中にエラーが発生したため大きく減算

                break;
            }
            default:
            {
                // エラー
            }
        }
        if (this_status < kMinStatus) this_status = kMinStatus;
        if (this_status > kMaxStatus) this_status = kMaxStatus;
    }
    /*
    処理の動作状況を返します
    戻り値は，オブジェクトのポインタと自動的に紐づけられています
    クラス内の処理の一意な通し番号を引数として渡してください
    */
    RunState Status(uint8_t process_id)
    {
        if (kStatus[this].count(process_id) == 0) kStatus[this][process_id] == kDefaultStatus;  // 新たに動作状況の変数を作る場合は，デフォルトの値で初期化
        uint8_t success_rate = (kStatus[this][process_id] - kMinStatus) / not0(kMaxStatus - kMinStatus);  // 処理の成功割合 (%)
        if (success_rate > 70) return kWorking;
        else if (success_rate > 30) return kLittle;
        else return kStopped;
    }
public:
    /*
    オブジェクト内で行われている処理の動作状況を返します
    戻り値は，オブジェクトのポインタと自動的に紐づけられています
    */
    RunState Status()
    {
        uint32_t average_status = 0;  // そのクラスに含まれる全動作状況の平均値
        for (std::pair<const uint8_t, uint16_t> id_and_status : kStatus[this])
        {
            average_status += id_and_status.second;
        }
        average_status /= kStatus[this].size();
        uint8_t success_rate = (average_status - kMinStatus) / not0(kMaxStatus - kMinStatus);  // 処理の成功割合 (%)
        if (success_rate > 70) return kWorking;
        else if (success_rate > 30) return kLittle;
        else return kStopped;
    }

    /*
    エラーを記録します
    file 部分には __FILE__ と書いてください
    line 部分には __LINE__ と書いてください
    (コンパイル時に自動でファイル名と行番号に置き換わります)
    */
    void RecordError(const char* file, unsigned line, uint8_t process_id, const std::exception& e = std::runtime_error("Unknown error occurred"))
    {
        try
        {
            std::string message = "!!ERROR  FILE : " + std::string(file) + "  LINE : " + std::to_string(line) + "  ID : " + std::to_string(process_id) + "\n!!       MESSAGE : " + e.what();
            std::cerr << message << std::endl;
            // RecordResult(process_id, kError);

            // エラーをログデータに記録
        }
        catch(const std::exception& e)
        {
            std::cerr << "!!ERROR  FILE : " << __FILE__ << "  LINE : " << __LINE__ << "/n!!       MESSAGE : " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "!!ERROR  FILE : " << __FILE__ << "  LINE : " << __LINE__ << "/n!!       MESSAGE : Unknown error occurred" << std::endl;
        }
    }
};
const uint16_t Exception::kMaxStatus, Exception::kDefaultStatus, Exception::kMinStatus;
std::unordered_map<Exception*, std::unordered_map<uint8_t, uint16_t>> Exception::kStatus;

#endif  // CANSAT_MAIN_SYSTEM_RP_PICO_PICO_SDK_EXCEPTION_HANDRING_S_HPP_