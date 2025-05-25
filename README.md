# Projeto4
Projeto 4 -- Desenvolvimento de Algoritmos 

GRUPO I:  Isabela Bonoto de Castro - RA: 24.224.008-3;  Gustavo Runge Passarelli - RA: 52.224.004-3; 

OBJETIVO:

O objetivo deste projeto é desenvolver um sistema de exchange de criptomoedas, dividido em dois programas: um para o investidor e outro para o administrador.

*O programa do investidor permite realizar cadastro, login, consultar saldo e extrato, além de efetuar operações como depósito, saque, compra e venda de criptomoedas (incluindo moedas personalizadas, criadas por administradores).

*O programa do administrador possibilita cadastrar e excluir investidores e criptomoedas, atualizar cotações das criptomoedas criadas e consultar saldo e extrato de qualquer investidor pelo CPF.

Todos os dados são armazenados em arquivos binários, garantindo a persistência das informações.


Arquivos:


-"usuarios.dat" contém as informações dos investidores;

-"criptomoedas.dat" contém as informações das criptomoedas personalizadas (Bitcoin, Ethereum e Ripple são as criptomoedas fixas que não podem ser excluídas ou alteradas pelos administradores, então não estão no arquivo);

-Cada investidor tem seu próprio arquivo para extrato. Exemplo: "extrato_12345678900.bin" , é o arquivo de extrato para o Cpf: 12345678900;


FORMA DE USO:

*Programa do Investidor:
Ao iniciar, o investidor pode fazer cadastro ou login com CPF e senha. Após o login, ele acessa um menu com as opções de consultar saldo e extrato, depositar, sacar, comprar e vender criptomoedas. As operações são feitas digitando os dados solicitados, como valores e confirmação de senha. Ao sair, todas as informações são salvas em arquivos para que estejam disponíveis na próxima execução do programa.

-CPFs de investidor já cadastrados: "12345678900" , "12345678988" ;

-Senhas para os respectivos CPF: "admin123" , "oi123" ;


*Programa do Administrador:
O administrador faz login com CPF e senha já predefinidos. Após o login, ele pode cadastrar ou excluir investidores e criptomoedas, consultar saldo e extrato de qualquer investidor informando o CPF, além de atualizar as cotações das criptomoedas personalizadas. As operações são feitas digitando os dados solicitados. Os dados dos novos usuários e criptomoedas cadastrados pelo administrador são salvos automaticamente em seus respectivos arquivos binários ao final de cada operação.


-CPF para login do administrador: 12345678900 ;


-Senha para login do administrador: admin123 ;


FORMA DE COMPILAÇÃO E EXECUÇÃO UTILIZADA: 

Para compilar e executar o código no Windows, foi feito o download do MinGW-w64, que permite compilar programas em C usando o GCC. Após fazer o download e devidas configurações, abra o prompt de comando ou terminal no diretório do projeto.

*Programa do Invesidor:

-Para compilar:
Compile os arquivos "investidor.c" e "funcoesi.c" juntos, utilizando o comando:

"gcc investidor.c funcoesi.c -o inv"  ;

Esse comando vai gerar o arquivo executavel "inv.exe". 


-Para executar:
Digite "inv.exe" no terminal para iniciar o programa para investidores;



*Programa do Administrador:

-Para compilar:
Compile os arquivos "adm.c" e "funcoesa.c" juntos, utilizando o comando:

"gcc adm.c funcoesa.c -o adm"  ;

Esse comando vai gerar o arquivo executavel "adm.exe". 


-Para executar:

Digite "adm.exe" no terminal para iniciar o programa para administradores;

