#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int quantClausulas = 91;
int quantTerm = 20;

void inic_3sat(int clausulas[quantClausulas][3]){
        FILE *f = fopen("uf20-01.txt", "r");
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
    for(int i =0; i < 3; i++){
        aux = rand() % 20;
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

void rotina(int pos_solu[quantTerm], int vizinhos[quantTerm], int clausulas[quantClausulas][3]){
    int a, b;
    a = clausulas_aceitas(clausulas, pos_solu);
    b = clausulas_aceitas(clausulas, vizinhos);

    if (b > a){
        for(int i = 0; i < quantTerm; i++){
            pos_solu[i] = vizinhos[i];
        }
    }
    //Se for falso tem que analisar a temperatura;
}


int main(){
    int clausulas[quantClausulas][3];
    int pos_solu[quantTerm];
    int vizinhos[quantTerm];
    int melhor_solu[quantTerm];
    srand(time(NULL));

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
    for(int k = 0; k < 50000; k++){
        int aux = 0;
        aux = clausulas_aceitas(clausulas, pos_solu);
        if(aux > maior_pop){
            maior_pop = aux;
            for(int i = 0; i< quantTerm; i++){
               melhor_solu[i] = pos_solu[i];
            }
        }
        rotina(pos_solu, vizinhos, clausulas);
        sort_vizinho(vizinhos);
    }
    printf("\n%i\n", maior_pop);
    for(int i = 0; i< quantTerm; i++){
        printf("%i ",melhor_solu[i]);
    }
    printf("\n");
    return 0;
}