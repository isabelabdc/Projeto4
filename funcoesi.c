#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "investidor.h"

//EXTRATO:
//funcao para obter a data e hora atual:
DataHora data() {
    DataHora data;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    data.dia = tm_info->tm_mday;
    data.mes = tm_info->tm_mon + 1;
    data.ano = tm_info->tm_year + 1900;
    data.hora = tm_info->tm_hour;
    data.minuto = tm_info->tm_min;
    data.segundo = tm_info->tm_sec;
    return data;
}

//funcao que salva o extrato em arquivo binario:
void salvarExtrato(Usuario *usuario) {
    char nomeArquivo[100];
    sprintf(nomeArquivo, "extrato_%s.bin", usuario->cpf);

    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        printf("Erro ao salvar extrato!\n");
        return;
    }
    fwrite(&usuario->totalTransacoes, sizeof(int), 1, arquivo);
    fwrite(usuario->transacoes, sizeof(Extrato), usuario->totalTransacoes, arquivo);
    fclose(arquivo);
}

//funcao que carrega o extrato:
void carregarExtrato(Usuario *usuario) {
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

//DADOS DE USUARIOS:
//funcao para salvar dados de usuarios no arquivo
void salvarUsuarios(Usuario usuarios[], int total) {
    FILE *fp = fopen("usuarios.dat", "wb");
    if (fp) {
        fwrite(&total, sizeof(int), 1, fp);
        fwrite(usuarios, sizeof(Usuario), total, fp);
        fclose(fp);
    } else {
        printf("Erro ao salvar arquivo de usuarios!\n");
    }
}

//funcao que carrega dados do usuario no arquivo
int carregarUsuarios(Usuario usuarios[]) {
    FILE *fp = fopen("usuarios.dat", "rb");
    int total = 0;
    if (fp) {
        fread(&total, sizeof(int), 1, fp);
        fread(usuarios, sizeof(Usuario), total, fp);
        //carrega o extrato:
        for (int i = 0; i < total; i++) {
            carregarExtrato(&usuarios[i]);
        }
        fclose(fp);
    }
    return total;
}

//LOGIN
//funcao para realizacao do login
int login(Usuario usuarios[], int total, char *cpf, char *senha) {
    for (int i = 0; i < total; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0 && strcmp(usuarios[i].senha, senha) == 0) {
            return i;
        }
    }
    return -1;
}

//CADASTRO
//funcao para realizacao de cadastro 
int cadastrarUsuario(Usuario usuarios[], int *total) {
    if (*total >= TOTAL_USUARIOS) {
        printf("Limite de usuarios atingido!\n");
        return -1;
    }

    Usuario novo = {0};

    printf("Digite seu CPF: ");
    scanf(" %11s", novo.cpf);

    for (int i = 0; i < *total; i++) {
        if (strcmp(usuarios[i].cpf, novo.cpf) == 0) {
            printf("Usuario com este CPF ja esta cadastrado.\n");
            return -1;
        }
    }
    
    printf("Digite seu primeiro nome: ");
    scanf(" %49s", novo.nome);

    printf("Digite sua senha: ");
    scanf(" %19s", novo.senha);

    novo.reais = 0.0;
    novo.bitcoin = 0.0;
    novo.ethereum = 0.0;
    novo.ripple = 0.0;
    novo.totalTransacoes = 0;

    usuarios[*total] = novo;
    (*total)++;

    printf("Cadastro realizado com sucesso!\n");
    return 0;
}

//FUNCIONALIDADES:
//funcao depositar
void depositar(Usuario *usuario) {
    float valor;
    printf("Digite o valor a ser depositado R$ ");
    scanf(" %f", &valor);

    if (valor <= 0) {
        printf("Valor invalido!\n");
        return;
    }

    usuario->reais += valor;

    //registra no extrato:
    if (usuario->totalTransacoes < TOTAL_TRANSACOES) {
        Extrato *transacao = &usuario->transacoes[usuario->totalTransacoes++];
        strcpy(transacao->operacao, "Deposito");
        strcpy(transacao->moeda, "R$");
        transacao->valor = valor;
        transacao->taxa = 0.0;
        transacao->data = data();
    }
    salvarExtrato(usuario);
    printf("Deposito realizado com sucesso!\n");
}

//funcao sacar
void sacar(Usuario *usuario) {
    float valor;
    char senhaDigitada[SENHA_TAM];

    printf("Digite o valor a ser sacado R$ ");
    scanf(" %f", &valor);

    if (valor <= 0 || valor > usuario->reais) {
        printf("Valor invalido ou saldo insuficiente!\n");
        return;
    }

    printf("Confirme sua senha: ");
    scanf(" %19s", senhaDigitada);

    if (strcmp(usuario->senha, senhaDigitada) != 0) {
        printf("Senha incorreta. Saque cancelado!\n");
        return;
    }

    usuario->reais -= valor;

    //registra no extrato:
    if (usuario->totalTransacoes < TOTAL_TRANSACOES) {
        Extrato *transacao = &usuario->transacoes[usuario->totalTransacoes++];
        strcpy(transacao->operacao, "Saque");
        strcpy(transacao->moeda, "R$");
        transacao->valor = valor;
        transacao->taxa = 0.0;
        transacao->data = data();
    }
    salvarExtrato(usuario);
    printf("Saque realizado com sucesso!\n");
}

//funcao de comprar criptomoedas
void comprar(Usuario *usuario, Cotacoes cotacoes) {
    int opcao;
    float valor, taxa = 0, quantidade = 0;
    char senha[SENHA_TAM];
    char moeda[10];

    printf("1. Bitcoin (%.2f)\n", cotacoes.cotacaoBitcoin);
    printf("2. Ethereum (%.2f)\n", cotacoes.cotacaoEthereum);
    printf("3. Ripple (%.2f)\n", cotacoes.cotacaoRipple);
    printf("Digite o numero da moeda que deseja comprar: ");
    scanf("%d", &opcao);

    printf("Digite o valor em reais para a compra: ");
    scanf("%f", &valor);

    if (valor <= 0 || valor > usuario->reais) {
        printf("Valor invalido ou saldo insuficiente.\n");
        return;
    }

    printf("Confirme sua senha: ");
    scanf(" %19s", senha);

    if (strcmp(usuario->senha, senha) != 0) {
        printf("Senha incorreta. Operacao cancelada.\n");
        return;
    }

    if(opcao==1) {
            taxa = 0.02;
            quantidade = (valor * (1 - taxa)) / cotacoes.cotacaoBitcoin;
            strcpy(moeda, "BTC");
            usuario->bitcoin += quantidade;
    } else if(opcao==2){
            taxa = 0.01;
            quantidade = (valor * (1 - taxa)) / cotacoes.cotacaoEthereum;
            strcpy(moeda, "ETH");
            usuario->ethereum += quantidade;
    } else if(opcao==3){
            taxa = 0.01;
            quantidade = (valor * (1 - taxa)) / cotacoes.cotacaoRipple;
            strcpy(moeda, "XRP");
            usuario->ripple += quantidade;
    } else{
            printf("Opção inválida.\n");
            return;
    }

    usuario->reais -= valor;

    printf("Compra realizada com sucesso!\nVoce comprou %.6f %s (+Taxa de: %.2f%% aplicada)\n", quantidade, moeda, taxa * 100);
    //registra no extrato:
    if (usuario->totalTransacoes < TOTAL_TRANSACOES) {
        Extrato *trans = &usuario->transacoes[usuario->totalTransacoes++];
        strcpy(trans->operacao, "Compra");
        strcpy(trans->moeda, moeda);
        trans->valor = valor;
        trans->taxa = taxa;
        trans->data = data();
    }
    salvarExtrato(usuario);
}

//funcao vender criptomoedas
void vender(Usuario *usuario, Cotacoes cotacoes) {
    int opcao;
    float quantidade, taxa = 0, valorRecebido = 0;
    char senha[SENHA_TAM];
    char moeda[10];
    float *saldoMoeda = NULL;
    float cotacao = 0;

    printf("1. Bitcoin (%.2f)\n", cotacoes.cotacaoBitcoin);
    printf("2. Ethereum (%.2f)\n", cotacoes.cotacaoEthereum);
    printf("3. Ripple (%.2f)\n", cotacoes.cotacaoRipple);
    printf("Digite o numero da moeda que deseja vender: ");
    scanf("%d", &opcao);

    if(opcao==1) {
            taxa = 0.03;
            saldoMoeda = &usuario->bitcoin;
            cotacao = cotacoes.cotacaoBitcoin;
            strcpy(moeda, "BTC");
        } else if(opcao==2){
            taxa = 0.02;
            saldoMoeda = &usuario->ethereum;
            cotacao = cotacoes.cotacaoEthereum;
            strcpy(moeda, "ETH");
        } else if(opcao==3){
            taxa = 0.01;
            saldoMoeda = &usuario->ripple;
            cotacao = cotacoes.cotacaoRipple;
            strcpy(moeda, "XRP");
        } else{
            printf("Opcao invalida.\n");
            return;
        }

    printf("Digite quantidade de %s que deseja vender: ", moeda);
    scanf(" %f", &quantidade);

    if (quantidade <= 0 || quantidade > *saldoMoeda) {
        printf("Quantidade invalida ou saldo insuficiente.\n");
        return;
    }

    printf("Confirme sua senha: ");
    scanf(" %19s", senha);

    if (strcmp(usuario->senha, senha) != 0) {
        printf("Senha incorreta. Operacao cancelada.\n");
        return;
    }

    valorRecebido = quantidade * cotacao * (1 - taxa);
    *saldoMoeda -= quantidade;
    usuario->reais += valorRecebido;

    printf("Venda realizada com sucesso!\nVoce recebeu R$ %.2f (+Taxa de: %.2f%% aplicada)\n", valorRecebido, taxa * 100);
    //registra no extrato:
    if (usuario->totalTransacoes < TOTAL_TRANSACOES) {
        Extrato *trans = &usuario->transacoes[usuario->totalTransacoes++];
        strcpy(trans->operacao, "Venda");
        strcpy(trans->moeda, moeda);
        trans->valor = valorRecebido;
        trans->taxa = taxa;
        trans->data = data();
    }
    salvarExtrato(usuario);
}

//atualizando cotacoes aleatoriamnete entre -5% e +5%:
//funcao que gera uma variacao aleatoria
float random() {
    return ((rand() % 101) - 50) / 1000.0f;  // -0.05 a +0.05
}
//funcao para atualizar cotacoes
void atualizar(Cotacoes *cotacoes) {
    float variacaoBitcoin = random();
    float variacaoEthereum = random();
    float variacaoRipple = random();

    cotacoes->cotacaoBitcoin *= (1 + variacaoBitcoin);
    cotacoes->cotacaoEthereum *= (1 + variacaoEthereum);
    cotacoes->cotacaoRipple *= (1 + variacaoRipple);

    printf("\n--- COTACOES ATUALIZADAS ---\n");
    printf("Bitcoin: %.2f (%+.2f%%)\n", cotacoes->cotacaoBitcoin, variacaoBitcoin * 100);
    printf("Ethereum: %.2f (%+.2f%%)\n", cotacoes->cotacaoEthereum, variacaoEthereum * 100);
    printf("Ripple: %.2f (%+.2f%%)\n", cotacoes->cotacaoRipple, variacaoRipple * 100);
}