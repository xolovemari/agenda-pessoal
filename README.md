#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ainda precisa adicionar os arquivos para cada usuario, no comeco do programa deve ser perguntado o nome do usuario,
// se ja houver um arquivo com o mesmo nome, carregar a lista desse arquivo, se nao, criar outro
// o programa precisa usar busca binaria (ou outra abordada)

// Estrutura para representar um compromisso
typedef struct
{
    char descricao[100];
    int dia, mes, ano, hora, minuto;
    int prioridade; // 1 - Alta, 2 - Média, 3 - Baixa
    int concluido;  // 0 - Não concluído, 1 - Concluído
} Compromisso;

// Nó da lista ligada
typedef struct No
{
    Compromisso compromisso;
    struct No *proximo;
} No;

void clrscr() // limpa a tela
{
    system("@cls||clear");
}

// Função para adicionar um compromisso
void adicionarCompromisso(No **lista)
{
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL)
    {
        printf("Erro ao alocar memória!\n");
        return;
    }

    printf("|       Digite a descricao do compromisso: ");
    scanf(" %[^\n]", novo->compromisso.descricao);

    printf("|       Digite a data (dd/mm/aaaa): ");
    if (scanf("%2d/%2d/%4d", &novo->compromisso.dia, &novo->compromisso.mes, &novo->compromisso.ano) != 3 || novo->compromisso.dia < 0 || novo->compromisso.dia > 31 || novo->compromisso.mes < 0 || novo->compromisso.mes > 12 || novo->compromisso.ano < 1)
    {
        puts("Entrada invalida");
        free(novo);
        return;
    }

    printf("|       Digite o horario (hh:mm): ");
    if (scanf("%2d:%2d", &novo->compromisso.hora, &novo->compromisso.minuto) != 2 || novo->compromisso.hora > 24 || novo->compromisso.hora < 0 || novo->compromisso.minuto > 59)
    {
        puts("Entrada invalida");
        free(novo);
        return;
    }

    printf("|       Digite a prioridade (1 - Alta, 2 - Média, 3 - Baixa): ");
    if (scanf("%d", &novo->compromisso.prioridade) != 1 || novo->compromisso.prioridade < 1 || novo->compromisso.prioridade > 3)
    {
        puts("Entrada invalida");
        free(novo);
        return;
    }

    No *atual = *lista;
    while (atual != NULL)
    {
        if (strcmp(atual->compromisso.descricao, novo->compromisso.descricao) == 0 &&
            atual->compromisso.dia == novo->compromisso.dia &&
            atual->compromisso.mes == novo->compromisso.mes &&
            atual->compromisso.ano == novo->compromisso.ano &&
            atual->compromisso.hora == novo->compromisso.hora &&
            atual->compromisso.minuto == novo->compromisso.minuto)
        {
            printf("|       Compromisso ja existe.\n");
            free(novo);
            return;
        }
        atual = atual->proximo;
    }

    novo->compromisso.concluido = 0; // Por padrão, não concluído
    novo->proximo = NULL;

    // Inserir no final da lista
    if (*lista == NULL)
    {
        *lista = novo;
    }
    else
    {
        No *temp = *lista;
        while (temp->proximo != NULL)
        {
            temp = temp->proximo;
        }
        temp->proximo = novo;
    }

    printf("|       Compromisso adicionado com sucesso!\n");
}

// Função para exibir compromissos
void exibirCompromissos(No *lista)
{
    if (lista == NULL)
    {
        printf("Nenhum compromisso encontrado.\n");
        return;
    }

    printf("============Lista de compromissos:=============\n\n");
    while (lista != NULL)
    {
        printf("    Descricao: %s\n", lista->compromisso.descricao);
        printf("    Data: %02d/%02d/%04d\n", lista->compromisso.dia, lista->compromisso.mes, lista->compromisso.ano);
        printf("    Hora: %02d:%02d\n", lista->compromisso.hora, lista->compromisso.minuto);
        printf("    Prioridade: %d\n", lista->compromisso.prioridade);
        printf("    Concluido: %s\n", lista->compromisso.concluido ? "Sim" : "Nao");
        printf("----------------------\n");
        lista = lista->proximo;
    }
    printf("===============================================\n\n");
}

// Função para buscar compromissos por data
void buscarCompromissosPorData(No *lista, int dia, int mes, int ano)
{
    int encontrado = 0;
    while (lista != NULL)
    {
        if (lista->compromisso.dia == dia &&
            lista->compromisso.mes == mes &&
            lista->compromisso.ano == ano)
        {
            printf("Descricao: %s\n", lista->compromisso.descricao);
            printf("Hora: %02d:%02d\n", lista->compromisso.hora, lista->compromisso.minuto);
            printf("Prioridade: %d\n", lista->compromisso.prioridade);
            printf("Concluído: %s\n", lista->compromisso.concluido ? "Sim" : "Não");
            printf("----------------------\n");
            encontrado = 1;
        }
        lista = lista->proximo;
    }
    if (!encontrado)
    {
        printf("Nenhum compromisso encontrado na data %02d/%02d/%04d.\n", dia, mes, ano);
    }
}

// Função para excluir compromisso
void excluirCompromisso(No **lista, char descricao[])
{
    if (*lista == NULL)
    {
        printf("Nenhum compromisso encontrado.\n");
        return;
    }

    No *atual = *lista, *anterior = NULL;

    while (atual != NULL)
    {
        if (strcmp(atual->compromisso.descricao, descricao) == 0)
        {
            if (anterior == NULL)
            { // Primeira posição
                *lista = atual->proximo;
            }
            else
            {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            printf("Compromisso excluido com sucesso!\n");
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("Compromisso com descricao '%s' nao encontrado.\n", descricao);
}

// Função para marcar compromisso como concluído
void marcarComoConcluido(No *lista, char descricao[])
{
    while (lista != NULL)
    {
        if (strcmp(lista->compromisso.descricao, descricao) == 0)
        {
            lista->compromisso.concluido = 1;
            printf("Compromisso '%s' marcado como concluido.\n", descricao);
            return;
        }
        lista = lista->proximo;
    }
    printf("Compromisso com descricao '%s' não encontrado.\n", descricao);
}

void ordenarPorData(No **lista)
{
    if (*lista == NULL)
        return;

    int trocado;
    No *ptr1, *lptr = NULL;

    do
    {
        trocado = 0;
        ptr1 = *lista;

        while (ptr1->proximo != lptr)
        {
            No *prox = ptr1->proximo;

            if (ptr1->compromisso.ano > prox->compromisso.ano ||
                (ptr1->compromisso.ano == prox->compromisso.ano &&
                 ptr1->compromisso.mes > prox->compromisso.mes) ||
                (ptr1->compromisso.ano == prox->compromisso.ano &&
                 ptr1->compromisso.mes == prox->compromisso.mes &&
                 ptr1->compromisso.dia > prox->compromisso.dia) ||
                (ptr1->compromisso.ano == prox->compromisso.ano &&
                 ptr1->compromisso.mes == prox->compromisso.mes &&
                 ptr1->compromisso.dia == prox->compromisso.dia &&
                 ptr1->compromisso.hora > prox->compromisso.hora) ||
                (ptr1->compromisso.ano == prox->compromisso.ano &&
                 ptr1->compromisso.mes == prox->compromisso.mes &&
                 ptr1->compromisso.dia == prox->compromisso.dia &&
                 ptr1->compromisso.hora == prox->compromisso.hora &&
                 ptr1->compromisso.minuto > prox->compromisso.minuto) ||
                (ptr1->compromisso.ano == prox->compromisso.ano &&
                 ptr1->compromisso.mes == prox->compromisso.mes &&
                 ptr1->compromisso.dia == prox->compromisso.dia &&
                 ptr1->compromisso.hora == prox->compromisso.hora &&
                 ptr1->compromisso.minuto == prox->compromisso.minuto))
            {
                // Trocar compromissos
                Compromisso temp = ptr1->compromisso;
                ptr1->compromisso = prox->compromisso;
                prox->compromisso = temp;
                trocado = 1;
            }
            ptr1 = ptr1->proximo;
        }
        lptr = ptr1;
    } while (trocado);
}
// Função para ordenar compromissos por data, hora e prioridade
void ordenarPorPrioridade(No **lista)
{
    if (*lista == NULL)
        return;

    int trocado;
    No *ptr1, *lptr = NULL;

    do
    {
        trocado = 0;
        ptr1 = *lista;

        while (ptr1->proximo != lptr)
        {
            No *prox = ptr1->proximo;

            if (ptr1->compromisso.prioridade > prox->compromisso.prioridade)
            {
                // Trocar compromissos
                Compromisso temp = ptr1->compromisso;
                ptr1->compromisso = prox->compromisso;
                prox->compromisso = temp;
                trocado = 1;
            }
            ptr1 = ptr1->proximo;
        }
        lptr = ptr1;
    } while (trocado);
}

// Função para salvar compromissos em um arquivo
void salvarCompromissos(No *lista, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo para escrita!\n");
        return;
    }

    while (lista != NULL)
    {
        fwrite(&lista->compromisso, sizeof(Compromisso), 1, arquivo);
        lista = lista->proximo;
    }
    fclose(arquivo);
    printf("Compromissos salvos com sucesso em '%s'.\n", nomeArquivo);
}

// Função para carregar compromissos de um arquivo
void carregarCompromissos(No **lista, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo para leitura!\n");
        return;
    }

    Compromisso temp;
    while (fread(&temp, sizeof(Compromisso), 1, arquivo))
    {
        No *novo = (No *)malloc(sizeof(No));
        if (novo == NULL)
        {
            printf("Erro ao alocar memória!\n");
            fclose(arquivo);
            return;
        }
        novo->compromisso = temp;
        novo->proximo = *lista;
        *lista = novo;
    }
    fclose(arquivo);
    printf("Compromissos carregados com sucesso de '%s'.\n", nomeArquivo);
}

void menu()
{

    printf("========== Sistema de Agenda Pessoal ==========\n\n");
    printf("    1. Adicionar compromisso\n");
    printf("    2. Exibir compromissos\n");
    printf("    3. Buscar compromissos por data\n");
    printf("    4. Excluir compromisso\n");
    printf("    5. Marcar compromisso como concluido\n");
    printf("    6. Ordenar compromissos\n");
    printf("    7. Salvar compromissos\n");
    printf("    8. Carregar compromissos\n");
    printf("    9. Sair\n\n");
    printf("===============================================\n");
}

int main()
{
    No *lista = NULL;
    int opcao;
    char descricao[100];
    int aux;

    do
    {
        menu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            adicionarCompromisso(&lista);
            break;
        case 2:
            exibirCompromissos(lista);
            break;
        case 3:
        {
            int dia, mes, ano;
            printf("Digite a data (dd/mm/aaaa): ");
            scanf("%d/%d/%d", &dia, &mes, &ano);
            buscarCompromissosPorData(lista, dia, mes, ano);
            break;
        }
        case 4:
            printf("Digite a descricaoo do compromisso para excluir: ");
            scanf(" %[^\n]", descricao);
            excluirCompromisso(&lista, descricao);
            break;
        case 5:
            printf("Digite a descricao do compromisso para marcar como concluido: ");
            scanf(" %[^\n]", descricao);
            marcarComoConcluido(lista, descricao);
            break;
        case 6:
            clrscr();
            printf("1 - Ordenar por prioridade\n 2 - Ordenar por data\n 0 - Voltar\n");
            scanf("%d", &aux);
            if (aux == 1)
            {
                ordenarPorPrioridade(&lista);
                break;
            }
            else if (aux == 2)
            {
                ordenarPorData(&lista);
                break;
            }
            break;
        case 7:
            salvarCompromissos(lista, "compromissos.dat");
            break;
        case 8:
            carregarCompromissos(&lista, "compromissos.dat");
            break;
        case 9:
            printf("Saindo do programa...\n");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 9);

    return 0;
}
