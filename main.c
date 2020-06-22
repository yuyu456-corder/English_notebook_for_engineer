#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include "header.h"

//グローバル変数の宣言
char* get_json_key_pointer[MAX_RECORDS];
char* get_json_value_pointer[MAX_RECORDS];

int main(void)
{
	printf("==English note book for engineer== \n");

	//英単語をまとめたJSONをパースする
	parse_json();

	printf("Press 0 -> question mode, press 1 -> english notes mode \n");
	printf("If you press ESC key, this system is exited \n");
	printf("Please press number key to migrate mode > \n");

	while (1) {

		//入力モードを受け付ける
		char mode_flag = '\0';
		mode_flag = getch();

		//入力された文字をチェックする
		switch (mode_flag) {
		case '0':
			//問題解答モードへ移行する
			question();
			break;
		case '1':
			//単語帳モードへ移行する
			english_notebook();
			break;
		case 0x1b:
			//システムの正常終了
			exit(0);
			//無効な値の場合
		default:
			printf("Please press number key to migrate mode > \n");
		}
	}
	//tmp_memo
	// printf("get json key: %s \n", get_json_key_pointer[1]);     //json_key
	// printf("get json value: %s \n", get_json_value_pointer[1]); //json_value

	return 0;
}