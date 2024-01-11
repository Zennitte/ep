#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>
#include <stdbool.h>

#define TAMANHO 1000

// Arvore binaria de busca

// Estrutura da linha
struct Line
{
	int line_number;
	char line_content[TAMANHO];
	struct Line *next;
};

typedef struct Line Line;

// Estutura da arvore
struct Node
{
	char *key;
	int count;
	Line *lines;
	struct Node *left;
	struct Node *right;
};

typedef struct Node Node;

// Criação do nó da arvore
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

// Função para converter a string para minusculo
void toLowerCase(char *str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		str[i] = tolower(str[i]);
	}
}

// Função para remover o hífen
void removeHyphen(char *str)
{
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '-')
		{
			str[i] = ' '; // Substitui o hífen por um espaço
		}
	}
}

// Função para inserir um nó na arvore
Node *insert(Node *root, char *key, int line_num, char *content)
{
	if (root == NULL)
	{
		return createNode(key, line_num, content);
	}

	removeHyphen(key);
	toLowerCase(key);

	int cmp = strcmp(key, root->key);

	if (cmp == 0)
	{
		Line *current = root->lines;
		Line *last = NULL;
		while (current != NULL)
		{
			if (strcmp(current->line_content, content) == 0)
			{
				// evitando duplicação de linhas
				return root;
			}
			last = current;
			current = current->next;
		}

		if (last != NULL)
		{
			last->next = (Line *)malloc(sizeof(Line));
			last->next->line_number = line_num;
			strcpy(last->next->line_content, content);
			last->next->next = NULL;
		}
		else
		{
			root->lines = (Line *)malloc(sizeof(Line));
			root->lines->line_number = line_num;
			strcpy(root->lines->line_content, content);
			root->lines->next = NULL;
		}

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

// Função para contar o número de ocorrências da palavre
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

// Verifica se o caractere é uma pontuação
int isPunctuation(char c)
{
	return ispunct(c);
}

// Processa a palavra, removendo sinais de pontuação e hífen(substitui por espaço)
void processWord(char *word, Node *root, int line_num, char *content)
{
	int len = strlen(word);
	int i, j = 0;
	for (i = 0; i < len; i++)
	{
		if (!isPunctuation(word[i]) || word[i] == '-')
		{
			if (word[i] == '-')
			{
				if (j > 0)
				{
					word[j] = '\0'; // Process the part of the word before hyphen
					insert(root, word, line_num, content);
					j = 0;
				}
			}
			else
			{
				word[j++] = tolower(word[i]);
			}
		}
	}

	if (j > 0)
	{
		word[j] = '\0';
		insert(root, word, line_num, content);
	}
}

// Exibe as linhas que contém a palavra
void printLines(Line *lines, int start_line)
{
	Line *current = lines;
	while (current != NULL)
	{
		printf("%05d: %s\n", start_line + current->line_number, current->line_content);
		current = current->next;
	}
}

// Realiza a busca na arvore
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

// Lista

// Estrutura da linha da lista
struct ListLine
{
	int line_number;
	char line_content[TAMANHO];
	struct ListLine *next;
};

typedef struct ListLine ListLine;

// Estrutura do nó da lista
struct ListNode
{
	char *key;
	int count;
	ListLine *lines;
	struct ListNode *next;
};

typedef struct ListNode ListNode;

// Função para criar um nó da lista

ListNode *createListNode(char *key, int line_num, char *content)
{
	ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
	newNode->key = strdup(key);
	newNode->count = 1;

	newNode->lines = (ListLine *)malloc(sizeof(ListLine));
	newNode->lines->line_number = line_num;
	strcpy(newNode->lines->line_content, content);
	newNode->lines->next = NULL;

	newNode->next = NULL;
	return newNode;
}

// Conversão para minúsculo
void toLowerCaseList(char *str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		str[i] = tolower(str[i]);
	}
}

// Remover hífen das palavras
void removeListHyphen(char *str)
{
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '-')
		{
			str[i] = ' '; // Substitui o hífen por um espaço
		}
	}
}

// Função para inserir um nó na lista
ListNode *listInsert(ListNode *head, char *key, int line_num, char *content)
{
	removeListHyphen(key);
	toLowerCaseList(key);

	ListNode *current = head;
	ListNode *last = NULL;

	while (current != NULL && strcmp(key, current->key) != 0)
	{
		last = current;
		current = current->next;
	}

	if (current == NULL)
	{
		ListNode *newNode = createListNode(key, line_num, content);

		if (last != NULL)
		{
			last->next = newNode;
		}
		else
		{
			head = newNode;
		}
	}
	else
	{
		ListLine *lineCurrent = current->lines;
		ListLine *lineLast = NULL;

		while (lineCurrent != NULL)
		{
			if (strcmp(lineCurrent->line_content, content) == 0)
			{
				return head;
			}
			lineLast = lineCurrent;
			lineCurrent = lineCurrent->next;
		}

		if (lineLast != NULL)
		{
			lineLast->next = (ListLine *)malloc(sizeof(ListLine));
			lineLast->next->line_number = line_num;
			strcpy(lineLast->next->line_content, content);
			lineLast->next->next = NULL;
		}
		else
		{
			current->lines = (ListLine *)malloc(sizeof(ListLine));
			current->lines->line_number = line_num;
			strcpy(current->lines->line_content, content);
			current->lines->next = NULL;
		}

		current->count++;
	}

	return head;
}

// Conta a ocorrência de uma palavra na lista
int countListOccurrences(ListLine *lines)
{
	int count = 0;
	ListLine *current = lines;
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return count;
}

// Processa as palavras da lista
void processListWord(char *word, ListNode **head, int line_num, char *content)
{
	int len = strlen(word);
	int i, j = 0;
	for (i = 0; i < len; i++)
	{
		if (!isPunctuation(word[i]) || word[i] == '-')
		{
			if (word[i] == '-')
			{
				if (j > 0)
				{
					word[j] = '\0'; // Process the part of the word before hyphen
					*head = listInsert(*head, word, line_num, content);
					j = 0;
				}
			}
			else
			{
				word[j++] = tolower(word[i]);
			}
		}
	}

	if (j > 0)
	{
		word[j] = '\0';
		*head = listInsert(*head, word, line_num, content);
	}
}

// Imprime as linhas da lista
void printListLines(ListLine *lines, int start_line)
{
	ListLine *current = lines;
	while (current != NULL)
	{
		printf("%05d: %s\n", start_line + current->line_number, current->line_content);
		current = current->next;
	}
}

// Realiza a busca na lista
ListNode *listSearch(ListNode *head, char *key)
{
	ListNode *current = head;

	while (current != NULL && strcmp(key, current->key) != 0)
	{
		current = current->next;
	}

	return current;
}

int main(int argc, char **argv)
{
	char *palavra;

	// Verifico se o número de argumentos é correto e que a estrutura escolhida é arvore
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

		// Verifico a data de inicio da operação
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

				// Processa a palavra, removendo sinais de pontuação e hífen(substitui por espaço)
				processWord(palavra, root, contador_linha, linha);
				// Insere a palavra na arvore
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

		// Inicio o loop para receber os comandos do usuário
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

				// Inicio a contagem do tempo de busca
				gettimeofday(&start_search, NULL);

				// Realiza a busca na arvore
				Node *found = search(root, search_word);

				// Fim da contagem do tempo de busca
				gettimeofday(&end_search, NULL);

				// Verifico se a palavra foi encontrada
				if (found != NULL)
				{
					// Conto o numero de ocorrencias da palavra
					int occurrences = found->count;
					printf("Existem %d ocorrência(s) da palavra '%s' na(s) seguinte(s) linha(s):\n", occurrences, search_word);
					printLines(found->lines, 1); // Começa a contar as linhas a partir de 1
				}
				else
				{
					// Caso a palavra não seja encontrada
					printf("Palavra '%s' não encontrada.\n", search_word);
				}

				// Calculo o tempo de busca
				long search_time = (end_search.tv_sec * 1000000 + end_search.tv_usec) - (start_search.tv_sec * 1000000 + start_search.tv_usec);
				printf("Tempo de busca: %ld ms\n", search_time / 1000);
			}
			else
			{
				// Caso algum comando inválido seja digitado
				printf("Opcao invalida!\n");
				__fpurge(stdin);
			}
			printf("> ");
		}
	}
	else if (argc == 3 && strcmp(argv[2], "lista") == 0)
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

		ListNode *head = NULL;

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
				processListWord(palavra, &head, contador_linha, linha);
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

				ListNode *found = listSearch(head, search_word);

				gettimeofday(&end_search, NULL);
				if (found != NULL)
				{
					int occurrences = found->count;
					printf("Existem %d ocorrência(s) da palavra '%s' na(s) seguinte(s) linha(s):\n", occurrences, search_word);
					printListLines(found->lines, 1); // Começa a contar as linhas a partir de 1
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
		// Caso o número de argumentos seja insuficiente ou a estrutura escolhida seja inválida
		printf("Uso: %s <nome_arquivo> arvore\n", argv[0]);
		return 1;
	}

	return 0;
}