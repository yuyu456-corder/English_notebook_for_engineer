#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

/*JSONの型を一意に判別できるようにJSON_TYPE型を定義*/
typedef enum
{
  TYPE_STRING,
  TYPE_INT,
  TYPE_NULL,
  TYPE_BOOLEAN,
  TYPE_OBJECT,
  TYPE_ARRAY
} JSON_TYPE;

/*読み込むファイルの一行の最大文字数（データ数）*/
#define STR_MAX_ROW 1000
/*マルチバイト文字のバイト数*/
#define MULTI_CHAR_BYTE 3
/*テスト用に制限する読み込み行数*/
#define INPUT_MAX_RECORDS 5

/*グローバル変数の宣言*/
char *get_json_key_pointer[MAX_RECORDS];
char *get_json_value_pointer[MAX_RECORDS];

/*
*@brief JSONからkeyとValueを取り出す関数
*@param void
*@return: 0(ファイル読み込み成功),-1(ファイル読み込み失敗)
*@detail: この関数が呼ばれるとグローバル変数で配列にkeyとvalueの先頭ポインタが一行ずつ格納されていく
*/
int parse_json(void)
{

  FILE *fp;
  /*読み込むファイルの設定*/
  char *filename = "english_notebook.json";
  fp = fopen(filename, "r+");

  /*
    読み込むファイルの文字列を格納する配列
    1行ずつ読み込むことを想定して2次元配列にしている
  */
  char get_str[256][256] = {};
  /*読み込んだJSONの行数*/
  int get_max_records = 0;

  /*JSONの型を判別する用の型を定義する*/
  JSON_TYPE check_type;

  if (fp != NULL)
  {

    /*
    ファイルから1文字ずつ読み込み、1行ずつ格納する
    その1行からJSON(value)の値を取得する
    */
    for (int index = 0; fgets(get_str[index], STR_MAX_ROW, fp) != NULL; index++)
    {
      printf("============================================================\n");
      printf("index:%d \n", index);
      printf("get_str: %s \n", get_str[index]); //"key": "value"

      /*JSONの行数をカウントする*/
      ++get_max_records;

      /*
      取得した1行からkeyとvalue部分を取り出す
      keyは最初のダブルクォーテーションを先頭ポインタとし2回目のダブルクォーテーションを削除し取得する
      valueはコロンとカンマのポインタの差分から取得する
      */
      char *colon_address = strchr(get_str[index], (int)':');
      char *comma_address = strchr(get_str[index], (int)',');
      char *double_quotation_address_key_first = strchr(get_str[index], (int)'\"');

      printf("colon_address: %d, comma_address: %d, double_quotation_address: %d \n", colon_address, comma_address, double_quotation_address_key_first);

      /*JSONの値部分のメモリサイズを計算する*/
      int get_value_buffer_size = (int)(comma_address - colon_address);
      /*カンマがない最終行はコロンと改行文字の差分から取得する*/
      if ((comma_address == 0) && (get_value_buffer_size != 0))
      {
        char *new_line_char_address = strchr(get_str[index], (int)'\n');
        printf("new_line_char_address: %d \n", new_line_char_address);
        get_value_buffer_size = (int)(new_line_char_address - colon_address);
      }
      /*valueが無い行は次の行の処理に移る*/
      else if (get_value_buffer_size == 0)
      {
        continue;
      }
      printf("get_value_buffer_size: %d \n", get_value_buffer_size);

      /*key部分の先頭ポインタを取得する*/
      char *get_json_key_address = (char *)double_quotation_address_key_first + 1;
      printf("get_json_key_address: %d \n", get_json_key_address);
      /*value部分の先頭ポインタを取得する*/
      char *get_json_value_address = (char *)colon_address + 1;
      printf("get_json_value_address: %d \n", get_json_value_address);

      /*1行ごとに取得したkey部分を加工する為の配列型のポインタに格納する*/
      get_json_key_pointer[index] = get_json_key_address;
      /*1行ごとに取得したvalue部分を加工する為の配列型のポインタに格納する*/
      get_json_value_pointer[index] = get_json_value_address;

      /*key部分を取得する*/
      char *double_quotation_address_key_second = strchr(get_json_key_pointer[index], (int)'\"');
      *double_quotation_address_key_second = '\0';
      printf("index: %d, get key> %s \n", index, get_json_key_pointer[index]);

      /*取得した値部分からJSONの型を検知する*/
      // switch (check_type)
      // {
      // case TYPE_STRING:
      //   break;
      // }

      /*
      取得した行のvalueがstring型(TYPE_STRING)の場合の処理
      取得したポインタからvalueをポインタの値で1文字ずつ参照する
      文字列型の値を取り出したい為、最初と最後のダブルクォーテーション内の文字を取得する
      */
      int buffer_counter = 0;

      /*1,2文字目ダブルクォーテーションのポインタを取得*/
      char *double_quotation_address_string_first = strchr(get_json_value_pointer[index], (int)'\"');
      char *double_quotation_address_string_second = strrchr(get_json_value_pointer[index], (int)'\"');

      /*2文字目のダブルクォーテーションを削除する*/
      *double_quotation_address_string_second = '\0';

      /*文字列部分を取得する*/
      get_json_value_pointer[index] = double_quotation_address_string_first + 1;
      printf("index: %d, get value string> %s \n", index, get_json_value_pointer[index]);

      /*テスト用に読み込む行数を制限する*/
      if (index > INPUT_MAX_RECORDS)
      {
        break;
      }
    }

    /* tmp MEMO */
    // int j = 0;
    // printf("check value lead address: %d \n",get_json_value_pointer[j+1]); //value1 lead address
    // printf("check value lead address: %d \n",get_json_value_pointer[j+2]); //value2 lead address
    // printf("check value lead address: %d \n",get_json_value_pointer[j+3]); //same as above

    // int k = 0;
    // printf("check value get: %s \n",get_json_value_pointer[k+1]); //value1
    // printf("check value get: %s \n",get_json_value_pointer[k+2]); //value2
    // printf("check value get: %s \n",get_json_value_pointer[k+3]); //same as above

    printf("==file inputting process done== \n");
    fclose(fp);

    return 0;
  }

  /*ファイルの読み込みに失敗した場合*/
  printf("==file inputting failed== \n");
  return -1;
}