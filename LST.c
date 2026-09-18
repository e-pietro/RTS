#include <stdio.h>

#define MAX_N 26 
#define MAX_T 2048 

typedef struct {
    unsigned time_comp;
    unsigned period;
    unsigned deadline;
} task_t;

int main(void)
{
    unsigned n, t; // n = quantidade de tarfeas ; t = tempo de simulacao
    task_t tasks[MAX_N]; // guarda as tarfeas
    
    while (1)
    {
        // leitura
        if (scanf("%u%u", &n, &t) != 2) break;
        if (n == 0 || t == 0) break;

        for(int i = 0; i < n; i++)
            scanf("%u %u %u", &tasks[i].time_comp, &tasks[i].period, &tasks[i].deadline);

        // processamento
        char grade[MAX_T+1]; // diagrama de gantt
        unsigned    relogio = 0, // tempo atual da simulacao
                    n_pre   = 0, // contador de preemp
                    n_tcont = 0; // contador de troca
        
        unsigned tc_rest[MAX_N]; // tempo restante
        unsigned abs_deadline[MAX_N]; // prazo limite
        int pronta[MAX_N]; 

        for(int i = 0; i < n; i++)
        {
            tc_rest[i] = 0;
            abs_deadline[i] = 0;
            pronta[i] = 0;
        }

        int tarefa_atual = -1; // proc ocioso

        while (relogio < t)
        {
            //  atualizacao de chegadas no relogio atual
            for (int i = 0; i < n; i++)
            {
                if (relogio % tasks[i].period == 0)
                {
                    tc_rest[i] = tasks[i].time_comp;
                    abs_deadline[i] = relogio + tasks[i].deadline;
                    pronta[i] = 1;
                }
            }

            // busca da tarefa com menor folga
            int escolhida = -1;
            int menor_folga = 999999;

            for (int i = 0; i < n; i++)
            {
                if (pronta[i] && tc_rest[i] > 0)
                {
                    int folga = (int)abs_deadline[i] - (int)relogio - (int)tc_rest[i];

                    // criterio de menor folga (mantendo a menor ordem de ID em caso de empate)
                    if (folga < menor_folga)
                    {
                        menor_folga = folga;
                        escolhida = i;
                    }
                }
            }

            // contagem de troca de contexto e preempção
            if (escolhida != tarefa_atual)
            {
                if (tarefa_atual != -1)
                {
                    if (escolhida == -1)
                    {
                        // indo para idle: conta como preemp
                        // mesmo que a tarefa anterior tenha terminado naturalmente
                        n_pre++;
                    }
                    else if (tc_rest[tarefa_atual] > 0)
                    {
                        // troca entre tarefas: so conta preemp se a anterior
                        // ainda tinha trabalho restante
                        n_pre++;
                    }
                }

                n_tcont++;
                tarefa_atual = escolhida;
            }

            // exec de 1 unidade de tempo
            if (escolhida != -1)
            {
                grade[relogio] = 'A' + escolhida;
                tc_rest[escolhida]--;

                if (tc_rest[escolhida] == 0)
                {
                    pronta[escolhida] = 0;
                }
            } 
            else 
            {
                grade[relogio] = '.';
            }

            relogio++;
        }

        // caso a simulacao termine com uma tarefa ainda pela metade, conta preemp final
        if (relogio == t && tarefa_atual != -1 && tc_rest[tarefa_atual] > 0)
        {
            n_pre++;
        }

        grade[relogio] = '\0';

        printf("\n%s\n", grade);
        printf("%u %u\n\n", n_tcont, n_pre);
    }

    return 0;
}
