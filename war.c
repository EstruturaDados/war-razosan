#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 5

typedef struct {
    char nome[50];
    char cor[20];
    int tropas;
} Territorio;

typedef struct {
    int tipo; // 1 = destruir cor | 2 = conquistar territórios
    char corAlvo[20];
    int objetivo;
} Missao;

// ---------------- FUNÇÕES ----------------

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarTerritorios(Territorio *mapa) {
    for (int i = 0; i < MAX; i++) {
        printf("\nTerritório %d\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, 50, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(mapa[i].cor, 20, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();
    }
}

void mostrarMapa(const Territorio *mapa) {
    printf("\n=== MAPA ===\n");
    for (int i = 0; i < MAX; i++) {
        printf("%d - %s | Cor: %s | Tropas: %d\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
}

void atacar(Territorio *mapa, int atk, int def) {
    if (atk == def) {
        printf("Não pode atacar o mesmo território!\n");
        return;
    }

    if (mapa[atk].tropas <= 1) {
        printf("Tropas insuficientes para atacar!\n");
        return;
    }

    int dadoAtk = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;

    printf("\n%s (Atk) tirou %d | %s (Def) tirou %d\n",
           mapa[atk].nome, dadoAtk,
           mapa[def].nome, dadoDef);

    if (dadoAtk >= dadoDef) {
        mapa[def].tropas--;
        printf("Defensor perdeu 1 tropa!\n");

        if (mapa[def].tropas <= 0) {
            printf("Território conquistado!\n");

            strcpy(mapa[def].cor, mapa[atk].cor);
            mapa[def].tropas = 1;
            mapa[atk].tropas--;
        }
    } else {
        mapa[atk].tropas--;
        printf("Atacante perdeu 1 tropa!\n");
    }
}

Missao gerarMissao() {
    Missao m;
    m.tipo = rand() % 2 + 1;

    if (m.tipo == 1) {
        strcpy(m.corAlvo, "Verde");
    } else {
        m.objetivo = 3;
    }

    return m;
}

void mostrarMissao(Missao m) {
    printf("\n=== MISSÃO ===\n");

    if (m.tipo == 1) {
        printf("Destruir o exército da cor: %s\n", m.corAlvo);
    } else {
        printf("Conquistar %d territórios\n", m.objetivo);
    }
}

int verificarMissao(const Territorio *mapa, Missao m) {
    if (m.tipo == 1) {
        for (int i = 0; i < MAX; i++) {
            if (strcmp(mapa[i].cor, m.corAlvo) == 0) {
                return 0;
            }
        }
        return 1;
    }

    if (m.tipo == 2) {
        int count = 0;
        for (int i = 0; i < MAX; i++) {
            if (mapa[i].tropas > 0) count++;
        }
        return count >= m.objetivo;
    }

    return 0;
}

// ---------------- MAIN ----------------

int main() {
    srand(time(NULL));

    Territorio *mapa = calloc(MAX, sizeof(Territorio));

    if (mapa == NULL) {
        printf("Erro de memória!\n");
        return 1;
    }

    printf("=== CADASTRO DE TERRITÓRIOS ===\n");
    cadastrarTerritorios(mapa);

    Missao missao = gerarMissao();

    int opcao;

    do {
        printf("\n==== MENU ====\n");
        printf("1 - Atacar\n");
        printf("2 - Ver mapa\n");
        printf("3 - Ver missão\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        limparBuffer();

        switch (opcao) {
            case 1: {
                int atk, def;

                mostrarMapa(mapa);

                printf("Escolha atacante (1-5): ");
                scanf("%d", &atk);

                printf("Escolha defensor (1-5): ");
                scanf("%d", &def);

                limparBuffer();

                atk--; def--;

                if (atk < 0 || atk >= MAX || def < 0 || def >= MAX) {
                    printf("Escolha inválida!\n");
                    break;
                }

                atacar(mapa, atk, def);

                if (verificarMissao(mapa, missao)) {
                    printf("\n🎉 MISSÃO CUMPRIDA! VOCÊ VENCEU!\n");
                    opcao = 0;
                }

                break;
            }

            case 2:
                mostrarMapa(mapa);
                break;

            case 3:
                mostrarMissao(missao);
                break;

            case 0:
                printf("Saindo do jogo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    free(mapa);
    return 0;
}