#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include "header.h"

/*グローバル変数の定義*/
int get_max_words = 0;

int main(void)
{
	//JSONパース結果の構造体の宣言と初期化
	static parse_json_string_t parse_json_string = { 0,0 };
	parse_json_string_t* parse_json_string_p = &parse_json_string;

	printf("==English note book for engineer== \n");

	//英単語をまとめたJSONをパースする
	parse_json(parse_json_string_p);

	//get_max_wordsの値が確定したので宣言と初期化を行う
	//DBのUNIQUE KEYとして使いたいため配列の添字に+1をしている（インデックスを1から始めるようにしている）
	word_attributes_t* s_word_attributes;
	size_t element_size_of_word_attributes = sizeof(word_attributes_t);
	s_word_attributes = malloc(element_size_of_word_attributes * get_max_words);
	memset(s_word_attributes, 0, sizeof(element_size_of_word_attributes * get_max_words));
	if (s_word_attributes == NULL) {
		printf("Error: Memory allocation failed");
		exit(0);
	}

	printf("Press 0 -> question mode, press 1 -> English notes mode \n");
	printf("If you press ESC key, this system is exited \n");
	printf("Please press number key to migrate mode > \n");

	while (1) {

		//入力モードを受け付ける
		char mode_flag = '\0';
		mode_flag = getch();

		//入力された文字をチェックする
		switch (mode_flag) {
		case '0':
			//問題解答モードへ移行する
			question(parse_json_string_p, s_word_attributes);
			break;
		case '1':
			//単語帳モードへ移行する
			english_notebook(parse_json_string_p);
			break;
		case 0x1b:
			//システムの正常終了
			free(s_word_attributes);
			exit(0);
			//無効な値の場合
		default:
			printf("Please press number key to migrate mode > \n");
		}
	}

	return 0;
}