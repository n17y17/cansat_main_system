# 最低限必要なCMakeのバージョンを設定
cmake_minimum_required(VERSION 3.12)

# SDKの読み込み (プロジェクトに関する設定の前にある必要がある)
include(pico_sdk_import.cmake)

# プロジェクト名，プログラミング言語を設定
project(SC00 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# 例外を有効にする
set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)
# 以下の資料を参考にしました
# https://community.element14.com/products/raspberry-pi/b/blog/posts/raspberry-pico-and-cmake---enable-c-exceptions-and-rtti-run-time-type-information

# 警告レベルを上げる
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif()

# SDKを初期化
pico_sdk_init()

# # フォルダを追加する
# add_subdirectory(sc)

# ビルドを実行するファイルを追加
add_executable(SC00 sc00_cpp_example.cpp sc.cpp bme280.cpp)

# pico_stdlib（ライブラリ）の読み込み
target_link_libraries(SC00 pico_stdlib hardware_gpio hardware_i2c hardware_spi hardware_uart hardware_pwm)

# USB出力を有効にし，UART出力を無効にする
pico_enable_stdio_usb(SC00 1)
pico_enable_stdio_uart(SC00 0)

# map/bin/hex/uf2などのファイルを追加で出力する
pico_add_extra_outputs(SC00)