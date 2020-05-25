#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"

/*グローバル変数の宣言*/
char *get_json_key_pointer[MAX_RECORDS];
char *get_json_value_pointer[MAX_RECORDS];
int get_max_words;

/*
*@brief 問題（英単語）を出題する関数
*@param void
*@return: 0(処理正常終了)
*@detail: 英単語の出力、解答入力受付、解答出力を行う
*/
int question(void)
{
  printf("==question mode== \n");

  printf("get max words: %d \n", get_max_words);

  /*プログラム実行毎に異なる順序の乱数が出力されるようにする*/
  unsigned int current_time = (unsigned int)time(0);
  srand(current_time);

  while (1)
  {
    /*問題出力用に乱数を取得する*/
    int random_num = (int)(rand() * (100 - 1 + 1.0) / (1.0 + RAND_MAX));
    /*解答入力用の文字*/
    char answer_str[256] = {};

    /*問題出題・解答入力・解答出力*/
    printf("index: %d, Question: %s \n", random_num, get_json_key_pointer[random_num]);
    printf("Your answer> ");
    scanf("%s", answer_str);
    /*Escキー押下でプログラム終了*/
    // if((int)*answer_str == 0x1b)
    // {
    //   printf("question mode end");
    //   exit(0);
    // }
    printf("The answer: %s \n", get_json_value_pointer[random_num]);
    printf("==================================================\n");
  }

  return 0;
}