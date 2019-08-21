#define NUM_OPS 8
#define MAX_FIELDS 30

typedef struct t_tabela Tabela;
typedef struct t_registro Reg;

const char *FUNCTIONS[NUM_OPS];
const int NUM_PARAMS[NUM_OPS];
int (*functions[NUM_OPS]) (char *result, int num_args, char * argv[]);

int interpreter(char *result, int num_args, char * argv[], int func_index);
int check_functions(char *op);

int token(char *argv, char *field[MAX_FIELDS]);
//Manipulação das tabelas
int CT(char *result, int num_args, char * argv[]);
int RT(char *result, int num_args, char * argv[]);
int AT(char *result, int num_args, char * argv[]);
int LT(char *result, int num_args, char * argv[]);

//Manipulaçao de resgistros nas tabelas
int IR(char *result, int num_args, char * argv[]);
int BR(char *result, int num_args, char * argv[]);
int AR(char *result, int num_args, char * argv[]);
int RR(char *result, int num_args, char * argv[]);
