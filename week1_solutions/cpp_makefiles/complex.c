#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct complexN {
	
	float re;
	float im;
};

struct complexN add(struct complexN n1, struct complexN n2);

int main(int argc, char *argv[]) {
	struct complexN cN1, cN2, result;
	printf("Enter first complex number:");
	if (fscanf(stdin, "%f %f", &cN1.re, &cN1.im) != 2) {
		printf("error in values entered\n");
		exit(1);
	}
	printf("Enter second complex number:");
	if (fscanf(stdin, "%f %f", &cN2.re, &cN2.im) != 2) {
		printf("error in values entered\n");
		exit(1);
	}
	result = add(cN1, cN2);
	printf("Result: %f %f\n", result.re, result.im);

	return 0;
}

struct complexN add(struct complexN n1, struct complexN n2) {
	struct complexN r;

	r.re = n1.re + n2.re;
	r.im = n1.im + n2.im;
	return r;
}
