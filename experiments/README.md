# Ejecución de Experimentos

En esta carpeta se encuentran todos los **lanzadores de experimentos** del proyecto.  
Cada experimento evalúa diferentes estrategias y algoritmos utilizados para resolver el problema planteado.  

Todos los resultados se almacenan en la carpeta `results/` en formato **.csv**, y cada archivo se registra con el **nombre correspondiente al experimento ejecutado**.

## Estructura general

```

experiments/
├── experiments_greedy.sh
├── experiments_probabilistic.sh
├── experiments_taboo/
│   ├── Cargo.toml
│   ├── Cargo.lock
│   └── src/
│       └── main.rs
├── Makefile
├── results/
│   ├── experiments_taboo.csv
│   └── readme.md
└── README

````

---

## 1. Experimento Greedy

**Archivo:** `experiments_greedy.sh`

Este lanzador ejecuta el enfoque *Greedy* (avaro) del proyecto.  
Para ejecutarlo:

```bash
./experiments_greedy.sh
````

Los resultados se guardarán en `results/` con el nombre `experiments_greedy.csv`.

---

## 2. Experimento Probabilístico

**Archivo:** `experiments_probabilistic.sh`

Este lanzador ejecuta el enfoque probabilístico, que incluye componentes aleatorios en la búsqueda de soluciones.
Para ejecutarlo:

```bash
./experiments_probabilistic.sh
```

Los resultados se guardarán en `results/` con el nombre `experiments_probabilistic.csv`.

---

## 3. Experimento Tabú (Rust)

**Carpeta:** `experiments_taboo/`

A diferencia de los anteriores, este lanzador está implementado en **Rust**, aprovechando su rendimiento, control de concurrencia y manejo eficiente de paralelismo.

Antes de ejecutarlo, asegúrate de tener instalado **Rust** y **Cargo**.
Puedes instalarlo con el siguiente comando:

```bash
curl https://sh.rustup.rs -sSf | sh
```

Para compilar y ejecutar el experimento **no es necesario entrar** en la carpeta `experiments_taboo/`.
Simplemente ejecuta desde `experiments/`:

```bash
make run
```

Esto compilará y ejecutará el lanzador de Rust, generando múltiples procesos en paralelo (por defecto, hasta 4 simultáneamente) y guardando los resultados en:

```
results/experiments_taboo.csv
```

---

## Notas finales

* Todos los scripts y programas están configurados para ejecutarse desde la carpeta `experiments/`.
* No es necesario modificar rutas internas ni acceder manualmente a las subcarpetas.
* Los resultados se sobrescribirán en caso de repetir un experimento con el mismo nombre de salida.

