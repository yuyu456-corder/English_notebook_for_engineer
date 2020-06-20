#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "header.h"

//出題する問題数
#define NUMBER_OF_QUESTION 10
//出題する解答の選択肢の数
#define NUMBER_OF_CHOICES 4

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
	int random_number[NUMBER_OF_CHOICES + 1] = { 0 };

	//プログラム実行毎に異なる順序の乱数が出力されるようにする
	unsigned int current_time = (unsigned int)time(0);
	srand(current_time);

	int current_question_index = 1;
	while (current_question_index <= NUMBER_OF_QUESTION) {

		//問題出力用に乱数を設定する(下限:1,上限:get_max_words)
		for (int i = 0; i < NUMBER_OF_CHOICES + 1; i++) {
			random_number[i] = 1 + (int)(rand() * (get_max_words - 1 + 1.0) / (1.0 + RAND_MAX));
		}
		//選択肢に解答を含める場所を決める(下限:1,上限:NUMBER_OF_CHOICES)
		int answer_number = 1 + (int)(rand() * (NUMBER_OF_CHOICES - 1 + 1.0) / (1.0 + RAND_MAX));

		//英単語を4択で出力
		printf("current question number: %d, Question: %s \n", current_question_index, get_json_key_pointer[random_number[0]]);
		for (int i = 1; i <= NUMBER_OF_CHOICES; i++) {
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
			char answer_flag = '\0';
			answer_flag = getch();
			//Escキー押下でプログラム終了
			if (answer_flag == 0x1b) {
				printf("english notebook mode end \n");
				printf("please Enter key to return main mode \n");
				return 0;
			}
			//正解のキーを検知する
			else if (answer_flag == answer_number + '0') {
				printf("Correct!! \n");
				break;
			}
			//不正解のキーを検知する
			else if (answer_flag >= 0x31 && answer_flag <= 0x34) {
				printf("Incorrect... \n");
				printf("The answer: %s \n", get_json_value_pointer[random_number[0]]);
				break;
			}
			//使用しないキーは何もしない
			else {
				continue;
			}
		}

		++current_question_index;
		printf("==================================================\n");
	}

	printf("question mode is finished! \n");
	printf("please press any key to return main mode \n");

	return 0;
}