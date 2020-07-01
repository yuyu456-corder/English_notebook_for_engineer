#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include "header.h"

//出題する問題数
#define NUMBER_OF_QUESTION 10
//出題する解答の選択肢の数
#define NUMBER_OF_CHOICES 4
//読み込むファイルの一行の最大文字数（データ数）
#define STR_MAX_ROW 1000

//グローバル変数の宣言
char* get_json_key_pointer[MAX_RECORDS];
char* get_json_value_pointer[MAX_RECORDS];
int get_max_words;
int number_of_playing_question_mode;

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
int question(void) {

	printf("==Question mode== \n");
	printf("Please press number key correspond to an answer > \n");

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
	//解答モードの結果を記録する構造体の宣言
	result_log_t result_log = { 0,{0},0 };
	//CSVファイルから文字を読み込む配列
	char get_str[256];

	//読み込むファイルの設定
	FILE* fp_log_write;
	fp_log_write = fopen("play_data_of_question_mode.csv", "a");
	//ファイルのオープンに失敗した場合
	if (fp_log_write == NULL) {
		printf("error: can't open log file... \n");
		return -1;
	}

	//プログラム実行毎に異なる順序の乱数が出力されるようにする
	unsigned int current_time = (unsigned int)time(0);
	srand(current_time);

	int str_max_row = STR_MAX_ROW;
	//CSV読み込み時に使用する一時的な変数
	int read_csv_index = 0;
	int read_csv_data[] = { 0 };
	char tmp_chr[256];
	//CSVファイルからゲーム成績を取得する
	//CSVファイル内は英数字しか入力してないのでマルチバイト文字は考慮していない
	while (fp_log_write == NULL) {
		//ヘッダ部分は成績として読み込まないようにする
		if (read_csv_index == 0) {
			//ヘッダ部分を読み込んで1行分ポインタをずらす
			fgets(tmp_chr, str_max_row,fp_log_write);
			continue;
		}
		//CSVファイルから1文字ずつ読み込む
		get_str[read_csv_index] = fgetc(fp_log_write);
		//カンマを検知した場合
		if (get_str[read_csv_index] == 0x4c) {
			//カンマ以前に読み込んだ値を取得する
		//左波括弧を検知した場合
		}
		else if (get_str[read_csv_index] == 0x7b) {
			//間違った単語を宣言した構造体に取得していく
		}
		//改行コードを検知した場合
		else if (get_str[read_csv_index] == '\n') {
			read_csv_index == 0;
			continue;
		}
		//次の1文字を受け入れる為にインデックスをずらす
		++read_csv_index;
	}

	int current_question_index = 0;
	while (current_question_index < NUMBER_OF_QUESTION) {

		//問題出力用に乱数を設定する(下限:1,上限:get_max_words)
		for (int i = 0; i < NUMBER_OF_CHOICES + 1; i++) {
			random_number[i] = 1 + (int)(rand() * (get_max_words - 1 + 1.0) / (1.0 + RAND_MAX));
		}
		//選択肢に解答を含める場所を決める(下限:1,上限:NUMBER_OF_CHOICES)
		int answer_number = 1 + (int)(rand() * (NUMBER_OF_CHOICES - 1 + 1.0) / (1.0 + RAND_MAX));

		//英単語を4択で出力
		printf("Current question number: %d, Question: %s \n", current_question_index, get_json_key_pointer[random_number[0]]);
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
				printf("The answer: %s \n", get_json_value_pointer[random_number[0]]);
				//不正解になった問題のインデックスを記録する
				result_log.incorrectWordIndex[incorrect_word_of_number] = random_number[0];
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

	//正答率を計算する
	correct_answer_rate = (double)correct_answer_number / (double)current_question_index;
	//解答モードをプレイした回数のカウント
	++number_of_playing_question_mode;

	//今回の解答モードの成績を保存する
	result_log.playingLogId = number_of_playing_question_mode;
	result_log.correctAnswerRate = correct_answer_rate * 100;

	printf("Question mode is finished! \n");

	//今回の成績の表示
	printf("Your correct answer rate is %.0f Percent \n", result_log.correctAnswerRate);
	printf("Your play count is %d \n", result_log.playingLogId);
	printf("Your incorrect words is shown below \n");

	//今回の成績をCSVファイルに書き出す
	/**
	*CSVファイルの形式：{"id(int), clearRate(int), inccorentWordIndex(array)"}
	*/
	fprintf(fp_log_write, "%d,", result_log.playingLogId);
	fprintf(fp_log_write, "%.0f,", result_log.correctAnswerRate);
	fprintf(fp_log_write, "{");
	//不正解の単語が一つでもあった場合
	if (result_log.incorrectWordIndex[0] != 0) {
		while (result_log.incorrectWordIndex[incorrect_words_index]) {
			fprintf(fp_log_write, "%d", result_log.incorrectWordIndex[incorrect_words_index]);
			//間違った解答のArrayの要素が最終かどうかを検知する
			if (result_log.incorrectWordIndex[incorrect_words_index + 1] != 0) {
				fprintf(fp_log_write, ",");
			}
			else {
				fprintf(fp_log_write, "}\n");
			}
			//不正解の単語を表示する
			printf(">%s ", get_json_key_pointer[result_log.incorrectWordIndex[incorrect_words_index]]);
			++incorrect_words_index;
			printf("\n");
		}
	}
	//不正解の単語が無かった（全問正解）場合
	else {
		fprintf(fp_log_write, "}\n");
		printf("Perfect Clear!! \n");
	}

	printf("Please press any key to return main mode \n");

	fclose(fp_log_write);

	return 0;
}