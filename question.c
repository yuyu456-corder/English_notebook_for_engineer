#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "header.h"

//�o�肷���萔
#define number_of_question 10
//�o�肷��𓚂̑I�����̐�
#define number_of_choices 4

//�O���[�o���ϐ��̐錾
char* get_json_key_pointer[MAX_RECORDS];
char* get_json_value_pointer[MAX_RECORDS];
int get_max_words;

/*
*@brief �p�P���4���`���ɏo�肵�A�𓚂��󂯕t����֐�
*@param void
*@return: 0(��������I��)
*/
int question(void) {

	printf("==question mode== \n");
	printf("please press number key correspond to an answer > \n");

	//���o�͗p�̗������m��(�I�����{���)
	int random_number[number_of_choices + 1] = { 0 };

	//�v���O�������s���ɈقȂ鏇���̗������o�͂����悤�ɂ���
	unsigned int current_time = (unsigned int)time(0);
	srand(current_time);

	int current_question_index = 1;
	while (current_question_index <= number_of_question) {

		//���o�͗p�ɗ�����ݒ肷��
		for (int i = 0; i < number_of_choices + 1; i++) {
			random_number[i] = (int)(rand() * (get_max_words - 1 + 1.0) / (1.0 + RAND_MAX));
			//printf("get random number: %d \n", random_number[i]);
		}
		//�I�����ɉ𓚂��܂߂�ꏊ�����߂�
		int answer_number = (int)(rand() * (number_of_choices - 1 + 1.0) / (1.0 + RAND_MAX));

		//�p�P���4���ŏo��
		printf("current question number: %d, Question: %s \n", current_question_index, get_json_key_pointer[random_number[0]]);
		for (int i = 1; i <= number_of_choices; i++) {
			//�𓚂̑I�������o��
			if (i == answer_number) {
				printf("%d: %s / ", i, get_json_value_pointer[random_number[0]]);
				continue;
			}
			printf("%d: %s / ", i, get_json_value_pointer[random_number[i]]);
		}
		printf("\n");
		//�L�[�{�[�h���͂����m���ĉ𓚂��o�͂���
		while (1) {
			//Esc�L�[�����Ńv���O�����I��
			if (getch() == 0x1b) {
				printf("english_notebook mode end \n");
				printf("please Enter key to return main mode \n");
				return 0;
			}
			else if (getch() != 0x1b) {
				printf("The answer: %s \n", get_json_value_pointer[random_number[0]]);
				break;
			}
		}

		++current_question_index;
		printf("==================================================\n");
	}

	printf("question mode is finished! \n");

	return 0;
}