#include "../main.h"

/**
 * main - entry
 *
 * Return: always 0
 */
int main(void)
{
	char *pth;
	int exists_ = exists("clss", &pth);

	printf("Is_Program: %d, %s\n", exists_, pth);
	return (0);
}
