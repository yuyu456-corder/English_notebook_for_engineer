#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "header.h"

//グローバル変数の宣言
int get_max_words;

/*
*@brief 英単語＞日本語を交互に出力する関数
*@param void
*@return: 0(処理正常終了)
*@detail: 英単語の出力、解答の出力をキー入力で交互に行う
*/
int english_notebook(parse_json_string_t* parse_json_string_p)
{
	printf("==English notebook mode== \n");
	printf("When you press something key, this question of answer is outputted \n");
	printf("If you press ESC key, this mode is exited \n");

	printf("get max words: %d \n", get_max_words);

	//プログラム実行毎に異なる順序の乱数が出力されるようにする
	unsigned int current_time = (unsigned int)time(0);
	srand(current_time);

	while (1) {
		//問題出力用に乱数を設定する
		int random_num = (int)(rand() * (get_max_words - 1 + 1.0) / (1.0 + RAND_MAX));
		//英単語出力
		printf("Index: %d, Question: %s \n", random_num, parse_json_string_p->get_json_key_pointer[random_num]);

		//キーボード入力を検知して解答を出力する
		while (1) {
			//Escキー押下でプログラム終了
			if (getch() == 0x1b) {
				printf("English　notebook mode end \n");
				printf("Please Enter key to return main mode \n");
				return 0;
			}
			else if (getch() != 0x1b) {
				printf("The answer: %s \n", parse_json_string_p->get_json_value_pointer[random_num]);
				break;
			}
		}
		printf("==================================================\n");
	}

	return 0;
}