# KA PROIEKTUA

## KA TXOSTENA

[KA Txostena](https://upvehueus-my.sharepoint.com/:w:/g/personal/jpacios002_ikasle_ehu_eus/IQB8fMWIXovgTpnxzytk1oWiAZDfN-vMiu1s_zVMRgDLeMk?e=wP4ptw)


## Proiektuaren deskribapena

Proiektu honek hitzen multzokatze semantikoa eta UNESCOk definitutako zientzia eta teknologiaren alorrekin duen harremana aztertzen du, clustering algoritmoak eta medianen kalkuluak erabiliz.

## Exekuzio eta konpilazio instrukzioak

```bash
cd compile

./compExekS.sh      # Serieko exekuzioa

./compExekP.sh      # Paralelo exekuzioa
```

# **Hitz Multzokatzea**

## **Proiektuaren Deskribapena**

Proiektu honen helburua **200.000 hitz-bektore** (Word embeddings, GloVe eredua) prozesatzea da, K-means algoritmoa erabiliz hitzen antzekotasunaren arabera multzokatzea. Programa C lengoaian inplementatu da eta **OpenMP** erabiliz paralelizatu da.

## **Egiturra**

```
3-proiektua/
├── src/
│   ├── include/           # Goiburu-fitxategiak (.h)
│   ├── serie/             # Serieko kodea (.c)
│   └── paralelo/          # Paraleloko kodea (.c)
├── compile/               # Konpilazio script-ak
├── emaitzak/              # Exekuzio-emaitzak
└── README.md
```

## **Paralelizazio Estrategia**

### **Paralelizatutako Funtzioak:**

1. **`multzo_gertuena`** - Hitz bakoitza zentroiderik gertuenarekin lotzen du
   ```c
   #pragma omp parallel for private(i, j, dist, min_dist, gertuena) schedule(static)
   ```

2. **`balidazioa`** - CVI indizea kalkulatzen du
   ```c
   #pragma omp parallel for schedule(dynamic, 1) private(k, i, j, ind_x) reduction(+:batura)
   ```

3. **`ztalorren_analisia`** - Zientzia-alorrak analizatzen ditu
   ```c
   #pragma omp parallel private (k, i, n, mediana) num_threads(48)
   #pragma omp for schedule(dynamic, 1)
   #pragma omp critical
   ```

4. **`medoideak_kalkulatu`** - Zentroide berriak kalkulatzen ditu
   ```c
   #pragma omp parallel for private(i, j, k, talde_tam, i1, i2, dist, dist_min, pos_min) schedule(dynamic, 1)
   ```

### **Paralelizatu Gabe Utzitakoak:**
- `hitzen_distantzia` - *Overhead* handiegia
- `hasierako_zentroideak` - Denbora laburregia
- `zentroide_berriak` - `medoideak_kalkulatu`-ren barruan deitua

## **Errendimendua**

### **Denbora-konparaketa:**
| Fasea          | Seriea  | Paraleloa (8 hari) | Azelerazioa |
|----------------|---------|-------------------|-------------|
| Sailkatzea     | 676 s   | 109 s             | 6.2×        |
| Balidazioa     | 465 s   | 63 s              | 7.3×        |
| Medoideak      | 45 s    | 7 s               | 6.4×        |
| Ztalor         | 9 s     | 2 s               | 4.5×        |
| **GUZTIRA**    | **736 s** | **109 s**       | **6.7×**    |

## **Nola Exekutatu**

**Konpilatuzailea hasieratu:**
    ```bash

    cd compile/
    ./compExekS.sh      # Serieko exekuzioa

    ./compExekP.sh      # Paralelo exekuzioa
   ```

## **Oinarrizko Komandoak**

```bash
# Konpilazioa
gcc -o programa_nagusia -fopenmp *.c

# Exekuzioa
OMP_NUM_THREADS=8 ./programa_nagusia
```

## **Egileak**
   - Thomas Mugica
   - Oier Villanueva
   - Jon Pacios
