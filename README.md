Integrantes:
- Carlos Tomás Álvarez Norambuena
- Antonio Jesus Benavides Puentes

Para compilar use el siguiente comando en la raiz del proyecto:

```bash
make
```

> [!IMPORTANT]
> Antes de ejecutar el codigo procure que existan los .graph que se van a evaluar en la carpeta `/grafos`.

Para ejecutar el algoritmo Greedy use:

```bash
./Greedy.out -i <ruta del archivo .graph>
```

Para ejecutar el algoritmo Greedy Probabilista use:

```bash
/Greedy-probabilista.out -i <ruta del archivo .graph> -l <largo de la lista de seleccion> -a <alpha>
```
Alpha es la probabilidad de explorar o explotar.

> [!NOTE]
> Si el `largo de la lista de seleccion` se coloca como -1 entonces se interpretara internamente como si fuera infinito.

> [!NOTE]
> Para realizar los experimentos automáticos, debes ejecutar los scripts desde la carpeta raíz del proyecto (no desde la subcarpeta `experiments`).

Para ejecutar el algoritmo Taboo Search use:

```bash
./Taboo.out -i <ruta del archivo .graph> -t <tiempo maximo de ejecucion> -L <largo de la lista tabu>
```

Para eliminar los archivos compilados e intermedios(.o), ejecute el siguiente comando:
```bash
make clean
```

> **Nota:** Si desea replicar los experimentos, consulte las instrucciones detalladas en [`experiments/README.md`](experiments/README.md).

