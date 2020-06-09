#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "header.h"

//出題する問題数
#define number_of_question 10
//出題する解答の選択肢の数
#define number_of_choices 4

//グローバル変数の宣言
char* get_json_key_pointer[MAX_RECORDS];
char* get_json_value_pointer[MAX_RECORDS];
int get_max_words;

/*
*@brief 英単語を4択形式に出題し、解答を受け付ける関数
*@param void
*@return: 0(処理正常終了)
*/
int question(void) {

	printf("==question mode== \n");
	printf("please press number key correspond to an answer > \n");

	//問題出力用の乱数を確保(選択肢＋問題)
	int random_number[number_of_choices + 1] = { 0 };

	//プログラム実行毎に異なる順序の乱数が出力されるようにする
	unsigned int current_time = (unsigned int)time(0);
	srand(current_time);

	int current_question_index = 1;
	while (current_question_index <= number_of_question) {

		//問題出力用に乱数を設定する
		for (int i = 0; i < number_of_choices + 1; i++) {
			random_number[i] = (int)(rand() * (get_max_words - 1 + 1.0) / (1.0 + RAND_MAX));
			//printf("get random number: %d \n", random_number[i]);
		}
		//選択肢に解答を含める場所を決める
		int answer_number = (int)(rand() * (number_of_choices - 1 + 1.0) / (1.0 + RAND_MAX));

		//英単語を4択で出力
		printf("current question number: %d, Question: %s \n", current_question_index, get_json_key_pointer[random_number[0]]);
		for (int i = 1; i <= number_of_choices; i++) {
			//解答の選択肢を出力
			if (i == answer_number) {
				printf("%d: %s / ", i, get_json_value_pointer[random_number[0]]);
				continue;
			}
			printf("%d: %s / ", i, get_json_value_pointer[random_number[i]]);
		}
		printf("\n");
		//キーボード入力を検知して解答を出力する
		while (1) {
			//Escキー押下でプログラム終了
			if (getch() == 0x1b) {
				printf("english_notebook mode end \n");
				printf("please Enter key to return main mode \n");
				return 0;
			}
			else if (getch() != 0x1b) {
				printf("The answer: %s \n", get_json_value_pointer[random_number[0]]);
				break;
			}
		}

		++current_question_index;
		printf("==================================================\n");
	}

	printf("question mode is finished! \n");

	return 0;
}