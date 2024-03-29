#define NUM_OPS 11
#define MAX_FIELDS 30

typedef struct t_tabela Tabela;
typedef struct t_registro Reg;

const char *FUNCTIONS[NUM_OPS];
const int NUM_PARAMS[NUM_OPS];
int (*functions[NUM_OPS]) (char *result, int num_args, char * argv[]);

int valid_Fields(char * argv, int num_Index);
int interpreter(char *result, int num_args, char * argv[], int func_index);
int check_functions(char *op);
void result_Fields(char argv[], char *result);
int token(char *argv, char *field[MAX_FIELDS], char *delim);



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

//Manuntenção de estruturas de indexação
int CI(char *result, int num_args, char * argv[]);
int RI(char *result, int num_args, char * argv[]);
int GI(char *result, int num_args, char * argv[]);

