import subprocess
import matplotlib.pyplot as plt
import os

# 1. Configurações
NOME_ARQUIVO_C = "3sat.c"  # Troque pelo nome do seu arquivo .c
EXECUTAVEL = "./simulated_annealing"
ARQUIVO_LOG = "convergencia.txt"

def rodar_sa():
    print("Compilando código C...")
    # Compila o código C (incluindo a biblioteca math -lm)
    compile_proc = subprocess.run(["gcc", NOME_ARQUIVO_C, "-o", EXECUTAVEL, "-lm"])
    
    if compile_proc.returncode != 0:
        print("Erro na compilação!")
        return False

    print("Executando Simulated Annealing...")
    subprocess.run([EXECUTAVEL])
    return True

def plotar_grafico():
    iteracoes = []
    energia_atual = []
    melhor_global = []

    if not os.path.exists(ARQUIVO_LOG):
        print("Arquivo de log não encontrado!")
        return

    # Lendo o arquivo gerado pelo C
    with open(ARQUIVO_LOG, "r") as f:
        for linha in f:
            it, atual, melhor = map(int, linha.split())
            iteracoes.append(it)
            energia_atual.append(atual)
            melhor_global.append(melhor)

    # Criando o gráfico
    plt.figure(figsize=(12, 6))
    
    # Plot da solução atual (o "caminho" do algoritmo)
    plt.plot(iteracoes, energia_atual, color='skyblue', alpha=0.6, label='Solução Atual (Oscilação)')
    
    # Plot da melhor solução (a curva de convergência)
    plt.plot(iteracoes, melhor_global, color='red', linewidth=2, label='Melhor Global Encontrada')

    plt.title('Convergência do Simulated Annealing (SAT-3)')
    plt.xlabel('Iteração')
    plt.ylabel('Cláusulas Aceitas (F(o))')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.7)
    
    print("Salvando gráfico em 'convergencia.png'...")
    plt.savefig("convergencia.png")
    plt.show()

if __name__ == "__main__":
    if rodar_sa():
        plotar_grafico()