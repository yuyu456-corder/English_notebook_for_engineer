#ifndef HEADER_H
#define HEADER_H

//グローバル定数
//読み込むファイルの最大行数
#define MAX_RECORDS 1000

//グローバル変数
//JSONからパースしたkeyとvalueを保持する
extern char* get_json_key_pointer[MAX_RECORDS];
extern char* get_json_value_pointer[MAX_RECORDS];
//読み込んだ単語数
extern int get_max_words;
//プレイ回数
extern int number_of_playing_question_mode;

//グローバル関数のプロトタイプ宣言
int parse_json(void);
int english_notebook(void);
int question(void);

#endif