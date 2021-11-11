#pragma once

struct TextureData {
    enum class Keyword {
        TITLE, //画像のタイトル
        AUTHOR, //作者の名前
        DESCRIPTION, //画像の説明
        COPYRIGHT, //著作権の通知
        CREATION_TIME, //画像の作成日時
        SOFTWARE, //作成に使用したソフト
        DISCLAIMER, //公的な使用の是非について
        WARNING, //注意事項
        SOURCE, //画像の作成に用いたもの
        COMMENT //雑多なコメント
    };

    //チャンクタイプ
    static constexpr char CHUNK_TYPE[4] = { 't', 'E', 'X', 't' };

    //TextureDataか判断する
    static bool isType(const unsigned char* type) {
        return (type[0] == CHUNK_TYPE[0]
            && type[1] == CHUNK_TYPE[1]
            && type[2] == CHUNK_TYPE[2]
            && type[3] == CHUNK_TYPE[3]
        );
    }
};
