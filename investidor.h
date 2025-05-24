//define o numero maximo de usuarios e transacoes
#define TOTAL_USUARIOS 10
#define TOTAL_TRANSACOES 100
//define o numero de caracteres maximos do cpf e senha
#define CPF_TAM 12  
#define SENHA_TAM 20  

//estrutura da data para ultilizar no extrato:
typedef struct {
    int dia, mes, ano, hora, minuto, segundo;
} DataHora;

//estrutura do extrato:
typedef struct {
    char operacao[20];
    DataHora data;
    char moeda[10];
    float valor;
    float taxa;
} Extrato;

//estrutura do usuario:
typedef struct {
    char cpf[CPF_TAM];
    char senha[SENHA_TAM];
    char nome[50];
    float reais;
    float bitcoin;
    float ethereum;
    float ripple;
    Extrato transacoes[TOTAL_TRANSACOES];
    int totalTransacoes;
} Usuario;

//estrutura das cotacoes:
typedef struct {
    float cotacaoBitcoin;
    float cotacaoEthereum;
    float cotacaoRipple;
} Cotacoes;

//declaracao de funcoes:
DataHora data();
void salvarExtrato(Usuario *usuario);
void carregarExtrato(Usuario *usuario);
void salvarUsuarios(Usuario usuarios[], int total);
int carregarUsuarios(Usuario usuarios[]);
int login(Usuario usuarios[], int total, char *cpf, char *senha);
int cadastrarUsuario(Usuario usuarios[], int *total);
void sacar(Usuario *usuario);
void depositar(Usuario *usuario);
void comprar(Usuario *usuario, Cotacoes cotacoes);
void vender(Usuario *usuario, Cotacoes cotacoes);
float variacaoAleatoria();
void atualizar(Cotacoes *cotacoes);