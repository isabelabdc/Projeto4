//bibliotecas:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adm.h"

//funcao que carrega o extrato:
void carregarExtrato(Investidor *usuario) {
    char nomeArquivo[100];
    sprintf(nomeArquivo, "extrato_%s.bin", usuario->cpf);

    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        usuario->totalTransacoes = 0;
        return; 
    }
    fread(&usuario->totalTransacoes, sizeof(int), 1, arquivo);
    fread(usuario->transacoes, sizeof(Extrato), usuario->totalTransacoes, arquivo);
    fclose(arquivo);
}

//funcao que carrega dados do usuario no arquivo
int carregarUsuarios(Investidor usuarios[]) {
    FILE *fp = fopen("usuarios.dat", "rb");
    int total = 0;
    if (fp) {
        fread(&total, sizeof(int), 1, fp);
        fread(usuarios, sizeof(Investidor), total, fp);
        //carrega o extrato:
        for (int i = 0; i < total; i++) {
            carregarExtrato(&usuarios[i]);
        }
        fclose(fp);
    }
    return total;
}

void salvarUsuarios(Investidor usuarios[], int total) {
    FILE *fp = fopen("usuarios.dat", "wb");
    if (fp) {
        fwrite(&total, sizeof(int), 1, fp);
        fwrite(usuarios, sizeof(Investidor), total, fp);
        fclose(fp);
    } else {
        printf("Erro ao salvar arquivo de usuarios!\n");
    }
}

//administrador predefinido:
#define ADM_CPF "12345678900"
#define ADM_SENHA "admin123"

int loginAdm() {
    char cpf[12], senha[20];

    printf("Digite seu CPF: ");
    scanf(" %11s", cpf);
    printf("Digite sua senha: ");
    scanf(" %19s", senha);

    if (strcmp(cpf, ADM_CPF) == 0 && strcmp(senha, ADM_SENHA) == 0) {
        return 0;
    } else {
        printf("CPF ou senha incorretos!\n");
        return 1;
    }
}

void cadastrarInvestidor(Investidor usuarios[], int *total) {
    Investidor novo;
    FILE *arquivo = fopen("usuarios.dat", "ab");

    if (*total >= TOTAL_USUARIOS) {
        printf("Limite de usuarios atingido!\n");
        return;
    }

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de investidores.\n");
        return;
    }

    printf("Digite o CPF do novo investidor: ");
    scanf(" %11s", novo.cpf);
    printf("Digite a senha: ");
    scanf(" %19s", novo.senha);
    printf("Digite o nome do novo investidor: ");
    scanf(" %[^\n]", novo.nome);
    novo.reais = 0.0;
    novo.bitcoin = 0.0;
    novo.ethereum = 0.0;
    novo.ripple = 0.0;
    novo.totalOutras=0;
    novo.totalTransacoes=0;

    usuarios[*total] = novo;
    (*total)++;

    fclose(arquivo);

    printf("Investidor cadastrado com sucesso!\n");
}

void excluirInvestidor(){
    char cpf[12];
    int encontrado = 0;

    FILE *arquivo = fopen("usuarios.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");  //arquivo temporario

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    printf("Informe o CPF do investidor a ser excluido: ");
    scanf(" %11s", cpf);

    Investidor inv;
    while (fread(&inv, sizeof(Investidor), 1, arquivo)) {
        if (strcmp(inv.cpf, cpf) == 0) {
            encontrado = 1;
            printf("\nInvestidor encontrado:\nNome: %s\nCPF: %s\n", inv.nome, inv.cpf);
            char confirmacao;
            printf("Deseja realmente excluir este investidor?\n ");
            printf("1. Sim\n2. Nao\n");
            scanf(" %d", &confirmacao);
            if (confirmacao == 1 ) {
                printf("Investidor excluido com sucesso.\n");
                continue;
            }
        }
        fwrite(&inv, sizeof(Investidor), 1, temp);  //reescreve todos os investidores no arquivo 'temp' mas sem o investidor excluido
    }

    fclose(arquivo);
    fclose(temp);

    remove("usuarios.dat");  //apaga o arquivo antigo
    rename("temp.dat", "usuarios.dat");  //o arquivo sem o investidor excluido passa a ser o novo arquivo com os usuarios

    if (!encontrado)
        printf("\nInvestidor com CPF %s nao encontrado.\n", cpf);

}

void cadastrarCriptomoeda() {
    Criptomoeda nova;
    FILE *arquivo = fopen("criptomoedas.dat", "ab");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de criptomoedas.\n");
        return;
    }

    printf("Digite o nome da nova criptomoeda: ");
    scanf(" %[^\n]", nova.nome);
    printf("Cotacao inicial da nova criptomoeda(R$): ");
    scanf("%f", &nova.cotacao);
    printf("Qual sera a taxa de compra (%%)? ");
    scanf("%f", &nova.taxaCompra);
    printf("Qual sera a taxa de venda (%%)? ");
    scanf("%f", &nova.taxaVenda);

    fwrite(&nova, sizeof(Criptomoeda), 1, arquivo);
    fclose(arquivo);

    printf("Criptomoeda cadastrada com sucesso!\n");
}

void excluirCriptomoeda(){
char nome[50];
    int encontrado = 0;

    FILE *arquivo = fopen("criptomoedas.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");  //arquivo temporario

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    printf("Digite o nome da criptomoeda que deseja excluir: ");
    scanf(" %[^\n]", nome);

    Criptomoeda c;
    while (fread(&c, sizeof(Criptomoeda), 1, arquivo)) {
        if (strcmp(c.nome, nome) == 0) {
            encontrado = 1;
            printf("Criptomoeda encontrada:\nNome: %s\nCotacao: R$ %.2f\n", c.nome, c.cotacao);
            char confirmacao;
            printf("Deseja  realmente excluir essa criptomoeda?\n");
            printf("1. Sim\n2. Nao\n");
            scanf(" %d", &confirmacao);
            if (confirmacao == 1) {
                printf("Criptomoeda excluida com sucesso.\n");
                continue; 
            }
        }
        fwrite(&c, sizeof(Criptomoeda), 1, temp);  //reescreve todas as criptomoedas no arquivo 'temp' mas sem a criptomoeda excluida
    }

    fclose(arquivo);
    fclose(temp);

    remove("criptomoedas.dat");  //apaga o arquivo antigo
    rename("temp.dat", "criptomoedas.dat");  //o arquivo sem a criptomoeda excluida passa a ser o novo arquivo

    if (!encontrado)
        printf("Criptomoeda '%s' nao encontrada.\n", nome);
}

int consultarSaldo(Investidor usuarios[], int total){
    char cpf[12];

    FILE *arquivo = fopen("usuarios.dat", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de investidores.\n");
        return -1;
    }

    printf("Digite o CPF do investidor: ");
    scanf("%11s", cpf);

    for (int i = 0; i < total; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0) {
            return i;
        }
    }
    
    printf("Investidor com CPF %s nao encontrado.\n", cpf);
    return -1;
    fclose(arquivo);
}

int consultarExtrato(Investidor usuarios[], int total){
    char cpf[12];
    char nomeArquivo[50];

    printf("Digite o CPF do investidor: ");
    scanf(" %11s", cpf);

    snprintf(nomeArquivo, sizeof(nomeArquivo), "extrato_%s.bin", cpf);

    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Nenhum extrato encontrado para o CPF %s.\n", cpf);
        return -1;
    }

    for (int i = 0; i < total; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0 ){
            return i;
        }
    }

    fclose(arquivo);
}


void atualizarCotacao(){
    char nome[50];
    int encontrado = 0;
    float novaCotacao;

    FILE *arquivo = fopen("criptomoedas.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    printf("Digite o nome da criptomoeda: ");
    scanf(" %[^\n]", nome);

    Criptomoeda c;
    while (fread(&c, sizeof(Criptomoeda), 1, arquivo)) {
        if (strcmp(c.nome, nome) == 0) {
            encontrado = 1;
            printf("Cotacao atual de %s: R$ %.2f\n", c.nome, c.cotacao);
            printf("Digite nova cotacao: ");
            scanf("%f", &novaCotacao);
            c.cotacao = novaCotacao;
            printf("Cotacao atualizada.\n");
        }
        fwrite(&c, sizeof(Criptomoeda), 1, temp);
    }

    fclose(arquivo);
    fclose(temp);

    remove("criptomoedas.dat");
    rename("temp.dat", "criptomoedas.dat");

    if (!encontrado)
        printf("Criptomoeda '%s' nao encontrada.\n", nome);
}

