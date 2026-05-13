    #include <stdio.h>
    #include <math.h>
    #include <time.h>

    #define MAX_CIDADES 100

    typedef struct {
        int id;
        double x, y;
    } Cidade;

    int n;
    Cidade cidades[MAX_CIDADES];
    double dist[MAX_CIDADES][MAX_CIDADES];
    int melhorRota[MAX_CIDADES];
    double melhorCusto;

    double distancia(Cidade a, Cidade b) {
        double dx = a.x - b.x;
        double dy = a.y - b.y;
        return sqrt(dx*dx + dy*dy);
    }

    void montarMatriz() {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                dist[i][j] = distancia(cidades[i], cidades[j]);
    }

    double calcularCusto(int rota[]) {
        double custo = 0;
        for (int i = 0; i < n - 1; i++)
            custo += dist[rota[i]][rota[i+1]];
        custo += dist[rota[n-1]][rota[0]];
        return custo;
    }

    void imprimirRota(int rota[]) {
        printf("Rota: ");
        for (int i = 0; i < n; i++)
            printf("%d -> ", rota[i]);
        printf("%d\n", rota[0]);
        printf("Custo: %.2f\n", calcularCusto(rota));
    }

    void swap(int rota[], int i, int j) {
        int temp = rota[i];
        rota[i] = rota[j];
        rota[j] = temp;
    }

    void permutacao(int rota[], int inicio) {
        if (inicio == n) {
            double custo = calcularCusto(rota);
            if (custo < melhorCusto) {
                melhorCusto = custo;
                for (int i = 0; i < n; i++)
                    melhorRota[i] = rota[i];
            }
            return;
        }

        for (int i = inicio; i < n; i++) {
            swap(rota, inicio, i);
            permutacao(rota, inicio + 1);
            swap(rota, inicio, i);
        }
    }

    int main() {
        // printf("Numero de cidades: ");
        scanf("%d", &n);

        clock_t inicio, fim;

        for (int i = 0; i < n; i++) {
            cidades[i].id = i;
            // printf("Cidade %d (x y): ", i);
            scanf("%lf %lf", &cidades[i].x, &cidades[i].y);
        }

        montarMatriz();

        int rota[MAX_CIDADES];
        for (int i = 0; i < n; i++)
            rota[i] = i;

        melhorCusto = 1e18;
        
        inicio = clock();
        permutacao(rota, 1);
        fim = clock();

        double tempo = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

        printf("\n--- FORCA BRUTA ---\n");
        imprimirRota(melhorRota);
        printf("Tempo de execucao: %f segundos\n", tempo);

        return 0;
    }