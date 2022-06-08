#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void factors(int n);
int main(int argc, char *argv[]) {
	
	int num;
	printf("Enter Number:");
	if (scanf("%d", &num) != 1) {
		printf("Not a valid number\n");
	} else {
		factors(num);
	}

	return 0;

}

void factors(int n) {

	int arr_size = 10;
	int *iarr = malloc(sizeof(int)*arr_size);
	int *iarr_new;
	int count = -1;
	for (int i=1; i <= n; i++) {
		if(n % i == 0) {
			if (count == arr_size) {
				arr_size *= 2;
				iarr_new = realloc(iarr,arr_size);
				if (iarr_new == NULL) {
					printf("unable to allocate space\n");
					exit(1);
				} else {

					iarr = iarr_new;
				}
			}
			
			count++;
			*(iarr + count) = i;
		}
	}
	
	if (count >= 0) {
		for (int j = 0; j <= count; j++)
			printf("%d ", *(iarr + j));
	} else {
		printf("No factors for the number %d\n", n);
	}
	printf("\n");
}

