#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 10
#define NAME_LEN 50
#define TYPE_LEN 30

typedef struct {
    char name[NAME_LEN];
    char type[TYPE_LEN];
    int quantity;
} Item;

int main(void) {
    Item bag[MAX_ITEMS];
    int count = 0;
    int choice;

    do {
        printf("\nMochila - Gerenciador de Itens\n");
        printf("1. Adicionar item\n");
        printf("2. Remover item por nome\n");
        printf("3. Listar itens\n");
        printf("4. Buscar item por nome\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            choice = 0;
        }

        switch (choice) {
            case 1:
                if (count >= MAX_ITEMS) {
                    printf("Mochila cheia (máx %d itens).\n", MAX_ITEMS);
                } else {
                    printf("Nome: ");
                    scanf(" %49[^\n]", bag[count].name);
                    printf("Tipo: ");
                    scanf(" %29[^\n]", bag[count].type);
                    printf("Quantidade: ");
                    while (scanf("%d", &bag[count].quantity) != 1) {
                        printf("Quantidade inválida. Digite um número: ");
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                    }
                    printf("Item adicionado: %s (%s) x%d\n",
                           bag[count].name, bag[count].type, bag[count].quantity);
                    count++;
                }
                break;

            case 2:
                if (count == 0) {
                    printf("Mochila vazia.\n");
                } else {
                    char target[NAME_LEN];
                    printf("Digite o nome do item a remover: ");
                    scanf(" %49[^\n]", target);
                    int found = -1;
                    for (int i = 0; i < count; i++) {
                        if (strcmp(bag[i].name, target) == 0) {
                            found = i;
                            break;
                        }
                    }
                    if (found == -1) {
                        printf("Item '%s' não encontrado.\n", target);
                    } else {
                        for (int j = found; j < count - 1; j++) {
                            bag[j] = bag[j + 1];
                        }
                        count--;
                        printf("Item '%s' removido.\n", target);
                    }
                }
                break;

            case 3:
                if (count == 0) {
                    printf("Nenhum item cadastrado.\n");
                } else {
                    printf("\n%-20s %-15s %s\n", "Nome", "Tipo", "Quantidade");
                    printf("------------------------------------------------\n");
                    for (int i = 0; i < count; i++) {
                        printf("%-20s %-15s %d\n",
                               bag[i].name, bag[i].type, bag[i].quantity);
                    }
                }
                break;

            case 4: {
                if (count == 0) {
                    printf("Mochila vazia.\n");
                } else {
                    char query[NAME_LEN];
                    printf("Digite o nome do item a buscar: ");
                    scanf(" %49[^\n]", query);
                    int found = -1;
                    for (int i = 0; i < count; i++) {
                        if (strcmp(bag[i].name, query) == 0) {
                            found = i;
                            break;
                        }
                    }
                    if (found == -1) {
                        printf("Item '%s' não encontrado.\n", query);
                    } else {
                        printf("\nItem encontrado:\n");
                        printf("Nome: %s\n", bag[found].name);
                        printf("Tipo: %s\n", bag[found].type);
                        printf("Quantidade: %d\n", bag[found].quantity);
                    }
                }
                break;
            }

            case 5:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }
    } while (choice != 5);

    return 0;
}