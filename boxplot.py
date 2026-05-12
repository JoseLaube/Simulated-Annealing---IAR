import subprocess
import time
import re
import pandas as pd
import matplotlib.pyplot as plt

# ======================================
# CONFIGURAÇÃO
# ======================================

EXECUCOES = 20

ARQUIVO_C = "3sat.c"

# ======================================
# COMPILAR
# ======================================

executavel = ARQUIVO_C.replace(".c", "")

print("\nCompilando programa...\n")

resultado = subprocess.run(
    [
        "gcc",
        ARQUIVO_C,
        "-o",
        executavel,
        "-lm"
    ],
    capture_output=True,
    text=True
)

if resultado.returncode != 0:

    print("Erro ao compilar:\n")
    print(resultado.stderr)

    exit()

print("Compilação concluída.\n")

# ======================================
# LISTAS
# ======================================

fitnesses = []

tempos = []

# ======================================
# EXECUÇÕES
# ======================================

for i in range(EXECUCOES):

    print(f"Execução {i+1}/{EXECUCOES}")

    inicio = time.perf_counter()

    processo = subprocess.run(
        [f"./{executavel}"],
        capture_output=True,
        text=True
    )

    fim = time.perf_counter()

    tempo_total = fim - inicio

    saida = processo.stdout

    # ======================================
    # PEGA ÚLTIMO NÚMERO INTEIRO
    # ======================================

    linhas = saida.strip().split("\n")

    fitness = None

    for linha in reversed(linhas):

        linha = linha.strip()

        # procura linha contendo apenas número inteiro
        if re.fullmatch(r"-?\d+", linha):

            fitness = int(linha)

            break

    if fitness is None:

        print("Erro ao capturar fitness.")
        fitness = 0

    fitnesses.append(fitness)

    tempos.append(tempo_total)

    print(f"Melhor solução: {fitness}")
    print(f"Tempo: {tempo_total:.4f} s\n")

# ======================================
# SALVAR CSV
# ======================================

df = pd.DataFrame({

    "fitness": fitnesses,
    "tempo": tempos

})

df.to_csv(
    "resultados_annealing.csv",
    index=False
)

print("\nCSV salvo com sucesso.\n")

# ======================================
# BOXPLOT FITNESS
# ======================================

plt.figure(figsize=(8, 6))

plt.boxplot(df["fitness"])

plt.title("Boxplot Fitness - Simulated Annealing 3SAT")

plt.ylabel("Cláusulas Satisfeitas")

plt.grid(True)

plt.savefig("boxplot_fitness_annealing.png")

plt.close()

# ======================================
# BOXPLOT TEMPO
# ======================================

plt.figure(figsize=(8, 6))

plt.boxplot(df["tempo"])

plt.title("Boxplot Tempo - Simulated Annealing 3SAT")

plt.ylabel("Tempo (s)")

plt.grid(True)

plt.savefig("boxplot_tempo_annealing.png")

plt.close()

# ======================================
# ESTATÍSTICAS
# ======================================

print("====================================")
print("ESTATÍSTICAS")
print("====================================\n")

print(df.describe())

print("\nBoxplots gerados com sucesso.")