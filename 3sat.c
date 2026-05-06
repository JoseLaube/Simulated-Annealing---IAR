#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int quantClausulas = 430;
int quantTerm = 100;
double T_ini = 5.0;
double T_fim = 0.1;
double N = 500000;
double T = 5;

void inic_3sat(int clausulas[quantClausulas][3]){
        FILE *f = fopen("uf100-01.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }

    int a, b, c, d;

    for (int i = 0; i < quantClausulas; i++) {

        // lê os dados do arquivo
        if (fscanf(f, "%i %i %i %i", &a, &b, &c, &d) != 4) {
            printf("Erro ao ler linha %d\n", i);
            break;
        }
        clausulas[i][0] = a;
        clausulas[i][1] = b;
        clausulas[i][2] = c;
    }
}
// Só é usado na primera instância
void sort_termos(int pos_solu[quantTerm]){
    int aux = 0;
    for(int i =0; i < quantTerm; i++){
        aux = rand() % 2;
        pos_solu[i] = aux;
    }
}

void sort_vizinho(int vizinho[quantTerm]){
    int aux =0;
    for(int i =0; i < 2; i++){
        aux = rand() % quantTerm;
        if(vizinho[aux] == 0){
            vizinho[aux] = 1;
        }
        else{
            vizinho[aux] = 0;
        }
    }
}

int clausulas_aceitas(int clausulas[quantClausulas][3], int pos_solu[quantTerm]){
    int cont = 0;
    int a, b;
    for(int i = 0; i < quantClausulas; i++){
        int aux = 0;
        for(int j = 0; j < 3; j++){
            if(clausulas[i][j] < 0){
                a = abs(clausulas[i][j]);
                b = pos_solu[a-1];
                if(b == 0){
                    aux++;
                }
            }
            else{
                a = abs(clausulas[i][j]);
                aux += pos_solu[a-1];
            }
        } 
        if(aux > 0){
            cont++;
        }       
    }
    return cont;
}

int pegar(int delta, int iteracao){
    double prob = 0.0;
    double chance = 0.0;

    //Equação de temperatura
    /*Pegar forula 1 - com t inicial
    double t_atual = T_ini*(pow((T_fim/T_ini), ((double)iteracao/N)));
    chance = exp(delta/t_atual);
    prob = (float)rand() / RAND_MAX;
    if(prob < chance){
        return 1;
    }
    else{
        return 0;
    }
    return 0;*/
    //Pegar formula 2 - sem t inicial
    double t_atual = pow((1.0 - ((double)(iteracao/N))), T);
    printf("\n%.2f\n", t_atual);
    prob = (float)rand() / RAND_MAX;
    if(prob < t_atual){
        return 1;
    }
    else{
        return 0;
    }
    return 0;
}

void rotina(int pos_solu[quantTerm], int vizinhos[quantTerm], int clausulas[quantClausulas][3], int interacao){
    int a, b;
    int cont = 0;
    a = clausulas_aceitas(clausulas, pos_solu);
    b = clausulas_aceitas(clausulas, vizinhos);

    if (b > a){
        for(int i = 0; i < quantTerm; i++){
            pos_solu[i] = vizinhos[i];
        }
    }
    else{
        int pega = 0;
        /*do{
            pega = pegar((b - a), interacao);
            cont++;
            sort_vizinho(vizinhos);
            b = clausulas_aceitas(clausulas, vizinhos);
        } while(cont < 10 && pega == 0);*/
        pega = pegar((b - a), interacao);
        if(pega == 1){
            for(int i = 0; i < quantTerm; i++){
                pos_solu[i] = vizinhos[i];
            }    
        }
    }
}


int main(){
    int clausulas[quantClausulas][3];
    int pos_solu[quantTerm];
    int vizinhos[quantTerm];
    int melhor_solu[quantTerm];
    srand(time(NULL));

    FILE *f_log = fopen("convergencia.txt", "w");
    if (f_log == NULL) {
        printf("Erro ao criar arquivo de log!\n");
        return 1;
    }

    inic_3sat(clausulas);
    for(int i = 0; i< quantClausulas; i++){
        for(int j = 0; j < 3; j++){
            printf("%i ", clausulas[i][j]);
        }
        printf("\n");
    }
    sort_termos(pos_solu);
    for(int i = 0; i < quantTerm; i++){
        vizinhos[i] = pos_solu[i];
    }
    int maior_pop = 0;
    for(int k = 0; k < N; k++){
        int aux = 0;
        aux = clausulas_aceitas(clausulas, pos_solu);
        if(aux > maior_pop){
            maior_pop = aux;
            for(int i = 0; i< quantTerm; i++){
               melhor_solu[i] = pos_solu[i];
            }
        }
        sort_vizinho(vizinhos);
        rotina(pos_solu, vizinhos, clausulas, k);
        for(int i = 0; i < quantTerm; i++){
            vizinhos[i] = pos_solu[i];
        }    
        fprintf(f_log, "%d %d %d\n", k, aux, maior_pop);
    }
    printf("\n%i\n", maior_pop);
    for(int i = 0; i< quantTerm; i++){
        printf("%i ",melhor_solu[i]);
    }
    fclose(f_log);
    printf("\n");
    return 0;
}