#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_CARTAS 5

typedef struct {
    char nome[30];
    int forca;
    int velocidade;
    int inteligencia;
} Carta;

// Limpa o buffer de entrada
void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void mostrarCarta(Carta c) {
    printf("Nome: %s\n", c.nome);
    printf("Força: %d\n", c.forca);
    printf("Velocidade: %d\n", c.velocidade);
    printf("Inteligência: %d\n", c.inteligencia);
}

// Mostra só o nome da carta (usada para suspense)
void mostrarCartaOculta(Carta c) {
    printf("Nome: %s\n", c.nome);
    printf("Atributos ocultos até a escolha!\n");
}

// Escolha do usuário
int escolherAtributo() {
    int escolha;
    do {
        printf("Escolha um atributo para competir:\n");
        printf("1 - Força\n2 - Velocidade\n3 - Inteligência\n");
        printf("Digite sua escolha: ");
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada inválida! Digite um número de 1 a 3.\n");
            limpaBuffer();
            escolha = 0;
        }
    } while (escolha < 1 || escolha > 3);
    limpaBuffer();
    return escolha;
}

// Escolha automática (computador) — retorna o maior atributo
int escolherAtributoComputador(Carta c) {
    if (c.forca >= c.velocidade && c.forca >= c.inteligencia)
        return 1; // Força
    else if (c.velocidade >= c.forca && c.velocidade >= c.inteligencia)
        return 2; // Velocidade
    else
        return 3; // Inteligência
}

int compararCartas(Carta c1, Carta c2, int atributo) {
    int valor1, valor2;
    switch (atributo) {
        case 1: valor1 = c1.forca; valor2 = c2.forca; break;
        case 2: valor1 = c1.velocidade; valor2 = c2.velocidade; break;
        case 3: valor1 = c1.inteligencia; valor2 = c2.inteligencia; break;
        default: return 0;
    }
    if (valor1 > valor2)
        return 1;
    else if (valor2 > valor1)
        return 2;
    else
        return 0;
}

void embaralhar(Carta baralho[], int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Carta temp = baralho[i];
        baralho[i] = baralho[j];
        baralho[j] = temp;
    }
}

int main() {
    srand(time(NULL));

    // Mensagem de boas-vindas e instruções
    printf("=== Bem-vindo ao Super Trunfo de Cartas Fantásticas! ===\n");
    printf("Você jogará contra o computador. Em cada rodada, escolha um atributo da sua carta para competir.\n");
    printf("Quem tiver o maior valor no atributo escolhido, ganha a rodada!\n\n");

    char nomeJogador[50];
    printf("Digite seu nome: ");
    fgets(nomeJogador, sizeof(nomeJogador), stdin);
    // Remove o \n do final do nome, se houver
    nomeJogador[strcspn(nomeJogador, "\n")] = '\0';

    Carta baralho[TOTAL_CARTAS] = {
        {"Dragão", 90, 60, 70},
        {"Fênix", 75, 85, 80},
        {"Troll", 95, 40, 30},
        {"Elfo", 60, 90, 85},
        {"Mago", 50, 70, 95}
    };

    embaralhar(baralho, TOTAL_CARTAS);

    int cartas_jogador1 = TOTAL_CARTAS / 2 + (TOTAL_CARTAS % 2);
    int cartas_jogador2 = TOTAL_CARTAS / 2;

    Carta jogador1[cartas_jogador1];
    Carta jogador2[cartas_jogador2];

    for (int i = 0; i < cartas_jogador1; i++) {
        jogador1[i] = baralho[i];
    }
    for (int i = 0; i < cartas_jogador2; i++) {
        jogador2[i] = baralho[i + cartas_jogador1];
    }

    int pontos1 = 0, pontos2 = 0;
    int rodadas = cartas_jogador2;
    int empates = 0;

    for (int rodada = 0; rodada < rodadas; rodada++) {
        printf("\n=== Rodada %d ===\n", rodada + 1);
        printf("%s, sua carta:\n", nomeJogador);
        mostrarCarta(jogador1[rodada]);
        printf("\nCarta do oponente:\n");
        mostrarCartaOculta(jogador2[rodada]);

        int atributo;
        if (rodada % 2 == 0) {
            printf("\n%s escolhe o atributo!\n", nomeJogador);
            atributo = escolherAtributo();
        } else {
            printf("\nO computador escolhe o atributo!\n");
            atributo = escolherAtributoComputador(jogador2[rodada]);
            switch (atributo) {
                case 1: printf("Computador escolheu: Força\n"); break;
                case 2: printf("Computador escolheu: Velocidade\n"); break;
                case 3: printf("Computador escolheu: Inteligência\n"); break;
            }
        }

        printf("\nAgora revelando a carta do oponente:\n");
        mostrarCarta(jogador2[rodada]);

        int resultado = compararCartas(jogador1[rodada], jogador2[rodada], atributo);

        if (resultado == 1) {
            printf("Você venceu esta rodada!\n");
            pontos1++;
        } else if (resultado == 2) {
            printf("Você perdeu esta rodada.\n");
            pontos2++;
        } else {
            printf("Empate!\n");
            empates++;
        }

        printf("Placar parcial: %s %d x %d Computador\n", nomeJogador, pontos1, pontos2);
    }

    if (cartas_jogador1 > cartas_jogador2) {
        printf("\n=== Rodada Extra ===\n");
        printf("%s, sua carta:\n", nomeJogador);
        mostrarCarta(jogador1[cartas_jogador1 - 1]);
        printf("O computador não possui carta para esta rodada.\n");
        printf("Você ganha automaticamente esta rodada!\n");
        pontos1++;
        printf("Placar parcial: %s %d x %d Computador\n", nomeJogador, pontos1, pontos2);
    }

    printf("\n=== Resultado Final ===\n");
    printf("%s: %d pontos\n", nomeJogador, pontos1);
    printf("Computador: %d pontos\n", pontos2);

    if (pontos1 > pontos2)
        printf("Parabéns %s! Você venceu o jogo!\n", nomeJogador);
    else if (pontos2 > pontos1)
        printf("Que pena %s! Você perdeu o jogo.\n", nomeJogador);
    else {
        printf("O jogo terminou empatado.\n");
        if (empates == rodadas) {
            printf("Todos empataram! Sorteando um vencedor...\n");
            int sorteio = rand() % 2;
            if (sorteio == 0)
                printf("A sorte sorriu para você, %s! Você venceu!\n", nomeJogador);
            else
                printf("O computador venceu no sorteio!\n");
        }
    }

    printf("\nObrigado por jogar!\n");
    return 0;
}
