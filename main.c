#include <stdio.h>
#include "header.h"

/*グローバル変数の宣言*/
char *get_json_key_pointer[MAX_RECORDS];
char *get_json_value_pointer[MAX_RECORDS];

int main(void)
{
  printf("==english note book for engineer== \n");

  /*英単語をまとめたJSONをパースする*/
  parse_json();

  /*問題を出題する*/
  question();

  /*tmp_test*/
  // printf("get json key: %s \n", get_json_key_pointer[1]);     //json_key
  // printf("get json value: %s \n", get_json_value_pointer[1]); //json_value

  return 0;
}