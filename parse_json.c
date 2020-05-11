#include <stdio.h>

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
#define STR_ROW_MAX 100

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

  /*JSONの型を判別する用の型を定義する*/
  JSON_TYPE check_type;

  /*check_type内のJSONのデータ型を区別する*/

  if (fp != NULL)
  {

    /*ファイルから1文字ずつ読み込み、1行ずつ格納していく*/
    for (int i = 0; fgets(get_str[i], STR_ROW_MAX, fp) != NULL; i++)
    {
      printf("get_str: %s \n", get_str[i]);
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
