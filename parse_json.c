#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
/*パースされたJSONのstring型を格納最大数*/
#define GET_MAX_STRING_TYPE 1000
/*マルチバイト文字のバイト数*/
#define MULTI_CHAR_BYTE 3
/*テスト用に制限する読み込み行数*/
#define INPUT_MAX_RECORDS 5

int main(void)
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
  /*JSONから取得した値（value）を格納するポインタ配列*/
  char *get_json_value_pointer[GET_MAX_STRING_TYPE] = {};

  /*JSONの型を判別する用の型を定義する*/
  JSON_TYPE check_type;

  /*check_type内のJSONのデータ型を区別する*/

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
      取得した1行から値（value）部分を取り出す
      JSONファイルを想定してコロンとカンマのポインタの差分から取得する
      */
      char *colon_address = strchr(get_str[index], (int)':');
      char *comma_address = strchr(get_str[index], (int)',');

      printf("colon_address: %d, comma_address: %d \n", colon_address, comma_address);

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

      /*JSONの値部分の先頭ポインタを取得する*/
      char *get_json_value_address = (char *)colon_address + 1;
      printf("get_json_value_address: %d \n", get_json_value_address);

      /*value部分を取得する*/
      get_json_value_pointer[index] = get_json_value_address;
      printf("index: %d, get value: %s \n", index, get_json_value_pointer[index]);

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
      char *double_quotation_address_first = strchr(get_json_value_pointer[index], (int)'\"');
      char *double_quotation_address_second = strrchr(get_json_value_pointer[index], (int)'\"');

      /*2文字目のダブルクォーテーションを削除する*/
      *double_quotation_address_second = '\0';

      /*文字列部分を取得する*/
      get_json_value_pointer[index] = double_quotation_address_first + 1;
      printf("index: %d, get value string: %s \n", index, get_json_value_pointer[index]);

      /*テスト用に読み込む行数を制限する*/
      if (index > INPUT_MAX_RECORDS)
      {
        break;
      }
    }

    /* tmp MEMO */
    // int j = 0;
    // printf("check value lead address: %d \n",get_json_value_pointer[j+1]); //value1 lead address1
    // printf("check value lead address: %d \n",get_json_value_pointer[j+2]); //value2 lead address2
    // printf("check value lead address: %d \n",get_json_value_pointer[j+3]); //same as above

    // int k = 0;
    // printf("check value get: %s \n",get_json_value_pointer[k+1]); //value1
    // printf("check value get: %s \n",get_json_value_pointer[k+2]); //value2
    // printf("check value get: %s \n",get_json_value_pointer[k+3]); //same as above

    printf("==file inputting process done==");
    fclose(fp);
    return 0;
  }

  /*ファイルの読み込みに失敗した場合*/
  printf("==file inputting failed==");
  return -1;
}