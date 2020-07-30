#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "header.h"

//上位いくつまでの単語を不正解の多かった単語として扱うか
#define TOP_OF_INCORRECT_WORDS 10

/**
* 読み込んだCSVデータの単語の情報をコピーする用の一時的な構造体
*/
typedef struct {
	int tmp_word_id; //!<DBとして扱うためのID(UNIQUE KEYかつFOREIGN KEY)
	int tmp_incorrect_count; //!< 不正解回数
} tmp_word_attributes_t;

//不正解数を昇順にソートするqsort用の関数
//返り値が負か正によって降順、昇順かが決まる
int sort_structure_ascending_order(const void* get_num_a, const void* get_num_b)
{
	return ((tmp_word_attributes_t*)get_num_b)->tmp_incorrect_count - ((tmp_word_attributes_t*)get_num_a)->tmp_incorrect_count;
}

/**
*@brief 読み込んだ成績データから平均正答率などの統計データを求める
*@param result_log 成績データを保存している構造体
*@return: 0(正常終了),-1(異常終了)
*/
int calculation_score_data(result_log_t* result_log){

	//成績データが無ければこの関数から抜ける
	if (result_log[0].playingLogId != 0) {
    return 0;
  }

  //読み込んだCSVデータの単語の情報をコピーする用の一時的な構造体配列の宣言と初期化
	tmp_word_attributes_t* s_tmp_word_attributes;
	size_t element_size_of_tmp_word_attributes = sizeof(tmp_word_attributes_t);
	s_tmp_word_attributes = malloc(element_size_of_tmp_word_attributes * get_max_words);
	memset(s_tmp_word_attributes, 0, sizeof(element_size_of_tmp_word_attributes * get_max_words));
	if (s_tmp_word_attributes == NULL) {
		printf("Error: Memory allocation failed");
		return -1;
	}

  printf("*************************\n");
  printf("*===your playing data===*\n");
  printf("*************************\n");

  //総プレイ回数
  int total_play_count = 0;
  //正答率の合計値	
  double sum_corrent_rate = 0;
  //正答率の平均値
  double average_corrent_rate = 0;
  //特に不正解数が多かった単語
  int top_of_incorrect_words[TOP_OF_INCORRECT_WORDS] = { 0 };

  //構造体result_logにゲーム成績があるためループ処理で全ての要素を参照して集計する
  int reference_result_log_index = 0;
  while (result_log[reference_result_log_index].playingLogId) {
    //正答率を加算して最終的に合計値を求める
    sum_corrent_rate += result_log[reference_result_log_index].correctAnswerRate;
    //不正解単語のインデックスを取得して各単語ごとに不正解数を加算していく
    //for (int i = 0; i < NUMBER_OF_QUESTION; i++) {
      //NULLへのインデックスは取得しない
      //if (result_log[reference_result_log_index].incorrectWordIndex[i] == NULL) {
      //	continue;
      //}
      //printf("now get data> index incorrect word: %d \n", result_log[reference_result_log_index].incorrectWordIndex[i]);
      //取得したインデックスをIDとして疑似データベースと紐づける(既に紐づけてあるインデックスは行わない)
      //if (s_word_attributes[result_log[reference_result_log_index].incorrectWordIndex[i]]->word_id != NULL) {
      //	s_word_attributes[result_log[reference_result_log_index].incorrectWordIndex[i]]->word_id = result_log[reference_result_log_index].incorrectWordIndex[i];
      //}
      //不正解単語のIDと対応する値を加算することで不正解数を集計していく
      //++s_word_attributes[result_log[reference_result_log_index].incorrectWordIndex[i]]->incorrect_count;
    //}
    //次の成績データを参照する
    ++reference_result_log_index;
  }
  //総プレイ回数の取得
  total_play_count = reference_result_log_index;
  printf(">total play count: %d \n", total_play_count);
  //正答率の平均値の計算
  average_corrent_rate = sum_corrent_rate / (double)total_play_count;
  printf(">average correct rate: %.2f \n", average_corrent_rate);
  //不正解が特に多かった単語の表示（表示上限はTOP_OF_INCORRECT_WORDSで設定）
  // printf(">top of incorrect words is shown below \n");
  //不正解数を元にコピーした構造体を昇順にソートする（コピーは元の構造体を壊さないようにするため）
  //&tmp_word_attributes = s_word_attributes;
  //使用中の構造体のデータ数を計算する
  //int size_of_tmp_word_attributes = sizeof(tmp_word_attributes) / sizeof(tmp_word_attributes_t);
  //qsort(tmp_word_attributes, size_of_tmp_word_attributes, sizeof(tmp_word_attributes_t), sort_ascending_order);
  //昇順でソート済みのため上位のデータから表示していく
  //for (int i = 1; i < TOP_OF_INCORRECT_WORDS; i++) {
  //	printf("%d: %s \n", i, parse_json_string_p->get_json_key_pointer[tmp_word_attributes.tmp_word_id]);
  //}

  //一時的な構造体の為、確保したメモリを開放する
  free(s_tmp_word_attributes);
  return 0;
}