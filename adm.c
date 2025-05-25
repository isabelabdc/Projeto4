#include <stdio.h>
#include <stdlib.h>
#include "adm.h"

//adiministrador predefinido:
#define ADM_NOME "Isabela"

int main(){
    Investidor usuarios[TOTAL_USUARIOS];
    int totalUsuarios = carregarUsuarios(usuarios);

    //login:
    printf("Bem-vindo(a) Administrador(a)!\n");

    int resultado=loginAdm();
    while(resultado!=0){
        resultado=loginAdm();
    }

    printf("Ola, %s!", ADM_NOME);

     //menu:
     int opcao;
     printf("\n");
      do {
        printf("1. Cadastrar novo investidor\n");
        printf("2. Excluir investidor\n");
        printf("3. Cadastrar nova criptomoeda\n");
        printf("4. Excluir criptomoeda\n");
        printf("5. Consultar saldo de um investidor\n");
        printf("6. Consultar extrato de um investidor\n");
        printf("7. Atualizar cotacoes de criptomoedas\n");
        printf("0. Sair\n");
        scanf("%d", &opcao);
        if(opcao==0){

        } else if(opcao==1){
            cadastrarInvestidor(usuarios, &totalUsuarios);
            salvarUsuarios(usuarios, totalUsuarios);
        } else if(opcao==2){
            excluirInvestidor();
        } else if(opcao==3){
            cadastrarCriptomoeda();
        } else if(opcao==4){
            excluirCriptomoeda();
        } else if(opcao==5){
            int logado = consultarSaldo(usuarios, totalUsuarios);
            if (logado == -1) {
                continue;
                }
            printf("\n--- SALDO ---\n");
            printf("Reais: R$ %.2f\n", usuarios[logado].reais);
            printf("Bitcoin: %.6f BTC\n", usuarios[logado].bitcoin);
            printf("Ethereum: %.6f ETH\n", usuarios[logado].ethereum);
            printf("Ripple: %.6f XRP\n", usuarios[logado].ripple);
            for (int i = 0; i < usuarios[logado].totalOutras; i++) {
                printf("%-10s: %.4f\n", usuarios[logado].outras[i].nome, usuarios[logado].outras[i].quantidade);
    }
        } else if(opcao==6){
            int logado=consultarExtrato(usuarios, totalUsuarios);
            if (logado == -1) {
                continue;
                }
            printf("\n--- EXTRATO ---\n");
            for (int i = 0; i < usuarios[logado].totalTransacoes; i++) {
            printf("%d/%d/%d %d:%d:%d - %s %s %.2f (Taxa: %.2f%%)\n", 
                usuarios[logado].transacoes[i].data.dia, usuarios[logado].transacoes[i].data.mes, usuarios[logado].transacoes[i].data.ano,
                usuarios[logado].transacoes[i].data.hora, usuarios[logado].transacoes[i].data.minuto, usuarios[logado].transacoes[i].data.segundo,
                usuarios[logado].transacoes[i].operacao, usuarios[logado].transacoes[i].moeda, 
                usuarios[logado].transacoes[i].valor, usuarios[logado].transacoes[i].taxa * 100);
        }
       
        } else if(opcao==7){
            atualizarCotacao();
        } else {
            printf("Opcao invalida. Tente novamente!\n");
        }

    } while(opcao!=0);

    return 0;
}