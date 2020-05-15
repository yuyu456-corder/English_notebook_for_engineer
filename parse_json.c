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
#define STR_MAX_ROW 100
/*全角文字のバイト数*/
#define MULTI_BYTE_STR 3

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
  /*読み込んだ文字列の値（value）部分を行ごとに格納する*/
  char get_value[256][256] = {};
  /*JSONから取得した値（value）を格納するポインタ配列*/
  char *get_json_value_pointer[STR_MAX_ROW] = {};

  /*JSONの型を判別する用の型を定義する*/
  JSON_TYPE check_type;

  /*check_type内のJSONのデータ型を区別する*/

  if (fp != NULL)
  {

    /*ファイルから1文字ずつ読み込み、1行ずつ格納していく*/
    for (int i = 0; fgets(get_str[i], STR_MAX_ROW, fp) != NULL; i++)
    {
      printf("============================================================\n");
      printf("index:%d \n", i);
      printf("get_str: %s \n", get_str[i]); //"key": "value"

      /*
      取得した1行から値（value）部分を取り出す
      JSONファイルを想定してコロンとダブルクォーテーションのポインタの差分から取得する
      */
      char *colon_address = strchr(get_str[i], (int)':');
      /*文字列の最後からダブルクォーテーションを探索する*/
      char *double_quotation_address = strrchr(get_str[i], (int)'\"');
      printf("colon_address: %d, double_quotation_address: %d \n", colon_address, double_quotation_address);

      /*JSONの値部分のメモリサイズを計算する*/
      int get_value_buffer_size = (int)(double_quotation_address - colon_address);
      printf("get_value_buffer_size: %d \n", get_value_buffer_size);

      /*JSONの値部分の先頭ポインタを取得する*/
      char *get_json_value_address = (char *)colon_address + 1;
      printf("get_json_value_address: %d \n", get_json_value_address);

      /*取得したポインタから値(value)を1文字ずつ参照する*/
      int buffer_counter = 0;
      while (buffer_counter != get_value_buffer_size)
      {
        printf("buffer counter: %d \n", buffer_counter);
        /*マルチバイト文字を検知したらそのバイト分ポインタをずらす*/
        if (isprint(*(get_json_value_address + buffer_counter)) == 0)
        {
          /*マルチバイト文字の最初の先頭1バイトへのアドレスを取得する*/
          get_json_value_pointer[i] = get_json_value_address + buffer_counter;
          printf("index: %d, get value: %s \n", i, get_json_value_pointer[i]);
          /*マルチバイト文字分ポインタをずらす*/
          buffer_counter = buffer_counter + MULTI_BYTE_STR;
        }
        /*マルチバイト文字以外は1バイト分ポインタをずらす*/
        else
        {
          ++buffer_counter;
        }
      }

      printf("\n");
    }

    /*読み込んだ文字のJSONデータ型ごとに変数に格納する*/
    // switch(check_type){
    //   case TYPE_STRING:
    /*文字列型として変数に格納するロジック*/
    //     break;
    // }

    printf("file inputting process done");
    fclose(fp);
    return 0;
  }

  /*ファイルの読み込みに失敗した場合*/
  printf("file inputting failed");
  return -1;
}