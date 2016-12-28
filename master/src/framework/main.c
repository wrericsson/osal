/*
 **************************************************************************************
 *       Filename:  main.c
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2014-04-20 22:32:41
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#include <stdio.h>
#include "mtype.h"
#include "osal_list.h"

boolean print_alphabet_list(void *data, void *user_data __unused)
{
	  char *alphabet;

	    printf("%s: \n", __func__);
	      alphabet = (char *)data;
	        printf("%c  ", *alphabet);
		  return TRUE;
}

int main(int argc __unused, char * argv[] __unused)
{
	osal_list_t *char_list = NULL;
	char alphabet;
	char alph[10];
	int i;

	alphabet = 'A';

	for (i = 0; i < 10; i++) {
		alph[i] = alphabet;
		char_list = osal_list_append(char_list, &alph[i], NULL, NULL);
		alphabet++;
	}

	osal_list_traverse(char_list, print_alphabet_list, NULL);
	printf("\n");

	char_list = osal_list_remove(char_list, &alph[2]);
	osal_list_traverse(char_list, print_alphabet_list, NULL);
	printf("\n");

	char_list = osal_list_remove(char_list, &alph[0]);
	osal_list_traverse(char_list, print_alphabet_list, NULL);
	printf("\n");

	char_list = osal_list_remove(char_list, &alph[9]);
	osal_list_traverse(char_list, print_alphabet_list, NULL);
	printf("\n");

	osal_list_free_list(char_list);

	return 0;
}
/********************************** END **********************************************/


