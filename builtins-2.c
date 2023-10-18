#include "main.h"
/**
 * alias_fn - implements alias built-in
 * @ei: pointer to exec info
 * Return: 0
 */
int alias_fn(exec_info *ei)
{
	int n = 1;
	char *arg, *equal, *val;
	alias_t *x;

	if (*(ei->argv + 1) == NULL)
	{
		print_list(ei->al_list);
		return (0);
	}
	else
	{
		while (*(ei->argv + n) != NULL)
		{
			arg = *(ei->argv + n);
			equal = strchr(arg, '=');
			if (equal == NULL)
			{
				for (x = ei->al_list; x != NULL; x = x->next)
				{
					if (strcmp(arg, x->name) == 0)
					{
						printf("%s='%s'\n", x->name, x->value);
						break;
					}
				}
			}
			else
			{
				arg = _strtok(arg, "=");
				val = &(*(equal + 1));
				new_node(ei, arg, val);
			}
			n++;
		}
	}
	return (0);
}
/**
 * new_node - adds new node
 * @ei: exec info
 * @arg: name of alias
 * @val: value of alias
 */
void new_node(exec_info *ei, char *arg, char *val)
{
	alias_t *x;
	int check = 0;

	for (x = ei->al_list; x != NULL; x = x->next)
	{
		if (strcmp(val, x->name) == 0)
		{
			val = x->value;
			break;
		}
	}
	for (x = ei->al_list; x != NULL; x = x->next)
	{
		if (strcmp(arg, x->name) == 0)
		{
			x->value = strcpy(x->value, val);
			check = 1;
			break;
		}
	}
	if (check == 0)
		add_node(&ei->al_list, arg, val);
}
/**
 * add_node - adds a new node to the end of a list
 * @head: pointer to a pointer to the list
 * @n: str to be name of member of th struct
 * @v: str to be value of member of struct
 * Return: address of new element or NULL
 */
alias_t *add_node(alias_t **head, const char *n, const char *v)
{
	alias_t *p;

	if (*head != NULL && (*head)->next == NULL)
	{
		p = malloc(sizeof(alias_t));
		if (p == NULL)
			return (NULL);
		p->name = strdup(n);
		p->value = strdup(v);
		p->next = NULL;
		(*head)->next = p;
	}
	else if (*head == NULL)
	{
		p = malloc(sizeof(alias_t));
		if (p == NULL)
			return (NULL);
		p->name = strdup(n);
		p->value = strdup(v);
		p->next = NULL;
		*head = p;
	}
	else if ((*head)->next != NULL)
	{
		p = add_node(&(*head)->next, n, v);
	}
	return (p);
}
/**
 * print_list - prints all elements of a linked list
 * @h: pointer to a list structure
 * Return: 0
 */
int print_list(const alias_t *h)
{

	if (h != NULL)
		printf("%s='%s'\n", h->name, h->value);
	if (h->next != NULL)
		print_list(h->next);
	return (0);
}
