//Joao Pedro Oliveira Ventura dos Santos 261032445

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32 
    #define CLEAR "cls"
#else 
    #define CLEAR "clear" 
#endif

typedef struct {
    char nickname[20];
    int pontos;
} dados;

dados jogador; 
char assento[2];
char filaEspera[4];
char matriz[11][11];
int ganhou;
FILE* fdt;
FILE* fdb;


void pausa() {
    while (getchar() != '\n');
    getchar();
}

void reiniciarRanking() {
    fdb = fopen("file.bin", "wb");
    fclose(fdb);
}

void inserirRanking() {
    fdb = fopen("file.bin", "ab");
    fwrite(&jogador, sizeof(dados), 1, fdb);
    fclose(fdb);
}

void ordenarRanking() {
    fdb = fopen("file.bin", "r+b");
    dados jogadores[20];
    int x = 0;
    while (fread(&jogador, sizeof(dados), 1, fdb) != 0) {
        jogadores[x] = jogador;
        x++;
    }

    int i, j, ok = 0;
    for (i = 0; i < x && !ok; i++) {
        ok = 1;
        for (j = 0; j < x-1-i; j++) {
            if (jogadores[j].pontos < jogadores[j+1].pontos) {
                ok = 0;
                int temp = jogadores[j].pontos;
                jogadores[j].pontos = jogadores[j+1].pontos;
                jogadores[j+1].pontos = temp;
                char auxiliar[30];
                strcpy(auxiliar, jogadores[j].nickname);
                strcpy(jogadores[j].nickname, jogadores[j+1].nickname);
                strcpy(jogadores[j+1].nickname, auxiliar);
            }
        }
    }

    rewind(fdb);
    fwrite(jogadores, sizeof(dados), x, fdb);
    fclose(fdb);
}

void mostrarRanking() {
    fdb = fopen("file.bin", "rb");
    while (fread(&jogador, sizeof(dados), 1, fdb) != 0) {
        printf("%d %s\n", jogador.pontos, jogador.nickname);
    }
    printf("\n"); 
    fclose(fdb);
}

int sairJogar() {
    char res;
    scanf(" %c", &res);
    while (res != 'S' && res != 'N') {
        scanf(" %c", &res);
    }
    if (res == 'N') {
        return 1;
    }
    return 0;
}

int assentoVazio() {
    if (assento[0] == ' ') {
        return 0;
    }
    if (assento[1] == ' ') {
        return 1;
    }
    return 2;
}

int filaEsperaVazia() {
    if (filaEspera[0] == '_') {
        return 0;
    }
    if (filaEspera[1] == '_') {
        return 1;
    }
    if (filaEspera[2] == '_') {
        return 2;
    }
    if (filaEspera[3] == '_') {
        return 3;
    }
    return 4;
}

void filaNaoEspera(char motorista[], int* qualMotor) {
    for (int i = 0; i < 4; i++) {
        if (filaEspera[i] == motorista[*qualMotor]) {
            if (assentoVazio() == 2) {
                jogador.pontos += 15;
                *qualMotor += 1;
                assento[0] = ' ';
                assento[1] = ' ';
                filaEspera[i] = '_';
            } else {
                assento[assentoVazio()] = filaEspera[i];
                filaEspera[i] = '_';
            }
        }
    }
    return;
}

int bloqueado(int x, int y) {
    if (matriz[x+1][y] == ' ' || matriz[x-1][y] == ' ' || matriz[x][y+1] == ' ' || matriz[x][y-1] == ' ' || x == 0) {
        return 0;
    } 
    return 1;
}

int jogabilidade(char motorista[], int* qualMotor, int m) {
    int x, y;
    printf("Infome a linha e a coluna para embarcar no onibus (1 1 ou 2 3 ou ...): ");
    scanf("%d %d", &x, &y);
    x--;
    y--;

    if (x < 0 || x > m-1 || y < 0 || y > 9) {
        printf("Posicao Inexistente! Tecle <enter> para continuar!");
        pausa();
        return 0;
    } else if (matriz[x][y] == '_') {
        printf("Posicao Invalida! Tecle <enter> para continuar!");
        pausa();
        return 0;
    } else if (matriz[x][y] == ' ') {
        printf("Elemento ja foi movido! Tecle <enter> para continuar!");
        pausa();
        return 0;
    } else if (bloqueado(x,y)) {
        printf("Elemento bloqueado! Tecle <enter> para continuar!");
        pausa();
        return 0;
    } else {
        if (matriz[x][y] == motorista[*qualMotor]) {
            if (assentoVazio() == 2) {
                jogador.pontos += 15;
                *qualMotor += 1;
                assento[0] = ' ';
                assento[1] = ' ';
                matriz[x][y] = ' ';
                filaNaoEspera(motorista, qualMotor);
                if (*qualMotor == strlen(motorista)) {
                    jogador.pontos += 100;
                    return 1;
                }
                return 0;
            } else {
                assento[assentoVazio()] = matriz[x][y];
                matriz[x][y] = ' ';
                return 0;
            }
        } else {
            if (filaEsperaVazia() == 4) {
                return -1;
            } else {
                filaEspera[filaEsperaVazia()] = matriz[x][y];
                matriz[x][y] = ' ';
                return 0;
            }
        }
    }
}

void desenhaGanhou() {
    system(CLEAR);
    printf("*** JOGO <Bus Jam made in China> ***        Pontuacao: %d\n\n", jogador.pontos);
    printf("****************************************************\n");
    printf("**           PARABENS!!! VOCE VENCEU!!!           **\n");
    printf("**                                                **\n");
    printf("**                  TECLE <enter>                 **\n");
    printf("****************************************************\n");
}

void desenhaPassou() {
    system(CLEAR);
    printf("*** JOGO <Bus Jam made in China> ***        Pontuacao: %d\n\n", jogador.pontos);
    printf("****************************************************\n");
    printf("**        MUITO BEM, VOCE FINALIZOU A FASE!       **\n");
    printf("**                                                **\n");
    printf("**       Continuar para a proxima fase? (S/N)     **\n");
    printf("****************************************************\n");
}

void desenhaPerdeu() {
    printf("\n");
    printf("****************************************************\n");
    printf("**    COM ESTE MOVIMENTO, LOTOU A FILA DE ESPERA  **\n");
    printf("**                  VOCE PERDEU :(                **\n");
    printf("**                                                **\n");
    printf("**                  TECLE <enter>                 **\n");
    printf("****************************************************\n");
}

void desenhaFase(int m, char motorista[], int qualMotor) {
    system(CLEAR);
    printf("*** JOGO <Bus Jam made in China> ***        Pontuacao: %d\n\n", jogador.pontos);
    printf("    +----o--------o--+\n");
    printf("    |  -    -    -   |-+\n");
    printf("    | |%c   |%c   |    |%c|\n", assento[0], assento[1], motorista[qualMotor]);
    printf("    |  -    -    -   |-+\n");
    printf("    +----o---==---o--+\n\n");
    printf("        %c %c %c %c _\n\n", filaEspera[0], filaEspera[1], filaEspera[2], filaEspera[3]);
    for (int i = 0; i < m; i++) {
        printf("        %d%s\n", i+1, matriz[i]);
    }
    printf("         12345678910\n\n");
}

void jogar() {
    system(CLEAR);
    jogador.pontos = 0;
    fdt = fopen("entrada.txt", "r+");

    int ganhou;
    int onibus, m, n, num;
    char final;
    do {
        fscanf(fdt, "%d %d %d %d", &onibus, &m, &n, &num);
        char motorista[onibus + 1];
        int qualMotor = 0;

        for (int i = 0; i < m; i++) {
            fscanf(fdt, "%10s", matriz[i]);
        }
        fscanf(fdt, "%s", motorista);
        fscanf(fdt, " %c", &final);

        ganhou = 0;
        for (int i = 0; i < 4; i++) {
            filaEspera[i] = '_';
        }
        assento[0] = ' ';
        assento[1] = ' ';
        
        while (ganhou == 0) {
            desenhaFase(m, motorista, qualMotor);
            ganhou = jogabilidade(motorista, &qualMotor, m);
        }
        if (ganhou == 1) {
            desenhaPassou();
            if (sairJogar()) {
                break;
            }
        } else {
            desenhaPerdeu();
            pausa();
            break;
        }
    } while (final != 'U');
    fclose(fdt);
    if (final == 'U' && ganhou == 1) {
        desenhaGanhou();
        pausa();
    }
    inserirRanking();
    return;
}

void configuracoes() {
    while (1) {
        system(CLEAR);
        printf("*** CONFIGURACOES ***\n\n");
        printf("1 - Zerar ranking\n");
        printf("2 - Modo dificuldade (em desenvolvimento)\n");
        printf("3 - Editor de fases (em desenvolvimento)\n");
        printf("4 - Voltar ao menu principal\n\n");
        printf("Digite a opcao desejada (1 ou 2 ou ...): ");
        int menu;
        if (scanf("%d", &menu) == 0) {
            getchar();
        } else if (menu == 1) {
            system(CLEAR);
            printf("*** JOGO <Bus Jam made in China> ***\n\n");
            printf("Confirma reinicializar o ranking? (S/N) ");
            char reinicia;
            scanf(" %c", &reinicia);
            while (reinicia != 'S' && reinicia != 'N') {
                scanf(" %c", &reinicia);
            }
            if (reinicia == 'S') {
                reiniciarRanking();
                printf("Ranking zerado! Tecle <enter> para continuar");
                pausa();
            }
        } else if (menu == 2) {
            system(CLEAR);
            printf("*** JOGO <Bus Jam made in China> ***\n\n");
            pausa();
        } else if (menu == 3) {
            system(CLEAR);
            printf("*** JOGO <Bus Jam made in China> ***\n\n");
            pausa();
        } else if (menu == 4) {
            return;
        }
    }
}

void instrucoes() {
    system(CLEAR);
    printf("INSTRUCOES SOBRE O JOGO <Bus Jam mande in China>:\n\n");
    printf("    O seu objetivo nesse jogo eh ajudar um grupo de pessoas ah entrar em seus respectivos onibus, cada onibus tem seu tipo(representado por um motorista), e apenas pessoas desse tipo podem entrar nesse onibus.\n\n");
    printf("    Caso voce tente colocar alguem de um tipo diferente no onibus, ela vai para a fila de espera, ou seja, ela fica esperando o seu proprio onibus e, quando ele chegar, ela vai automaticamente entrar nele.\n\n");
    printf("    Para ganhar, voce deve encher todos os onibus (cada um possui tres lugares), sendo que um novo onibus soh chega quando o anterior for completado. Entretanto, se voce encher a fila de espera (que tem 5 lugares), voce perde.\n\n");
    printf("    Completar um onibus te dah 15 pontos, e completar uma fase 100 pontos, voce pode comparar a pontuacao de varios jogadores pelo ranking, mas lembre de fechar e abrir o programa de novo para mudar de nickname.\n\n");
    printf("    Para mover alguem, voce deve dizer as coordenadas dele (linha coluna). A pessoa soh pode ir pro onibus se ela tiver um caminho em direcao ao onibus, ou seja, espacos vazios na frente ou do lado dela.\n\n");
    printf("    Obs: o simbolo '_' representa uma parede (que nao se move), e o simbolo ' ' representa um espaco vazio (ou seja, por onde alguem poderia andar).\n\n");
    printf("Tecle <enter> para visualizar um exemplo de fase...\n");
    pausa();
    system(CLEAR);
    printf("*** JOGO <Bus Jam made in China> ***        Pontuacao: 130\n\n");
    printf("    +----o--------o--+\n");
    printf("    |  -    -    -   |-+\n");
    printf("    | |X   |    |    |X|\n");
    printf("    |  -    -    -   |-+\n");
    printf("    +----o---==---o--+\n\n");
    printf("        Y Z Y _ _\n\n");
    printf("        1___X _____\n");
    printf("        2___  Z____\n");
    printf("        3__YX _____\n");
    printf("        4____Z_____\n");
    printf("         12345678910\n\n");
    printf("Essa fase tem, respectivamente, um onibus tipo X com um passageiro, uma lista de espera com tres pessoas e uma matriz do grupo (os numeros representam as coordenadas de cada elemento).\n\n");
    printf("Perceba que: o elemento '1 0' nao existe, o '1 2' eh uma parede, o '1 4' poderia entrar no onibus, o '2 4' tah vazio, '2 6' iria para a fila de espera e o '3 3' tah bloqueado e nao consegue andar\n\n");
    printf("Tecle <enter> para prosseguir...");
    pausa();
    return;
}

void ranking() {
    system(CLEAR);
    printf("*** JOGO <Bus Jam made in China> ***\n\n");
    printf("RANKING:\n\n");
    ordenarRanking();
    mostrarRanking();
    printf("Tecle <enter> para continuar\n");
    pausa();
    return;
}

void menu() {

    while (1) {
        system(CLEAR);
        printf("*** JOGO <Bus Jam made in China> ***\n\n");
        printf("1 - Jogar\n");
        printf("2 - Configuracoes\n");
        printf("3 - Instrucoes\n");
        printf("4 - Ranking\n");
        printf("5 - Sair\n\n");
        printf("Digite a opcao desejada (1 ou 2 ou ...): ");

        int menu;
        if (scanf("%d", &menu) == 0) {
            getchar();
        } else if (menu == 1) {
            jogar();
        } else if (menu == 2) {
            configuracoes();
        } else if (menu == 3) {
            instrucoes();
        } else if (menu == 4) {
            ranking();
        } else if (menu == 5) {
            return;
        }
    }
}

int main() {

    system(CLEAR);
    printf("Bem vindo(a) ao jogo <Bus Jam made in China>!!!\n\n");
    printf("Informe seu nickname: ");
    scanf("%s", jogador.nickname);
    menu();
    return 0;
}