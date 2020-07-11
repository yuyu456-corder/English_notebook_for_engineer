﻿#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include "header.h"

//読み込むファイルの一行の最大文字数（データ数）
#define STR_MAX_ROW 100
//出題する問題数
#define NUMBER_OF_QUESTION 10
//出題する解答の選択肢の数
#define NUMBER_OF_CHOICES 4
//CSVファイルから読み込む最大の要素数
#define NUMBER_OF_CSV_ELEMENTS 100

//グローバル変数の宣言
int get_max_words;

/**
* 1回毎の解答モードの結果
*/
typedef struct {
	int playingLogId;								//!< 結果のログを一意に判別するためのID
	//int playTime;									//!< 解答モードをプレイした時間
	int incorrectWordIndex[NUMBER_OF_QUESTION];		//!< 不正解だった単語のインデックス
	double correctAnswerRate;						//!< 正答率
} result_log_t;

/**
*@brief 英単語を4択形式に出題し、解答を受け付ける関数
*@param void
*@return: 0(処理正常終了)
*/
int question(parse_json_string_t* parse_json_string_p) {

	//問題出力用の乱数を確保(選択肢＋問題)
	int random_number[NUMBER_OF_CHOICES + 1] = { 0 };
	//正解数のカウントを行う
	int correct_answer_number = 0;
	//正答率(正解数÷問題数)
	double correct_answer_rate = 0;
	//間違った解答のカウンタ（インデックス）
	int incorrect_word_of_number = 0;
	//間違った問題のインデックス
	int incorrect_words_index = 0;
	//モードをプレイした時に間違えた単語を保存する
	int incorrect_word_index[NUMBER_OF_QUESTION] = { 0 };
	//解答モードの結果を記録する構造体の宣言
	result_log_t result_log = { 0,{0},0 };

	//読み込む・書きこむファイルの設定
	FILE* fp_log_write;
	char* file_name = "play_data_of_question_mode.csv";
	fp_log_write = fopen(file_name, "a+");
	//ファイルのオープンに失敗した場合
	if (fp_log_write == NULL) {
		printf("error: can't open log file... \n");
		return -1;
	}

	printf("==Question mode== \n");
	printf("Please press number key correspond to an answer > \n");

	/*
	*ゲーム成績をCSVファイルから読み込む
	*/
	//CSVファイルからゲーム成績を取得する
	//CSVファイルを文字列リテラルとして一度パースする
	char tmp_get_csv_elements[NUMBER_OF_CSV_ELEMENTS][STR_MAX_ROW] = { '\0' };
	char* tmp_incorrect_word = '\0';
	//CSVの各要素が代入に成功したかの判定を行うフラグ値
	int res_fscanf = 0;
	printf(">get your playing data \n");
	while ((res_fscanf = fscanf(fp_log_write, "%[^,],%[^,],%s", tmp_get_csv_elements[0], tmp_get_csv_elements[1], tmp_get_csv_elements[2])) != EOF) {
		//printf("parse_csv: %s %s %s \n", tmp_get_csv_elements[0], tmp_get_csv_elements[1], tmp_get_csv_elements[2]);
		//各要素を対応する構造体に代入する、同時に成績データを元に平均正答率などの統計データを求める
		result_log.playingLogId = atoi(tmp_get_csv_elements[0]);
		//データのIDが0の場合データとして含まない
		if (result_log.playingLogId == 0) {
			continue;
		}
		printf("playing id: %d \n", result_log.playingLogId);
		result_log.correctAnswerRate = atoi(tmp_get_csv_elements[1]);
		printf("answer rate: %3.2f \n", result_log.correctAnswerRate);
		tmp_incorrect_word = tmp_get_csv_elements[2];
		printf("incorrect word index: %s \n", tmp_incorrect_word);
		//不正解の単語はまだCSV形式のままなので、再帰的に同じ方法でもう一度パースを行う処理を書く
		printf("==================================================\n");
	}
	/*
	*問題出力とキー入力による解答を行う
	*/
	//プログラム実行毎に異なる順序の乱数が出力されるようにする
	unsigned int current_time = (unsigned int)time(0);
	srand(current_time);

	int current_question_index = 0;
	while (current_question_index < NUMBER_OF_QUESTION) {

		//問題出力用に乱数を設定する(下限:1,上限:get_max_words)
		for (int i = 0; i < NUMBER_OF_CHOICES + 1; i++) {
			random_number[i] = 1 + (int)(rand() * (get_max_words - 1 + 1.0) / (1.0 + RAND_MAX));
		}
		//選択肢に解答を含める場所を決める(下限:1,上限:NUMBER_OF_CHOICES)
		int answer_number = 1 + (int)(rand() * (NUMBER_OF_CHOICES - 1 + 1.0) / (1.0 + RAND_MAX));

		//英単語を4択で出力
		printf("Current question number: %d, Question: %s \n", current_question_index, parse_json_string_p->get_json_key_pointer[random_number[0]]);
		for (int i = 1; i <= NUMBER_OF_CHOICES; i++) {
			//解答の選択肢を出力
			if (i == answer_number) {
				printf("%d: %s / ", i, parse_json_string_p->get_json_value_pointer[random_number[0]]);
				continue;
			}
			printf("%d: %s / ", i, parse_json_string_p->get_json_value_pointer[random_number[i]]);
		}
		printf("\n");
		//キーボード入力を検知して解答を出力する
		while (1) {
			char answer_flag = '\0';
			answer_flag = getch();
			//Escキー押下でプログラム終了
			if (answer_flag == 0x1b) {
				printf("English notebook mode end \n");
				printf("Please Enter key to return main mode \n");
				return 0;
			}
			//正解のキーを検知する
			else if (answer_flag == answer_number + '0') {
				printf("Correct!! \n");
				++correct_answer_number;
				break;
			}
			//不正解のキーを検知する
			else if (answer_flag >= 0x31 && answer_flag <= 0x34) {
				printf("Incorrect... \n");
				printf("The answer: %s \n", parse_json_string_p->get_json_value_pointer[random_number[0]]);
				//不正解になった問題のインデックスを記録する
				incorrect_word_index[incorrect_word_of_number] = random_number[0];
				++incorrect_word_of_number;
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

	/*
	*ゲーム成績を保存する
	*/
	//正答率を計算する
	correct_answer_rate = (double)correct_answer_number / (double)current_question_index;

	printf("Question mode is finished! \n");
	printf("your current playing data \n");

	//今回の成績の表示
	printf("correct answer rate is %.0f Percent \n", correct_answer_rate * 100);
	printf("play count is %d \n", result_log.playingLogId);
	printf("incorrect words is shown below \n");

	/**
	*今回の成績をCSVファイルに書き出す
	*CSVファイルの形式：{"id(int), clearRate(int), inccorentWordIndex(array)"}
	*/
	fprintf(fp_log_write, "%d,", ++result_log.playingLogId);
	fprintf(fp_log_write, "%.0f,", correct_answer_rate * 100);
	fprintf(fp_log_write, "{");
	//不正解の単語が一つでもあった場合
	if (incorrect_word_index[0] != 0) {
		fprintf(fp_log_write, ",");
		while (incorrect_word_index[incorrect_words_index]) {
			fprintf(fp_log_write, "%d", incorrect_word_index[incorrect_words_index]);
			//間違った解答のArrayの要素が最終かどうかを検知する
			if (incorrect_word_index[incorrect_words_index + 1] != 0) {
				fprintf(fp_log_write, ",");
			}
			else {
				fprintf(fp_log_write, ",}\n");
			}
			//不正解の単語を表示する
			printf(">%s ", parse_json_string_p->get_json_key_pointer[incorrect_word_index[incorrect_words_index]]);
			++incorrect_words_index;
			printf("\n");
		}
	}
	//不正解の単語が無かった（＝全問正解）場合
	else {
		fprintf(fp_log_write, ",}\n");
		printf("Perfect Clear!! \n");
	}

	printf("Please press any key to return main mode \n");

	fclose(fp_log_write);

	return 0;
}