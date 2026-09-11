import pandas as pd
import matplotlib.pyplot as plt
import os


csv_path = "../data/measurements/matrix_metrics.csv"
plots_dir = "../data/plots/"


os.makedirs(plots_dir, exist_ok=True)

try:

    df = pd.read_csv(csv_path)
except FileNotFoundError:
    print(f"Error: No se encontró el archivo {csv_path}. Ejecuta 'make run' primero.")
    exit()


df = df[df['time_ms'] >= 0]

colores = {'naive': '#e74c3c', 'strassen': '#3498db'}
marcadores = {'naive': 'o', 'strassen': 's'}


# plot1 (tiempo vs tamaño N)

df_avg_n = df.groupby(['n', 'algorithm'])['time_ms'].mean().unstack()

plt.figure(figsize=(10, 6))

for algo in df_avg_n.columns:
    plt.plot(
        df_avg_n.index, df_avg_n[algo],
        marker=marcadores.get(algo, 'x'),
        color=colores.get(algo, 'black'),
        linewidth=2,
        label=f'{algo.capitalize()}'
    )

plt.title("Rendimiento Asintótico: Naive vs Strassen", fontsize=14)
plt.xlabel("Dimensión de la Matriz (N)", fontsize=12)
plt.ylabel("Tiempo de Ejecución Promedio (ms)", fontsize=12)


plt.xscale('log', base=2)
plt.yscale('log', base=10)
plt.xticks(df_avg_n.index, df_avg_n.index)
plt.grid(True, which="both", ls="--", alpha=0.4)
plt.legend(fontsize=11)
plt.tight_layout()

file_plot1 = os.path.join(plots_dir, "1_matrix_growth_log.png")
plt.savefig(file_plot1, dpi=300)
plt.close()


# plot2 tipo matriz (dispersa, diagonal, densa)

max_n = df['n'].max()
df_max = df[df['n'] == max_n]
df_type = df_max.groupby(['t', 'algorithm'])['time_ms'].mean().unstack()

file_plot2 = None
if not df_type.empty:

    df_type.plot(kind='bar', logy=True, figsize=(10, 6), color=[colores.get(x, 'black') for x in df_type.columns])

    plt.title(f"Tiempo de Ejecución según Estructura de Matriz (N = {max_n})", fontsize=14)
    plt.xlabel("Tipo de Matriz", fontsize=12)

    plt.ylabel("Tiempo Promedio (ms) [Escala Log]", fontsize=12)
    plt.xticks(rotation=0)


    plt.grid(axis='y', which='both', linestyle='--', alpha=0.7)
    plt.legend(title="Algoritmo", fontsize=11)
    plt.tight_layout()

    file_plot2 = os.path.join(plots_dir, f"2_matrix_type_N{max_n}.png")
    plt.savefig(file_plot2, dpi=300)
    plt.close()


# plot3 (KB vs Tamaño N)

df_avg_mem = df.groupby(['n', 'algorithm'])['memory_kb'].mean().unstack()

plt.figure(figsize=(10, 6))

for algo in df_avg_mem.columns:
    plt.plot(
        df_avg_mem.index, df_avg_mem[algo],
        marker=marcadores.get(algo, 'x'),
        color=colores.get(algo, 'black'),
        linewidth=2,
        label=f'{algo.capitalize()}'
    )

plt.title("Uso de Memoria: Naive vs Strassen", fontsize=14)
plt.xlabel("Dimensión de la Matriz (N)", fontsize=12)
plt.ylabel("Memoria Promedio (KB)", fontsize=12)

plt.xscale('log', base=2)
plt.xticks(df_avg_mem.index, df_avg_mem.index)

plt.grid(True, which="both", ls="--", alpha=0.4)
plt.legend(fontsize=11)
plt.tight_layout()

file_plot3 = os.path.join(plots_dir, "3_matrix_memory.png")
plt.savefig(file_plot3, dpi=300)
plt.close()


# plot4 (dominio D0 vs D10)

df_domain = df_max.groupby(['d', 'algorithm'])['time_ms'].mean().unstack()

file_plot4 = None
if not df_domain.empty:
    df_domain.plot(kind='bar', logy=True, figsize=(10, 6), color=[colores.get(x, 'black') for x in df_domain.columns])
    plt.title(f"Impacto del Dominio Numérico (N = {max_n})", fontsize=14)
    plt.xlabel("Dominio de los Datos (d)", fontsize=12)
    plt.ylabel("Tiempo Promedio (ms) [Escala Log]", fontsize=12)
    plt.xticks(rotation=0)
    plt.grid(axis='y', which='both', linestyle='--', alpha=0.7)
    plt.legend(title="Algoritmo", fontsize=11)
    plt.tight_layout()

    file_plot4 = os.path.join(plots_dir, f"4_matrix_domain_N{max_n}.png")
    plt.savefig(file_plot4, dpi=300)
    plt.close()

print(f"Gráficos generados exitosamente en '{plots_dir}':")
print(f" - {file_plot1}")
if file_plot2:
    print(f" - {file_plot2}")
print(f" - {file_plot3}")
if file_plot4:
    print(f" - {file_plot4}")
