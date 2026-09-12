import pandas as pd
import matplotlib.pyplot as plt
import os


csv_path = "../data/measurements/sorting_metrics.csv"
plots_dir = "../data/plots/"

os.makedirs(plots_dir, exist_ok=True)

try:
    df = pd.read_csv(csv_path)
except FileNotFoundError:
    print(f"Error: No se encontró el archivo {csv_path}. Ejecuta el benchmark primero.")
    exit()

df = df[df['time_ms'] >= 0]


colores = {
    'mergesort': '#1f77b4',
    'quicksort': '#ff7f0e',
    'patiencesort': '#2ca02c',
    'stdsort': '#d62728'
}
marcadores = {'mergesort': 'o', 'quicksort': 's', 'patiencesort': '^', 'stdsort': 'D'}


# plot1 (tiempo vs N)

df_n = df.groupby(['n', 'algorithm'])['time_ms'].mean().unstack()

plt.figure(figsize=(10, 6))
for algo in df_n.columns:
    plt.plot(df_n.index, df_n[algo],
             marker=marcadores.get(algo, 'x'),
             color=colores.get(algo, 'black'),
             linewidth=2, label=algo)

plt.title("Rendimiento Asintótico: Algoritmos de Ordenamiento", fontsize=14)
plt.xlabel("Tamaño del Arreglo (N)", fontsize=12)
plt.ylabel("Tiempo Promedio (ms)", fontsize=12)


plt.xscale('log', base=10)
plt.yscale('log', base=10)
plt.grid(True, which="both", ls="--", alpha=0.4)
plt.legend(title="Algoritmo", fontsize=11)
plt.tight_layout()

file_plot1 = os.path.join(plots_dir, "1_sorting_growth_log.png")
plt.savefig(file_plot1, dpi=300)
plt.close()


# plot2 impacto orden inicial (Ascendente, Descendente, Aleatorio)

max_n = df['n'].max()
df_t = df[df['n'] == max_n].groupby(['t', 'algorithm'])['time_ms'].mean().unstack()

file_plot2 = None
if not df_t.empty:
    df_t.plot(kind='bar', logy=True, figsize=(10, 6), color=[colores.get(x, 'black') for x in df_t.columns])
    plt.title(f"Impacto de la Distribución Inicial (N = {max_n})", fontsize=14)
    plt.xlabel("Estado Inicial del Arreglo (t)", fontsize=12)
    plt.ylabel("Tiempo Promedio (ms) [Escala Log]", fontsize=12)
    plt.xticks(rotation=0)
    plt.grid(axis='y', which='both', linestyle='--', alpha=0.7)
    plt.legend(title="Algoritmo", fontsize=11)
    plt.tight_layout()

    file_plot2 = os.path.join(plots_dir, f"2_sorting_type_N{max_n}.png")
    plt.savefig(file_plot2, dpi=300)
    plt.close()


# plot3 (D1 vs D7)

df_d = df[df['n'] == max_n].groupby(['d', 'algorithm'])['time_ms'].mean().unstack()

file_plot3 = None
if not df_d.empty:
    df_d.plot(kind='bar', logy=True, figsize=(10, 6), color=[colores.get(x, 'black') for x in df_d.columns])
    plt.title(f"Impacto del Dominio Numérico (N = {max_n})", fontsize=14)
    plt.xlabel("Dominio de los Datos (d)", fontsize=12)
    plt.ylabel("Tiempo Promedio (ms) [Escala Log]", fontsize=12)
    plt.xticks(rotation=0)
    plt.grid(axis='y', which='both', linestyle='--', alpha=0.7)
    plt.legend(title="Algoritmo", fontsize=11)
    plt.tight_layout()

    file_plot3 = os.path.join(plots_dir, f"3_sorting_domain_N{max_n}.png")
    plt.savefig(file_plot3, dpi=300)
    plt.close()


# plot4 (memoria vs N)

df_mem = df.groupby(['n', 'algorithm'])['memory_kb'].mean().unstack()

plt.figure(figsize=(10, 6))
for algo in df_mem.columns:
    plt.plot(df_mem.index, df_mem[algo],
             marker=marcadores.get(algo, 'x'),
             color=colores.get(algo, 'black'),
             linewidth=2, label=algo)

plt.title("Uso de Memoria: Algoritmos de Ordenamiento", fontsize=14)
plt.xlabel("Tamaño del Arreglo (N)", fontsize=12)
plt.ylabel("Memoria Promedio (KB)", fontsize=12)

plt.xscale('log', base=10)
plt.grid(True, which="both", ls="--", alpha=0.4)
plt.legend(title="Algoritmo", fontsize=11)
plt.tight_layout()

file_plot4 = os.path.join(plots_dir, "4_sorting_memory.png")
plt.savefig(file_plot4, dpi=300)
plt.close()

print(f"Gráficos generados exitosamente en '{plots_dir}':")
print(f" - {file_plot1}")
if file_plot2:
    print(f" - {file_plot2}")
if file_plot3:
    print(f" - {file_plot3}")
print(f" - {file_plot4}")
