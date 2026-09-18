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
    unsigned n, t;
    task_t tasks[MAX_N];
    
    while (1)
    {
        if (scanf("%u%u", &n, &t) != 2) break;
        if (n == 0 || t == 0) break;

        for(int i = 0; i < n; i++)
            scanf("%u %u %u", &tasks[i].time_comp, &tasks[i].period, &tasks[i].deadline);
        
        char grade[MAX_T+1];
        unsigned relogio = 0, n_pre = 0, n_tcont = 0;
        
        unsigned tc_rest[MAX_N];
        unsigned abs_deadline[MAX_N];
        int pronta[MAX_N];

        for(int i = 0; i < n; i++)
        {
            tc_rest[i] = 0;
            abs_deadline[i] = 0;
            pronta[i] = 0;
        }

        int tarefa_atual = -1;

        while (relogio < t)
        {
            // 1. Atualização de chegadas no relógio atual
            for (int i = 0; i < n; i++)
            {
                if (relogio % tasks[i].period == 0)
                {
                    tc_rest[i] = tasks[i].time_comp;
                    abs_deadline[i] = relogio + tasks[i].deadline;
                    pronta[i] = 1;
                }
            }

            // 2. Busca da tarefa de menor folga
            int escolhida = -1;
            int menor_folga = 999999;

            for (int i = 0; i < n; i++)
            {
                if (pronta[i] && tc_rest[i] > 0)
                {
                    int folga = (int)abs_deadline[i] - (int)relogio - (int)tc_rest[i];

                    // Critério de menor folga (mantendo a menor ordem de ID em caso de empate)
                    if (folga < menor_folga)
                    {
                        menor_folga = folga;
                        escolhida = i;
                    }
                }
            }

            // 3. Contagem de troca de contexto e preempção
            if (escolhida != tarefa_atual)
            {
                if (tarefa_atual != -1)
                {
                    if (escolhida == -1)
                    {
                        // indo para idle: conta como preempção sempre,
                        // mesmo que a tarefa anterior tenha terminado naturalmente
                        n_pre++;
                    }
                    else if (tc_rest[tarefa_atual] > 0)
                    {
                        // troca entre tarefas: só conta preempção se a anterior
                        // ainda tinha trabalho restante
                        n_pre++;
                    }
                }

                n_tcont++;
                tarefa_atual = escolhida;
            }

            // 4. Execução de 1 unidade de tempo
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

        // Caso a simulação termine com uma tarefa ainda pela metade, conta preempção final
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
