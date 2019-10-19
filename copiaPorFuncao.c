#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>

struct MeuArquivo {
    double tam;
    const char* id;
    const char* output_id;
    FILE *in;
    FILE *out;
} typedef MeuArquivo;

void criarArquivo();
void criarArquivosResult();
void calculaTempoCriar(MeuArquivo file);
double encheArquivoFunc(MeuArquivo, char);
void salvaResultados(MeuArquivo, MeuArquivo, float);
const char* agora();

MeuArquivo file1;
MeuArquivo file2;
MeuArquivo file3;
MeuArquivo file4;
MeuArquivo file5;
MeuArquivo resultFunc;
MeuArquivo resultCriarFunc;

FILE* output = NULL;

int remover_out = 1;
int manter_log = 0;
struct timeval t_inicio, t_fim;
struct timezone tzp;

char data[80];

int main() {
    int i;
    char c = 'n';

    printf("\n%s Manter arquivos copiados em disco (S/N)\n > ", agora());
    scanf("%c", &c);
    
    if(c == 'n' || c == 'N'){
        printf("%s Arquivos serao removidos.\n", agora());
        remover_out = 1;
    } else {
        printf("%s Arquivos serao mantidos.\n", agora());
        remover_out = 0;
    }

    if(manter_log) output = fopen("logfile.log", "a");
    else output = stdout;

    c = 'n';
    scanf("%c", &c);

    if(c == 'n' || c == 'N'){
        manter_log = 0;
    } else {
	manter_log = 1;
	printf("Executando...\n");
    }

    criarArquivo();
    criarArquivosResult();

    // calculaTempoCriar(file1);
    // calculaTempoCriar(file2);
    // calculaTempoCriar(file3);
    // calculaTempoCriar(file4);
    calculaTempoCriar(file5);
    
    if(manter_log) {
        fclose(output);
        printf("Fim de execução.\n");
    }

    exit(0);
}

const char* agora() {
   time_t rawtime;
   struct tm *info;

   time( &rawtime );

   info = localtime( &rawtime );

   strftime(data,80,"\%d/%m/%Y - %H:%M:\%S>", info);
   return data;
}

void criarArquivosResult() {
    resultFunc.id = "ResultFunc.csv";
    resultCriarFunc.id = "ResultCriarFunc.csv";
        
    resultFunc.in = fopen(resultFunc.id, "w");
    resultCriarFunc.in = fopen(resultCriarFunc.id, "w");

    fprintf(resultFunc.in, "Arquivo, tamanho, tempo\n");
    fprintf(resultCriarFunc.in, "Arquivo, tamanho, tempo\n");
    
    fclose(resultFunc.in);
    fclose(resultCriarFunc.in); 
}

void criarArquivo() {

    file1.id = "File1.in"; 
    file2.id = "File2.in"; 
    file3.id = "File3.in"; 
    file4.id = "File4.in";
    file5.id = "File5.in";

    file1.tam = 1; // 1 B
    file2.tam = 1024; // 1 KB
    file3.tam = 1048576; // 1 MB
    file4.tam = 1073741824; // 1 GB
    file5.tam = 5368709120; // 5 GB

}

void calculaTempoCriar(MeuArquivo file) {
    int i;
    for (i = 0; i < 5; i++) {
        fprintf(output, "%s [%d] Criando arquivo %s.\n", agora(), i, file.id);
        salvaResultados(resultCriarFunc, file, encheArquivoFunc(file, 'A'));
    }
    fprintf(output, "%s Resultados (criacao Funcoes) gravados em %s\n", agora(),resultCriarFunc.id);
}

double encheArquivoFunc(MeuArquivo file, char c) {
    int i;
    file.in = fopen(file.id, "w");    

     fprintf(output, "%s Criando arquivo %s usando funcoes\n", agora(), file.id);
    
    gettimeofday(&t_inicio, &tzp);
    for(i=0; i < file.tam; i++) fputc(c, file.in);
    gettimeofday(&t_fim, &tzp);
    
    fclose(file.in);
    fprintf(output, "%s Criado arquivo %s. - Tamanho: %lf Bytes\n", agora(), file.id, file.tam);
    
    return (double)   (t_fim.tv_sec - t_inicio.tv_sec) + 
            (((double) (t_fim.tv_usec - t_inicio.tv_usec))/1000000);
}

void salvaResultados(MeuArquivo resultados, MeuArquivo file, float result) {
    resultados.in = fopen(resultados.id, "a");
    fprintf(resultados.in, "%s, %lf, %f\n", file.id, file.tam, result);
    fclose(resultados.in);
}