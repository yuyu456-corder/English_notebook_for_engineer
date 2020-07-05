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

//グローバル変数の宣言
char* get_json_key_pointer[MAX_RECORDS];
char* get_json_value_pointer[MAX_RECORDS];
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
	char read_csv_char[256] = { '\0' };
	//CSVファイルを読み込む際のカンマのカウンタ
	int read_csv_conmma_count = 0;
	//CSVファイル読み込み時にバッファ用の配列のデータサイズ
	int buffer_size_read_csv = 0;
	//モードをプレイした時に間違えた単語を保存する
	int incorrect_word_index[NUMBER_OF_QUESTION] = { 0 };

	//読み込む・書きこむファイルの設定
	FILE* fp_log_write;
	char* file_name = "play_data_of_question_mode.csv";
	fp_log_write = fopen(file_name, "a+");
	//ファイルのオープンに失敗した場合
	if (fp_log_write == NULL) {
		printf("error: can't open log file... \n");
		return -1;
	}

	//プログラム実行毎に異なる順序の乱数が出力されるようにする
	unsigned int current_time = (unsigned int)time(0);
	srand(current_time);

	//CSVから読み込んだデータを格納する時に用いるインデックス
	int read_csv_data_index = 0;
	//読み込んでいるCSVの現在の行数をカウントする
	int read_csv_file_row = 0;
	//CSVファイルからゲーム成績を取得する
	//CSVファイル内は英数字しか入力してないのでマルチバイト文字は考慮していない
	//!!これだとwhileの条件文がずっと一行目を読み込みEOFが検知できない
	for (read_csv_file_row; fgets(read_csv_char, sizeof(read_csv_char), fp_log_write) != NULL; ++read_csv_file_row) {
		//ヘッダ部分は成績として読み込まないようにする
		if (read_csv_file_row == 0) {
			++read_csv_file_row;
			continue;
		}
		//CSVファイルから1文字ずつ読み込む
		read_csv_char[read_csv_data_index] = fgetc(fp_log_write);
		//カンマを検知した場合
		if (read_csv_char[read_csv_data_index] == 0x4c) {
			++read_csv_conmma_count;
			//何番目のカンマかでどの要素かを検知してそれ以前に読み込んだ値をデータとして取得する
			switch (read_csv_conmma_count) {
				//IDを取得
			case 1:
				result_log.playingLogId = atoi(read_csv_char);
				printf("result_log.playingLogId: %d \n", result_log.playingLogId);
				//次の要素を読み込むため、読み込んだ文字列をクリアする
				buffer_size_read_csv = strlen(read_csv_char);
				memset(read_csv_char, '\0', buffer_size_read_csv);
				read_csv_data_index = 0;
				break;
				//正答率を取得
			case 2:
				result_log.correctAnswerRate = atoi(read_csv_char);
				printf("result_log.correctAnswerRate: %d \n", result_log.correctAnswerRate);
				//読み込んだ文字列のクリア
				buffer_size_read_csv = strlen(read_csv_char);
				memset(read_csv_char, '\0', buffer_size_read_csv);
				read_csv_data_index = 0;
				break;
				//defaultはDBに見立てたCSVの最終カラムに対する処理
				//間違った単語のインデックスを取得
			default:
				//波括弧は成績データとして扱わない
				if (read_csv_char[read_csv_data_index] == '{') {
					//読み込んだ文字列のクリア
					buffer_size_read_csv = strlen(read_csv_char);
					memset(read_csv_char, '\0', buffer_size_read_csv);
					read_csv_data_index = 0;
					continue;
				}
				else if (read_csv_char[read_csv_data_index] == '}') {
					read_csv_char[read_csv_data_index] = '\0';
					continue;
				}
				//CSVの最終要素はカンマでなく改行文字で検知し取得する
				if (read_csv_char[read_csv_data_index] == '\n') {
					result_log.incorrectWordIndex[incorrect_words_index] = atoi(read_csv_char);
					++incorrect_words_index;
					printf("result_log.correctAnswerRate: %d \n", result_log.incorrectWordIndex[incorrect_words_index]);
					//読み込んだ文字列のクリア
					buffer_size_read_csv = strlen(read_csv_char);
					memset(read_csv_char, '\0', buffer_size_read_csv);
					read_csv_data_index = 0;
					//改行文字の検知で、その行のカンマを全て検知した事が分かったためカウンタをリセットする
					read_csv_conmma_count = 0;
					++read_csv_file_row;
				//最終要素以外は通常通りカンマで検知し取得する
				}
				else {
					result_log.incorrectWordIndex[incorrect_words_index] = atoi(read_csv_char);
					++incorrect_words_index;
					printf("result_log.correctAnswerRate: %d \n", result_log.incorrectWordIndex[incorrect_words_index]);
					//読み込んだ文字列のクリア
					buffer_size_read_csv = strlen(read_csv_char);
					memset(read_csv_char, '\0', buffer_size_read_csv);
					read_csv_data_index = 0;
				}
			}
		}
		//次の1文字を受け入れる為にインデックスをずらす
		++read_csv_data_index;
		//次の行のデータを受け入れるためにインデックスを削除する
		incorrect_words_index = 0;
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

	//正答率を計算する
	correct_answer_rate = (double)correct_answer_number / (double)current_question_index;
	//解答モードをプレイした回数のカウント
	++result_log.playingLogId;

	printf("Question mode is finished! \n");

	//今回の成績の表示
	printf("Your correct answer rate is %.0f Percent \n", correct_answer_rate * 100);
	printf("Your play count is %d \n", result_log.playingLogId);
	printf("Your incorrect words is shown below \n");

	/**
	*今回の成績をCSVファイルに書き出す
	*CSVファイルの形式：{"id(int), clearRate(int), inccorentWordIndex(array)"}
	*/
	fprintf(fp_log_write, "%d,", result_log.playingLogId);
	fprintf(fp_log_write, "%.0f,", correct_answer_rate * 100);
	fprintf(fp_log_write, "{");
	//不正解の単語が一つでもあった場合
	if (incorrect_word_index[0] != 0) {
		while (incorrect_word_index[incorrect_words_index]) {
			fprintf(fp_log_write, "%d", incorrect_word_index[incorrect_words_index]);
			//間違った解答のArrayの要素が最終かどうかを検知する
			if (incorrect_word_index[incorrect_words_index + 1] != 0) {
				fprintf(fp_log_write, ",");
			}
			else {
				fprintf(fp_log_write, "}\n");
			}
			//不正解の単語を表示する
			printf(">%s ", get_json_key_pointer[incorrect_word_index[incorrect_words_index]]);
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