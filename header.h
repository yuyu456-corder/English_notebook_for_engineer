#ifndef HEADER_H
#define HEADER_H

//マクロ定義
//読み込むファイルの最大行数
#define MAX_RECORDS 1000

//型定義
/**
* JSONファイルをパースした結果を格納する
*/
typedef struct {
	char* get_json_key_pointer[MAX_RECORDS];	//!< JSONの各英単語（Key）へのポインタ
	char* get_json_value_pointer[MAX_RECORDS];	//!< JSONの各解答（Value）へのポインタ
} parse_json_string_t;

/**
 * 各単語の属性（不正解回数、難易度等）をまとめた疑似DB
 */
typedef struct {
	int word_id; //!<DBとして扱うためのID(UNIQUE KEYかつFOREIGN KEY)
	int incorrect_count; //!< 不正解回数
} word_attributes_t;

//グローバル変数
//収録単語数
extern int get_max_words;

//グローバル関数のプロトタイプ宣言
int parse_json(parse_json_string_t*);
int english_notebook(parse_json_string_t*);
int question(parse_json_string_t*, word_attributes_t*);

#endif