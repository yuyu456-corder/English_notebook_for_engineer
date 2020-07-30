#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "header.h"

//読み込む成績CSVファイルのカラム数
#define READ_CSV_COLUMN 4
//読み込むファイルの一行の最大文字数（データ数）
#define STR_MAX_ROW 100
//読み込む成績CSVファイルから読み込む上限数
#define MAX_READ_PLAYING_DATA 100

/**
*@brief ゲーム成績をCSVファイルから読み込む
*@param result_log 成績データを保存している構造体
*@return: 0(正常終了),-1(異常終了)
*/
int read_game_score_csv(result_log_t* result_log){

  //読み込むファイルの設定
	FILE* fp_log_write;
	char* file_name = "play_data_of_question_mode.csv";
	fp_log_write = fopen(file_name, "r");
	//ファイルのオープンに失敗した場合
	if (fp_log_write == NULL) {
		printf("Error: can't open log file... \n");
		return -1;
	}

  //解答モードの総プレイ回数（初期化は成績データのCSVファイルを読み込み時に行う）
	int playing_count;

	//CSVファイルからゲーム成績を取得する
	//CSVファイルを文字列リテラルとして一度パースする
	char get_csv_elements[READ_CSV_COLUMN][STR_MAX_ROW] = { '\0' };
	char* tmp_incorrect_word = '\0';
	//CSVの各要素が代入に成功したかの判定を行うフラグ値
	int res_fscanf = 0;
	//読み込んだCSVの行数のカウンタ
	int current_csv_line = 1;
	//成績データを代入する構造体のインデックス
	int result_log_index = 0;
	printf(">get your playing data \n");
  //読み込むCSVの要素数が指定数と異なる場合読み込みを止める
	while ((res_fscanf = fscanf(fp_log_write, "%[^,],%[^,],%[^,],%s", get_csv_elements[0], get_csv_elements[1], get_csv_elements[2], get_csv_elements[3])) == READ_CSV_COLUMN) {
		printf("parse_csv: %d %s %s %s %s \n", res_fscanf, get_csv_elements[0], get_csv_elements[1], get_csv_elements[2], get_csv_elements[3]);
		//ヘッダ部分はデータとして含めない
		if (current_csv_line == 1) {
			//CSVファイルにデータが無い場合プレイ回数は0となる
			result_log[result_log_index].playingLogId = 0;
			++current_csv_line;
			continue;
			//データの読み込み上限に達したら処理を抜ける
		}
		else if (current_csv_line > MAX_READ_PLAYING_DATA) {
			break;
		}
		//各要素を対応する構造体に代入する
		//プレイ回数（データID）の取得
		result_log[result_log_index].playingLogId = atoi(get_csv_elements[0]);
		printf("result log index: %d, playing id: %d \n", result_log_index, result_log[result_log_index].playingLogId);
		//プレイ時刻の取得
		result_log[result_log_index].playingDate = atol(get_csv_elements[1]);
		//UNIX時間で取得するため時刻に換算する
		char tmp_read_time[256] = { '\0' };
		struct tm tm;
		localtime_s(&tm, &result_log[result_log_index].playingDate);
		strftime(tmp_read_time, sizeof(tmp_read_time), "%Y/%m/%d %a %H:%M:%S", &tm);
		printf("playing date is %s \n", tmp_read_time);
		//正答率の取得
		result_log[result_log_index].correctAnswerRate = atoi(get_csv_elements[2]);
		printf("result log index: %d, answer rate: %3.2f \n", result_log_index, result_log[result_log_index].correctAnswerRate);
		//不正解単語の取得
		tmp_incorrect_word = get_csv_elements[3];	//e.g. {,12,21,3,}
		printf("incorrect word index: %s \n", tmp_incorrect_word);
		//不正解の単語はまだCSV形式のままなので、再度パースを行う
		//構造体に代入する際に使用するインデックス
		int data_structure_index = 0;
		//代入すべきインデックスのデータサイズ（半角文字のみを想定しているのでそのまま文字数としても扱える）
		int incorrect_word_index_buffer_size = 0;
		//現在参照しているCSV形式(不正解の単語のインデックスの文字列リテラル)
		int tmp_incorrect_word_index = 0;
		//対象文字列を1文字ずつ参照してCSV形式の文字列リテラルをパースする
		//半角スペースで行の末尾を検知する
		while (*(get_csv_elements[3] + tmp_incorrect_word_index) != 0x20) {
			//カンマを検知したら、データサイズ分だけ構造体に不正解単語のインデックスを取得する
			if (*(get_csv_elements[3] + tmp_incorrect_word_index) == ',') {
				//最初のカンマ検知は波括弧のみなので取得しないようにする
				if (incorrect_word_index_buffer_size == 0) {
					++tmp_incorrect_word_index;
					continue;
				}
				char tmp_get_incorrect_word_index[256] = { '0' };
				//カンマ検知時に直前の要素を一時変数に保存する
				strncpy(tmp_get_incorrect_word_index, get_csv_elements[3] + tmp_incorrect_word_index - incorrect_word_index_buffer_size, incorrect_word_index_buffer_size);
				//不正解単語のインデックスを取得
				result_log[result_log_index].incorrectWordIndex[data_structure_index] = atoi(tmp_get_incorrect_word_index);
				printf("result log index: %d, incorrect word parse index: %d \n", result_log_index, result_log[result_log_index].incorrectWordIndex[data_structure_index]);
				//取得したインデックスから単語を紐づける
				printf("incorrect word: %s \n", parse_json_string_p->get_json_value_pointer[result_log[result_log_index].incorrectWordIndex[data_structure_index]]);
				//受け入れる構造体のインデックスと参照中のインデックスをずらす
				++data_structure_index;
				++tmp_incorrect_word_index;
				//次の要素を読み込みたい為、要素の文字数をリセットする
				incorrect_word_index_buffer_size = 0;
				//波括弧はデータに含めない
			}
			else if ((*(get_csv_elements[3] + tmp_incorrect_word_index) == '{') || (*(get_csv_elements[3] + tmp_incorrect_word_index) == '}')) {
				++tmp_incorrect_word_index;
				continue;
				//不正解単語のインデックス部分の処理
			}
			else {
				++tmp_incorrect_word_index;
				//インデックス部分は文字数をカウントする
				++incorrect_word_index_buffer_size;
			}
		}
		//次の行を読み込むのでカウンタを増やす
		++current_csv_line;
		//次の構造体に成績データを代入するのでインデックスをずらす
		++result_log_index;
		printf("==================================================\n");
	}
  fclose(fp_log_write);
  return 0;
}