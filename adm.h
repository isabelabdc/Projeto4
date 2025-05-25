#define TOTAL_TRANSACOES 100
#define TOTAL_USUARIOS 10

//estruturas:
typedef struct {
    char nome[50];
    float quantidade;
} OutraMoeda;

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

typedef struct {
    char cpf[12];
    char senha[20];
    char nome[50];
    float reais;
    float bitcoin;
    float ethereum;
    float ripple;
    OutraMoeda outras[50];
    int totalOutras;
    Extrato transacoes[TOTAL_TRANSACOES];
    int totalTransacoes;
} Investidor;

typedef struct {
    char nome[50];
    float cotacao;
    float taxaCompra;
    float taxaVenda;
} Criptomoeda;


//funcoes:

int loginAdm();
void cadastrarInvestidor(Investidor usuarios[], int *total);
void salvarUsuarios(Investidor usuarios[], int total);
void excluirInvestidor();
void cadastrarCriptomoeda();
void excluirCriptomoeda();
int consultarSaldo(Investidor usuarios[], int total);
int consultarExtrato(Investidor usuarios[], int total);
void atualizarCotacao();
void carregarExtrato(Investidor *usuario);
int carregarUsuarios(Investidor usuarios[]);