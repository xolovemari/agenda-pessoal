#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void ordenarPorData(No **lista);

//Função para verificar se uma data é valida
int dataValida(int dia, int mes, int ano) {
    if (ano < 1 || mes < 1 || mes > 12 || dia < 1)
        return 0;

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Ajusta para anos bissextos
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
        diasPorMes[1] = 29;

    return dia <= diasPorMes[mes - 1];
}

//Busca um compromisso por data
No *buscaBinariaPorData(No *lista, int dia, int mes, int ano) {
    ordenarPorData(&lista);
    int tamanho = 0;
    No *temp = lista;

    // Contar o número de elementos na lista
    while (temp) {
        tamanho++;
        temp = temp->proximo;
    }

    // Converter a lista ligada em um array
    No **array = (No **)malloc(tamanho * sizeof(No *));
    temp = lista;
    for (int i = 0; i < tamanho; i++) {
        array[i] = temp;
        temp = temp->proximo;
    }

    // Realizar busca binária
    int inicio = 0, fim = tamanho - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        No *meioNo = array[meio];

        if (ano < meioNo->compromisso.ano ||
            (ano == meioNo->compromisso.ano && mes < meioNo->compromisso.mes) ||
            (ano == meioNo->compromisso.ano && mes == meioNo->compromisso.mes && dia < meioNo->compromisso.dia)) {
            fim = meio - 1;
        } else if (ano > meioNo->compromisso.ano ||
                   (ano == meioNo->compromisso.ano && mes > meioNo->compromisso.mes) ||
                   (ano == meioNo->compromisso.ano && mes == meioNo->compromisso.mes && dia > meioNo->compromisso.dia)) {
            inicio = meio + 1;
        } else {
            free(array);
            return meioNo; // Encontrado
        }
    }

    free(array);
    return NULL; // Não encontrado
}


// Função para adicionar um compromisso
void adicionarCompromisso(No **lista) { 
    printf("|       Digite a descricao do compromisso: ");
    char descricao[100];
    scanf(" %[^\n]", descricao);

    int dia, mes, ano, hora, minuto, prioridade;

    printf("|       Digite a data (dd/mm/aaaa): ");
    if (scanf("%2d/%2d/%4d", &dia, &mes, &ano) != 3 || !dataValida(dia, mes, ano)) {
        puts("Data invalida. Operacao cancelada.");
        return;
    }

    printf("|       Digite o horario (hh:mm): ");
    if (scanf("%2d:%2d", &hora, &minuto) != 2 || hora < 0 || hora > 23 || minuto < 0 || minuto > 59) {
        puts("Horario invalido. Operacao cancelada.");
        return;
    }

    printf("|       Digite a prioridade (1 - Alta, 2 - Media, 3 - Baixa): ");
    if (scanf("%d", &prioridade) != 1 || prioridade < 1 || prioridade > 3) {
        puts("Prioridade invalida. Operacao cancelada.");
        return;
    }

    // Verificar se o compromisso já existe na lista
    ordenarPorData(lista);
    No *atual = *lista;
    while (atual != NULL) {
        if (atual->compromisso.ano > ano ||
            (atual->compromisso.ano == ano && atual->compromisso.mes > mes) ||
            (atual->compromisso.ano == ano && atual->compromisso.mes == mes && atual->compromisso.dia > dia) ||
            (atual->compromisso.ano == ano && atual->compromisso.mes == mes && atual->compromisso.dia == dia &&
             atual->compromisso.hora > hora) ||
            (atual->compromisso.ano == ano && atual->compromisso.mes == mes && atual->compromisso.dia == dia &&
             atual->compromisso.hora == hora && atual->compromisso.minuto > minuto)) {
            break; // Lista ordenada; se passou da posição, podemos parar.
        }

        if (strcmp(atual->compromisso.descricao, descricao) == 0 &&
            atual->compromisso.dia == dia &&
            atual->compromisso.mes == mes &&
            atual->compromisso.ano == ano &&
            atual->compromisso.hora == hora &&
            atual->compromisso.minuto == minuto) {
            printf("\n|       Compromisso ja existe!! Nao adicionado\n");
            return;
        }

        atual = atual->proximo;
    }


    // Alocar memória para o novo compromisso
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro ao alocar memoria!\n");
        return;
    }

    // Preencher os dados do novo compromisso
    strcpy(novo->compromisso.descricao, descricao);
    novo->compromisso.dia = dia;
    novo->compromisso.mes = mes;
    novo->compromisso.ano = ano;
    novo->compromisso.hora = hora;
    novo->compromisso.minuto = minuto;
    novo->compromisso.prioridade = prioridade;
    novo->compromisso.concluido = 0; // Por padrão, não concluído
    novo->proximo = NULL;

    // Inserir no final da lista
    if (*lista == NULL) {
        *lista = novo;
    } else {
        No *temp = *lista;
        while (temp->proximo != NULL) {
            temp = temp->proximo;
        }
        temp->proximo = novo;
    }

    printf("|       Compromisso adicionado com sucesso!\n\n");
}


// Função para exibir compromissos
void exibirCompromissos(No *lista)
{
    if (lista == NULL)
    {
        printf("Nenhum compromisso encontrado.\n");
        return;
    }

    printf("============ Lista de Compromissos ============\n");
    printf("| Descricao           | Data       | Hora  | Prioridade | Concluido |\n");
    printf("|---------------------|------------|-------|------------|-----------|\n");

    while (lista != NULL) {
    printf("| %-20s | %02d/%02d/%04d | %02d:%02d | %-10s | %-9s |\n",
           lista->compromisso.descricao,
           lista->compromisso.dia, lista->compromisso.mes, lista->compromisso.ano,
           lista->compromisso.hora, lista->compromisso.minuto,
           lista->compromisso.prioridade == 1 ? "Alta" :
           lista->compromisso.prioridade == 2 ? "Media" : "Baixa",
           lista->compromisso.concluido ? "Sim" : "Nao");
    lista = lista->proximo;
}
    printf("===============================================\n\n");
}

// Função para buscar compromissos por data
void buscarCompromissosPorData(No *lista, int dia, int mes, int ano) {
    No *resultado = buscaBinariaPorData(lista, dia, mes, ano);
    if (resultado) {
        printf("\n  Descricao: %s\n", resultado->compromisso.descricao);
        printf("    Hora: %02d:%02d\n", resultado->compromisso.hora, resultado->compromisso.minuto);
        printf("    Prioridade: %d\n", resultado->compromisso.prioridade);
        printf("    Concluido: %s\n", resultado->compromisso.concluido ? "Sim" : "Nao");
        printf("----------------------------\n");
    } else {
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
    printf("Compromisso com descricao '%s' nao encontrado.\n", descricao);
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
// Função para ordenar compromissos por prioridade
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
        printf("Erro ao abrir arquivo '%s' para escrita. Verifique as permissoes.\n", nomeArquivo);
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
        printf("Nenhum compromisso encontrado no arquivo '%s'. Continuando...\n", nomeArquivo);
        return; // Não há compromissos, apenas retorna sem erro
    }
    

    Compromisso temp;
    while (fread(&temp, sizeof(Compromisso), 1, arquivo))
    {
        No *novo = (No *)malloc(sizeof(No));
        if (novo == NULL)
        {
            printf("Erro ao alocar memoria!\n");
            fclose(arquivo);
            return;
        }
        novo->compromisso = temp;
        novo->proximo = *lista;
        *lista = novo;
    }
    fclose(arquivo);
    clrscr();
    printf("Compromissos carregados com sucesso de '%s'.\n", nomeArquivo);
}

void menu()
{
    printf("\n========== Sistema de Agenda Pessoal ==========\n\n");
    printf("    1. Adicionar compromisso\n");
    printf("    2. Exibir compromissos\n");
    printf("    3. Buscar compromissos por data\n");
    printf("    4. Excluir compromisso\n");
    printf("    5. Marcar compromisso como concluido\n");
    printf("    6. Ordenar compromissos\n");
    printf("    7. Salvar compromissos\n");
    printf("    8. Sair\n\n");
    printf("===============================================\n");
    
}

int main() {
    No *lista = NULL;
    int opcao;
    char descricao[100], nomeUsuario[100], nomeArquivo[100];
    int aux;

    printf("Nome de usuario: ");
    scanf("%s", nomeUsuario);

    //Verificacoes de nome do usuario
    if (strlen(nomeUsuario) == 0) {
        printf("Erro: Nome de usuario nao pode ser vazio.\n");
        return 1;
    }
    if (strlen(nomeUsuario) >= 100) {
        printf("Erro: Nome de usuario muito longo.\n");
        return 1;
    }
    for (int i = 0; i < strlen(nomeUsuario); i++) {
        if (nomeUsuario[i] == '/' || nomeUsuario[i] == '\\' || nomeUsuario[i] == ':') {
            printf("Erro: Nome de usuario contem caracteres invalidos.\n");
            return 1;
        }
    }

    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.dat", nomeUsuario);    
    //Tenta carregar compromissos do arquivo caso o usuario ja exista
    carregarCompromissos(&lista, nomeUsuario);


    do {
        menu();
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 8) {
            printf("Opcao invalida! Tente novamente.\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada
            continue;
        }

        switch (opcao) {
        case 1:
            adicionarCompromisso(&lista);
            break;
        case 2:
            exibirCompromissos(lista);
            break;
        case 3: {
            int dia, mes, ano;
            printf("Digite a data (dd/mm/aaaa): ");
            scanf("%d/%d/%d", &dia, &mes, &ano);
            ordenarPorData(&lista);
            buscarCompromissosPorData(lista, dia, mes, ano);
            break;
        }
        case 4:
            printf("Digite a descricao do compromisso para excluir: ");
            scanf(" %[^\n]", descricao);
            excluirCompromisso(&lista, descricao);
            break;
        case 5:
            printf("Digite a descricao do compromisso para marcar como concluido: ");
            scanf(" %[^\n]", descricao);
            marcarComoConcluido(lista, descricao);
            break;
        case 6:
            printf("    |1 - Ordenar por prioridade\n    |2 - Ordenar por data\n    |0 - Voltar\n");
            scanf("%d", &aux);
            if (aux == 1) {
                ordenarPorPrioridade(&lista);
                break;
            }
            else if (aux == 2) {
                ordenarPorData(&lista);
                break;
            }
            break;
        case 7:
            salvarCompromissos(lista, nomeArquivo);
            break;
        case 8:
            printf("Tem certeza de que deseja sair? (S/N): ");
            char sair;
            scanf(" %c", &sair);
            if (sair == 'S' || sair == 's') {
                salvarCompromissos(lista, nomeArquivo);
                printf("Saindo do programa...\n");
                break;
            } else {
                printf("Operacao cancelada. Voltando ao menu...\n");
            }
            continue;
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao != 8);

    //Libera a memoria da lista antes que o programa acabe
    No *temp;
    while (lista != NULL) {
        temp = lista;
        lista = lista->proximo;
        free(temp);
    }

    return 0;
}
