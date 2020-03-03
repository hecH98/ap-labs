#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mergeSort(char**, int, int);
void mergeString(char*[], int, int);
void mergeInt(char*[], int, int);
int isNumber;

int main(int argc, char** argv){
	printf("------ Welcome to generic merge sort ------\n");
	char** array = argv;
	// for ( i = 0; i < argc - 1; i++) {
	// 	printf("%s,",argv[i+1]);
	// 	array[i] = argv[i+1];
	int i;
	for ( i = 1; i < argc; i++){
		if ( strcmp("0", argv[i]) == 0) {
			isNumber = 1;
			printf("%s,",argv[i]);
		}
		else if ( atoi(argv[i]) ){
			isNumber = 1;
		}
		else {
			isNumber = 0;
			break;
		}
	}
	// printf("\n");
	// if (isNumber) {
	// 	printf("true\n");
	// } else {
	// 	printf("false\n");
	// }
	// if (atoi(argv[1])) {
	// 	printf("true\n");
	// } else {
	// 	printf("false\n");
	// }

	printf("The unsorted array is: \n");
	for ( i = 1; i < argc; i++) {
		printf("%s,",array[i]);
	}
	printf("\n");

	mergeSort(array, 1, argc-1);

	printf("The sorted array is: \n");
	for ( i = 1; i < argc; i++) {
		printf("%s,",array[i]);
	}
	printf("\n");

	return 0;
}

void mergeSort(char** array, int min, int max){
	if( min < max) {
		int average = (min+max)/2;
		mergeSort(array, min, average);
		mergeSort(array, average+1, max);
		if (isNumber) {
			mergeInt(array, min, max);
		}
		else {
			mergeString(array, min, max);
		}
	}
}

void mergeString(char* array[], int min, int max){
	int length = max-min+1;
	void** temp = malloc(sizeof(char)*(length));
	int average = (min+max)/2;
	int i, j = min, k = average + 1;
	// printf("size of temp: %lu\n",sizeof(temp));
	// printf("array to check: \n");
	// for ( i = min; i <= max; i++) {
	// 	printf("%s,",array[i]);
	// }

	// int[] array = [43, 0, 98, 2431, -32,7, 1, -197];
	// int[] solvedArray = [-197, -32, 0, 1, 7, 43, 98, 2431];

	// printf("\n");
	// printf("min: %d\n",min);
	// printf("max: %d\n",max);
	// printf("array[j]: %s\n",array[j]);
	// printf("array[k]: %s\n",array[k]);
	for( i = 0; i < length; i++) {
		// printf("checking data...\n");
		// printf("array[j]: %d\n",array[j]);
		// printf("array[k]: %d\n",array[k]);
		if( j <= average && k <= max ) {
			if( strcmp(array[j],  array[k]) < 0/*array[j] < array[k]*/ ) {
				temp[i] = array[j++];
				// printf("if 1, temp[%d]: %s\n",i,temp[i]);
			}
			else {
				temp[i] = array[k++];
				// printf("else 1, temp[%d]: %s\n",i,temp[i]);
			}
		}
		else if( j <= average ) {
			temp[i] = array[j++];
			// printf("if 2, temp[%d]: %s\n",i,temp[i]);
		}
		else {
			temp[i] = array[k++];
			// printf("else 2, temp[%d]: %s\n",i,temp[i]);
		}
	}
	// printf("printing temp...\n");
	// for ( i = 0; i < length; i++) {
	// 	printf("%s,",temp[i]);
	// }
	// printf("\n");
	// int l = min;
	int p = 0;
	for ( i = min; i <= max; i++) {
		array[i] = temp[p++];
	}
	// for ( i = min; i <= max; i++) {
	// 	printf("%s,",array[i]);
	// }
	// printf("\n");
	free(temp);
	// for( i = min; i <= max; i++) {
	// 	printf("%d,",array[i]);
	// }
	// printf("\nfin de la funcion\n");
}

void mergeInt(char* array[], int min, int max){
	int length = max-min+1;
	void** temp = malloc(sizeof(char)*(length));
	int average = (min+max)/2;
	int i, j = min, k = average + 1;
	// printf("size of temp: %lu\n",sizeof(temp));
	// printf("array to check: \n");
	// for ( i = min; i <= max; i++) {
	// 	printf("%s,",array[i]);
	// }

	// int[] array = [43, 0, 98, 2431, -32,7, 1, -197];
	// int[] solvedArray = [-197, -32, 0, 1, 7, 43, 98, 2431];

	// printf("\n");
	// printf("min: %d\n",min);
	// printf("max: %d\n",max);
	// printf("array[j]: %s\n",array[j]);
	// printf("array[k]: %s\n",array[k]);
	for( i = 0; i < length; i++) {
		// printf("checking data...\n");
		// printf("array[j]: %d\n",array[j]);
		// printf("array[k]: %d\n",array[k]);
		if( j <= average && k <= max ) {
			if( atoi(array[j]) < atoi(array[k]) ) {
				temp[i] = array[j++];
				// printf("if 1, temp[%d]: %s\n",i,temp[i]);
			}
			else {
				temp[i] = array[k++];
				// printf("else 1, temp[%d]: %s\n",i,temp[i]);
			}
		}
		else if( j <= average ) {
			temp[i] = array[j++];
			// printf("if 2, temp[%d]: %s\n",i,temp[i]);
		}
		else {
			temp[i] = array[k++];
			// printf("else 2, temp[%d]: %s\n",i,temp[i]);
		}
	}
	// printf("printing temp...\n");
	// for ( i = 0; i < length; i++) {
	// 	printf("%s,",temp[i]);
	// }
	// printf("\n");
	// int l = min;
	int p = 0;
	for ( i = min; i <= max; i++) {
		array[i] = temp[p++];
	}
	// for ( i = min; i <= max; i++) {
	// 	printf("%s,",array[i]);
	// }
	// printf("\n");
	free(temp);
	// for( i = min; i <= max; i++) {
	// 	printf("%d,",array[i]);
	// }
	// printf("\nfin de la funcion\n");
}
