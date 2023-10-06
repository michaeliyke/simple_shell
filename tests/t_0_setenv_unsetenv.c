#include "../main.h"

/**
 * main - entry
 *
 * Return: always 0
 */
int main(void)
{
	char **ptr;

	_setenv("NEW1", "NEWVALUE", 1);
	_setenv("NEW2", "NEWVALUE", 1);
	_setenv("NEW3", "NEWVALUE", 1);
	_setenv("NEW4", "NEWVALUE", 1);

	_unsetenv("NEW"); /* This caused a NEW* match earlier but fixed */
	_unsetenv("NEW2");
	_unsetenv("NEW3");
	/* _unsetenv("NEW3"); */
	for (ptr = environ; *ptr != NULL; ptr++)
	{
		printf("%s\n", *ptr);
	}
	return (0);
}
