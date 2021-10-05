#pragma once

enum class FbxTimeMode {
    DEFAULT_MODE,
    FRAMES_120,
    FRAMES_100,
    FRAMES_60,
    FRAMES_50,
    FRAMES_48,
    FRAMES_30,
    FRAMES_30_DROP,
    NTSC_DROP_FRAME,
    NTSC_FULL_FRAME,
    PAL,
    FRAMES_24,
    FRAMES_1000,
    FILM_FULL_FRAME,
    CUSTOM,
    FRAMES_96,
    FRAMES_72,
    FRAMES_59_DOT_94,
    MODES_COUNT
};

namespace FbxTimeModeTime {
constexpr long long FRAMES_24_TIME = 1924423250ll;
constexpr long long FRAMES_30_TIME = 1539538600ll;
constexpr long long FRAMES_60_TIME = 769769300ll;
constexpr long long FRAMES_120_TIME = 384884650ll;
}
