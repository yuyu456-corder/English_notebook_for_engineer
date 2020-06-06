#include <stdio.h>
#include "header.h"

//グローバル変数の宣言
char* get_json_key_pointer[MAX_RECORDS];
char* get_json_value_pointer[MAX_RECORDS];

int main(void)
{
	printf("==english note book for engineer== \n");

	//英単語をまとめたJSONをパースする
	parse_json();

	printf("flag:0 > question_mode flag:1 > english_notes_mode \n");
	printf("please press number key to migrate mode > ");

	//モード遷移用のフラグ
	int mode_flag = 0;
	while (1) {

		//入力モードを受け付ける
		mode_flag = getchar();

		switch (mode_flag) {
		case '0' :
			//問題解答モード
			question();
			break;
		case '1' :
			//単語帳モード
			english_notebook();
			break;
		//無効な値または、他のモードが終了した場合
		default :
			printf("please press number key to migrate mode > \n");
		}
	}
	//tmp_test
	// printf("get json key: %s \n", get_json_key_pointer[1]);     //json_key
	// printf("get json value: %s \n", get_json_value_pointer[1]); //json_value

	return 0;
}