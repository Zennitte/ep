#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdbool.h>

#define TAMANHO 1000

struct Line
{
	int line_number;
	char line_content[TAMANHO];
	struct Line *next;
};

typedef struct Line Line;

struct Node
{
	char *key;
	int count;
	Line *lines;
	struct Node *left;
	struct Node *right;
};

typedef struct Node Node;

Node *createNode(char *key, int line_num, char *content)
{
	Node *newNode = (Node *)malloc(sizeof(Node));
	newNode->key = strdup(key);
	newNode->count = 1;

	newNode->lines = (Line *)malloc(sizeof(Line));
	newNode->lines->line_number = line_num;
	strcpy(newNode->lines->line_content, content);
	newNode->lines->next = NULL;

	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

void toLowerCase(char *str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		str[i] = tolower(str[i]);
	}
}

Node *insert(Node *root, char *key, int line_num, char *content)
{
	if (root == NULL)
	{
		return createNode(key, line_num, content);
	}

	toLowerCase(key);

	int cmp = strcmp(key, root->key);

	if (cmp == 0)
	{
		Line *current = root->lines;
		Line *last = NULL;
		while (current != NULL)
		{
			last = current;
			current = current->next;
		}
		if (last != NULL && strcmp(last->line_content, content) == 0)
		{
			root->count++;
			return root;
		}
		last->next = (Line *)malloc(sizeof(Line));
		last->next->line_number = line_num;
		strcpy(last->next->line_content, content);
		last->next->next = NULL;
		root->count++;
	}
	else if (cmp < 0)
	{
		root->left = insert(root->left, key, line_num, content);
	}
	else
	{
		root->right = insert(root->right, key, line_num, content);
	}

	return root;
}

int countOccurrences(Line *lines)
{
	int count = 0;
	Line *current = lines;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return count;
}

int isPunctuation(char c)
{
	return ispunct(c);
}

void processWord(char *word)
{
	int len = strlen(word);
	int i, j = 0;
	for (i = 0; i < len; i++)
	{
		if (!isPunctuation(word[i]))
		{
			word[j++] = tolower(word[i]);
		}
	}
	word[j] = '\0';
}

void printLines(Line *lines, int start_line)
{
	Line *current = lines;
	while (current != NULL)
	{
		printf("%05d: %s\n", start_line + current->line_number, current->line_content);
		current = current->next;
	}
}

Node *search(Node *root, char *key)
{
	if (root == NULL || strcmp(key, root->key) == 0)
	{
		return root;
	}

	if (strcmp(key, root->key) < 0)
	{
		return search(root->left, key);
	}
	else
	{
		return search(root->right, key);
	}
}

int main(int argc, char **argv)
{
	char *palavra;

	if (argc == 3 && strcmp(argv[2], "arvore") == 0)
	{
		FILE *in = fopen(argv[1], "r");
		if (!in)
		{
			printf("Erro ao abrir o arquivo.\n");
			return 1;
		}

		printf("Tipo de indice: '%s'\n", argv[2]);
		printf("Arquivo texto: '%s'\n", argv[1]);

		struct timeval start_time, end_time;

		gettimeofday(&start_time, NULL);

		char *linha = (char *)malloc((TAMANHO + 1) * sizeof(char));
		int contador_linha = 0;

		Node *root = NULL;

		while (fgets(linha, TAMANHO, in))
		{
			char *quebra_de_linha;
			if ((quebra_de_linha = strrchr(linha, '\n')))
			{
				*quebra_de_linha = 0;
			}

			char *copia_ponteiro_linha = strdup(linha);
			int contador_palavra = 0;

			while ((palavra = strsep(&copia_ponteiro_linha, " ")))
			{
				contador_palavra++;
				processWord(palavra);
				root = insert(root, palavra, contador_linha, linha);
			}

			contador_linha++;
		}

		gettimeofday(&end_time, NULL);

		int total_lines = contador_linha;

		printf("Numero de linhas no arquivo: %d\n", total_lines);
		printf("Tempo para carregar o arquivo e construir o indice: %ld ms\n", ((end_time.tv_sec * 1000000 + end_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec)) / 1000);

		printf("> ");

		char user_command[50];
		struct timeval start_search, end_search;

		while (scanf("%s", user_command))
		{
			if (strcmp(user_command, "fim") == 0)
			{
				break;
			}
			else if (strcmp(user_command, "busca") == 0)
			{
				char search_word[50];
				scanf("%s", search_word);

				gettimeofday(&start_search, NULL);

				Node *found = search(root, search_word);

				gettimeofday(&end_search, NULL);
				if (found != NULL)
				{
					int occurrences = found->count;
					printf("Existem %d ocorrência(s) da palavra '%s' na(s) seguinte(s) linha(s):\n", occurrences, search_word);
					printLines(found->lines, 1); // Começa a contar as linhas a partir de 1
				}
				else
				{
					printf("Palavra '%s' não encontrada.\n", search_word);
				}

				long search_time = (end_search.tv_sec * 1000000 + end_search.tv_usec) - (start_search.tv_sec * 1000000 + start_search.tv_usec);
				printf("Tempo de busca: %ld ms\n", search_time / 1000);
			}
			else
			{
				printf("Opcao invalida!\n");
				__fpurge(stdin);
			}
			printf("> ");
		}
	}
	else
	{
		printf("Uso: %s <nome_arquivo> arvore\n", argv[0]);
		return 1;
	}

	return 0;
}