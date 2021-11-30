/*


    ALUNO: FAUSTINO FRANCISCO DA SILVA FILHO
    MATRÍCULA:  311441

    Codigo, que faz um jogo da forca!!
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#define MAX_ERROS 5
#define TAM_PALAVRA 30
#define ARQUIVO "palavras.txt"

typedef struct Forca
{
    char palavra[TAM_PALAVRA];
    char palavra_mascarada[TAM_PALAVRA];
    int erros, acertos, tentativas;

} t_forca;

void sorteia_palavra(char *palavra);
void inicializa_forca(t_forca *forca);
int vitoria(t_forca forca);
int derrota(t_forca forca);
char tentativa(t_forca *forca, char tent);
void desenha_forca(t_forca forca);
void fimbom();
void fimruim(char palavra[TAM_PALAVRA]);
void remove_barra_n(char *str);
void salva_jogo(t_forca forca);
void carrega_jogo(t_forca *forca);

/*
  Função main da aplicação
*/
int main()
{
    t_forca forca;
    char tent, res;

    inicializa_forca(&forca);

    do
    {
        // Loop do jogo!
        printf("\n");
        desenha_forca(forca);

        // Le caractere
        printf("\nDigite uma letra: ");
        scanf(" %c", &tent);

        tent = toupper(tent);
        if (tent == '1')
        {
            printf("Salvando o jogo.\n");
            salva_jogo(forca);
        }
        else if (tent == '2')
        {
            printf("Encerrando o jogo atual e carregando o jogo salvo...\n");
            carrega_jogo(&forca);
        }
        else
        {
            // Verifica se acertou
            res = tentativa(&forca, tent);

            system("cls");

            if (res == 'E')
            {
                printf("\nVoce errou!\n");
            }
            if (res == 'R')
            {
                printf("\n Letra igual a anterior\n");
            }
            else if (res == 'A')
            {
                printf("\nVoce acertou!\n");
            }
        }
    } while (!vitoria(forca) && !derrota(forca));

    if (vitoria(forca))
    {
        fimbom();
    }

    if (derrota(forca))
    {
        fimruim(forca.palavra);
    }

    return 0;
}

void sorteia_palavra(char *palavra)
{
    srand(time(NULL));
    int n;
    FILE *open;
    int rand_number;
    open = fopen(ARQUIVO, "r +");
    fscanf(open, "%d", &n);
    rand_number = rand() % n;

    for (int i = 0; i < rand_number; i++)
    {
        fscanf(open, "%s", palavra);
    }

    fclose(open);
}

void inicializa_forca(t_forca *forca)
{

    forca->erros = 0;
    forca->tentativas = 0;
    forca->acertos = 0;

    sorteia_palavra(forca->palavra);

    int max = strlen(forca->palavra);
    for (int i = 0; i < max; i++)
    {
        forca->palavra_mascarada[i] = '_';
    }
    forca->palavra_mascarada[max] = '\0';
}

int vitoria(t_forca forca)
{
    int vit = strlen(forca.palavra_mascarada);
    if (forca.acertos == vit)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int derrota(t_forca forca)
{
    if (forca.erros == MAX_ERROS)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

char tentativa(t_forca *forca, char tent)
{
    forca->tentativas = forca->tentativas + 1;
    int j = 0;
    int igual = 0;
    int max = strlen(forca->palavra);
    for (int i = 0; i < max; i++) // for loop que verifica  se a letra dada se encaixa na string
    {
        if (tent == forca->palavra[i])
        {
            if (forca->palavra_mascarada[i] != tent)
            {
                forca->acertos = forca->acertos + 1;
                j++;
            }
            if (forca->palavra_mascarada[i] == tent)
            {
                igual = igual + 1;
            }
            forca->palavra_mascarada[i] = forca->palavra[i];
        }
    }
    if (j == 0 && igual == 0) // if que verifica se não ouver nenhum acertos e nenhuma palavra
    {
        forca->erros = forca->erros + 1;
        return 'E';
    }
    if (j == 0 && igual > 0)
    {
        // caso seja considerado uma letra repetida como erro considere  forca->erros = forca->erros + 1;
        return 'R';
    }
    else
    {
        return 'A';
    }
}

void salva_jogo(t_forca forca)
{
    FILE *save;
    save = fopen("save.bin", "w+b");
    if (save == NULL)
    {
        printf("\nerro no arquivo\n");
        exit(0);
    }
    fwrite(&forca, sizeof(t_forca), 1, save);
    fclose(save);
}
void carrega_jogo(t_forca *forca)
{
    FILE *readsave;
    readsave = fopen("save.bin", "rb");
    if (readsave == NULL)
    {
        printf("\nerro no arquivo\n");
        exit(0);
    }
    fread(forca, sizeof(t_forca), 1, readsave);
    fclose(readsave);
}

void desenha_forca(t_forca forca)
{
    int k = strlen(forca.palavra_mascarada);
    printf("ACERTOS: [%d]   ERROS: [%d]   TENTATIVAS: [%d]\n\n", forca.acertos, forca.erros, forca.tentativas);

    printf("  _______       \n");
    printf(" |/      |      \n");
    printf(" |      %c%c%c     \n", (forca.erros >= 1 ? '(' : ' '), (forca.erros >= 1 ? '_' : ' '), (forca.erros >= 1 ? ')' : ' '));
    printf(" |      %c%c%c     \n", (forca.erros >= 3 ? '\\' : ' '), (forca.erros >= 2 ? '|' : ' '), (forca.erros >= 3 ? '/' : ' '));
    printf(" |       %c      \n", (forca.erros >= 2 ? '|' : ' '));
    printf(" |      %c %c     \n", (forca.erros >= 4 ? '/' : ' '), (forca.erros >= 4 ? '\\' : ' '));
    printf(" |              \n");
    printf("_|___           \n");
    printf("                \n");
    printf("\n\n");

    for (int i = 0; i < k; i++)
    {
        printf("%c ", forca.palavra_mascarada[i]);
    }
    printf("\n");
}

void fimbom()
{
    system("cls");
    printf("\nParabens, voce ganhou!\n\n");

    printf("       ___________      \n");
    printf("      '._==_==_=_.'     \n");
    printf("      .-\\:      /-.    \n");
    printf("     | (|:.     |) |    \n");
    printf("      '-|:.     |-'     \n");
    printf("        \\::.    /      \n");
    printf("         '::. .'        \n");
    printf("           ) (          \n");
    printf("         _.' '._        \n");
    printf("        '-------'       \n\n");
    system("pause");
}

void fimruim(char palavra[TAM_PALAVRA])
{
    system("cls");
    printf("\nPuxa, voce foi enforcado!\n");
    printf("A palavra era **%s**\n\n", palavra);

    printf("    _______________         \n");
    printf("   /               \\       \n");
    printf("  /                 \\      \n");
    printf("//                   \\/\\  \n");
    printf("\\|   XXXX     XXXX   | /   \n");
    printf(" |   XXXX     XXXX   |/     \n");
    printf(" |   XXX       XXX   |      \n");
    printf(" |                   |      \n");
    printf(" \\__      XXX      __/     \n");
    printf("   |\\     XXX     /|       \n");
    printf("   | |           | |        \n");
    printf("   | I I I I I I I |        \n");
    printf("   |  I I I I I I  |        \n");
    printf("   \\_             _/       \n");
    printf("     \\_         _/         \n");
    printf("       \\_______/           \n");
    system("pause");
}