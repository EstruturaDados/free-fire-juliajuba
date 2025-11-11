#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ITEMS 10
#define NAME_LEN 50
#define TYPE_LEN 30

typedef struct {
    char name[NAME_LEN];
    char type[TYPE_LEN];
    int quantity;
    int priority; // 1 a 5
} Item;

typedef enum {
    SORT_NAME = 1,
    SORT_TYPE = 2,
    SORT_PRIORITY = 3
} SortCriterion;

void print_item(const Item *it) {
    printf("Nome: %s\n", it->name);
    printf("Tipo: %s\n", it->type);
    printf("Quantidade: %d\n", it->quantity);
    printf("Prioridade: %d\n", it->priority);
}

int compare_items(const Item *a, const Item *b, SortCriterion crit, long *cmp_count) {
    if (cmp_count) (*cmp_count)++;
    if (crit == SORT_NAME) {
        int r = strcmp(a->name, b->name);
        if (r < 0) return -1;
        if (r > 0) return 1;
        return 0;
    } else if (crit == SORT_TYPE) {
        int r = strcmp(a->type, b->type);
        if (r < 0) return -1;
        if (r > 0) return 1;
        return 0;
    } else { // SORT_PRIORITY
        if (a->priority < b->priority) return -1;
        if (a->priority > b->priority) return 1;
        return 0;
    }
}

void insertion_sort(Item bag[], int count, SortCriterion crit, long *cmp_count) {
    if (cmp_count) *cmp_count = 0;
    for (int i = 1; i < count; i++) {
        Item key = bag[i];
        int j = i - 1;
        while (j >= 0 && compare_items(&bag[j], &key, crit, cmp_count) > 0) {
            bag[j + 1] = bag[j];
            j--;
        }
        bag[j + 1] = key;
    }
}

int binary_search_by_name(const Item bag[], int count, const char *target) {
    int lo = 0, hi = count - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int cmp = strcmp(bag[mid].name, target);
        if (cmp == 0) return mid;
        if (cmp < 0) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

int main(void) {
    Item bag[MAX_ITEMS];
    int count = 0;
    int choice;
    bool sorted_by_name = false;

    do {
        printf("\nMochila - Gerenciador de Itens (Nível Aventureiro+)\n");
        printf("1. Adicionar item\n");
        printf("2. Remover item por nome\n");
        printf("3. Listar itens\n");
        printf("4. Ordenar mochila\n");
        printf("5. Buscar item por nome (linear)\n");
        printf("6. Buscar item por nome (binária) - exige ordenação por nome\n");
        printf("7. Sair\n");
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
                    printf("Prioridade (1-5): ");
                    while (scanf("%d", &bag[count].priority) != 1 || bag[count].priority < 1 || bag[count].priority > 5) {
                        printf("Prioridade inválida. Digite um número entre 1 e 5: ");
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                    }
                    printf("Item adicionado: %s (%s) x%d [P%d]\n",
                           bag[count].name, bag[count].type, bag[count].quantity, bag[count].priority);
                    count++;
                    sorted_by_name = false; // alteração no conteúdo invalida ordenação por nome
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
                        sorted_by_name = false;
                    }
                }
                break;

            case 3:
                if (count == 0) {
                    printf("Nenhum item cadastrado.\n");
                } else {
                    printf("\n%-20s %-15s %-10s %s\n", "Nome", "Tipo", "Quantidade", "Prioridade");
                    printf("---------------------------------------------------------------\n");
                    for (int i = 0; i < count; i++) {
                        printf("%-20s %-15s %-10d %d\n",
                               bag[i].name, bag[i].type, bag[i].quantity, bag[i].priority);
                    }
                }
                break;

            case 4: {
                if (count == 0) {
                    printf("Nada a ordenar: mochila vazia.\n");
                } else {
                    printf("Escolha critério de ordenação:\n");
                    printf("1. Nome\n");
                    printf("2. Tipo\n");
                    printf("3. Prioridade\n");
                    printf("Critério: ");
                    int crit_choice;
                    if (scanf("%d", &crit_choice) != 1) {
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                        crit_choice = 0;
                    }
                    SortCriterion crit;
                    if (crit_choice == 1) crit = SORT_NAME;
                    else if (crit_choice == 2) crit = SORT_TYPE;
                    else if (crit_choice == 3) crit = SORT_PRIORITY;
                    else {
                        printf("Critério inválido.\n");
                        break;
                    }

                    long comparisons = 0;
                    insertion_sort(bag, count, crit, &comparisons);
                    printf("Mochila ordenada por %s. Comparações na ordenação: %ld\n",
                           crit == SORT_NAME ? "nome" : (crit == SORT_TYPE ? "tipo" : "prioridade"),
                           comparisons);
                    sorted_by_name = (crit == SORT_NAME);
                }
                break;
            }

            case 5: {
                if (count == 0) {
                    printf("Mochila vazia.\n");
                } else {
                    char query[NAME_LEN];
                    printf("Digite o nome do item a buscar (linear): ");
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
                        print_item(&bag[found]);
                    }
                }
                break;
            }

            case 6: {
                if (count == 0) {
                    printf("Mochila vazia.\n");
                } else if (!sorted_by_name) {
                    printf("Erro: a busca binária exige que a lista esteja ordenada por nome.\n");
                    printf("Ordene a mochila por nome antes de usar a busca binária.\n");
                } else {
                    char query[NAME_LEN];
                    printf("Digite o nome do item a buscar (binária): ");
                    scanf(" %49[^\n]", query);
                    int idx = binary_search_by_name(bag, count, query);
                    if (idx == -1) {
                        printf("Item '%s' não encontrado.\n", query);
                    } else {
                        printf("\nItem encontrado (pos %d):\n", idx);
                        print_item(&bag[idx]);
                    }
                }
                break;
            }

            case 7:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }
    } while (choice != 7);

    return 0;
}