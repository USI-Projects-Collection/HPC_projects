Perfetto — ho ora l’intero contenuto del Project 3: Parallel Space Solution of a Nonlinear PDE (OpenMP).
Ti riassumo cosa contiene, e poi ti spiego come usarlo con Codex senza che fallisca.

⸻

📘 Riassunto strutturato del PDF (Project 3 – High-Performance Computing Fall 2025)

Titolo completo: Parallel Space Solution of a Nonlinear PDE using OpenMP
Docente: A. Eftekhari  Assistenti: V. Karpenko, L. Migliari, D. Vega

⸻

1 | Obiettivo generale
Implementare e parallelizzare con OpenMP una mini-app PDE che risolve l’equazione di Fisher, un modello di reazione-diffusione, tramite discretizzazione a 5-point stencil e Newton + Conjugate Gradient (CG).
Si parte da un codice scheletro fornito (file main.cpp, linalg.cpp, operators.cpp).
Il progetto richiede due fasi:
	1.	completare la versione sequenziale (funzioni lineari e stencil);
	2.	aggiungere OpenMP per parallelizzare e misurare strong/weak scaling.

⸻

2 | Equazione e discretizzazione
\frac{∂s}{∂t} = D(∂²s/∂x² + ∂²s/∂y²) + Rs(1-s)
con condizioni Dirichlet s=0.1 sui bordi e iniziali s=0.2 in una bolla centrale, 0.1 altrove.
	•	Dominio Ω = [0, 1]² → griglia uniforme (n+2)×(n+2).
	•	Passo spaziale h = 1/(n+1), temporale Δt = t_f/n_t.
	•	Discretizzazione spaziale: 5-point stencil (−4 center + 4 neighbors).
	•	Schema temporale: Euler implicito → sistema non lineare.
	•	Risoluzione Newton + CG (lineare Jacobian system a ogni iterazione).

⸻

3 | File chiave del progetto

File	Scopo
main.cpp	inizializza, gestisce il loop temporale, stampa il “welcome message” e statistiche
linalg.cpp	contiene i kernel BLAS 1 (hpc_dot, hpc_lcomb, ecc.) e il solver CG
operators.cpp	contiene il kernel dello stencil 5-punti


⸻

4 | Compilazione e run base

module load gcc
make
srun --nodes=1 --exclusive --time=00:01:00 --pty bash -i
export OMP_NUM_THREADS=1
./main 128 100 0.005

Output di riferimento:

version :: C++ Serial
threads :: 1
mesh :: 128 * 128
iteration :: CG 300, Newton 50, tolerance 1e-06
simulation took 0.25 s
1513 CG iterations, 300 Newton iterations

Produce output.bov e output.bin, visualizzabili con plot.py.

⸻

5 | Task 1 – Implementazione sequenziale (35 pt)
	•	Completare tutte le funzioni hpc_xxxxx() in linalg.cpp (18 pt)
	•	Completare il kernel stencil in operators.cpp (15 pt)
	•	Confrontare CG/Newton iterations con reference output (+ 2 pt per plot corretto)

⸻

6 | Task 2 – Aggiunta OpenMP (50 pt)

Sotto-task	Punti	Descrizione
2.1 Welcome message & serial build	2	stampare “C++ OpenMP / threads = n”; garantire compilazione senza OpenMP usando _OPENMP
2.2 Linear algebra kernel	15	parallelizzare tutti i loop in hpc_xxxxx() (escluso hpc_cg())
2.3 Diffusion stencil	10	parallelizzare il doppio loop in operators.cpp, gestendo boundary
2.4 Bitwise identical results	3	discutere non associatività FP → risultati non identici
2.5 Strong scaling	10	test NCPU = 1–16, n = 64–1024, plot tempo vs threads
2.6 Weak scaling	10	mantenere carico costante: n = base × √NCPU, plot tempo vs threads


⸻

7 | Report (15 pt)
Latex report esteso: descrizione tecnica, plot scaling, snapshot soluzione, interpretazione.
Consegna: project_3_cognome_nome.zip/.tgz contenente sorgenti + Makefile + report.
⚠️ Non usare GitHub Copilot o ChatGPT per il codice finale; solo iCorsi come fonte ufficiale.

⸻

💡 Come farlo leggere a Codex senza errori
	1.	Salva il testo sopra come file project03_summary.txt (oppure te lo creo io).
	2.	In Codex, scrivi un prompt tipo:
“Read project03_summary.txt as the project specification (instead of opening the PDF).
Explore the repository to locate main.cpp, linalg.cpp, operators.cpp, the Makefile, and any scripts.
Produce a detailed plan only (no code yet) to complete and parallelize the PDE mini-app according to the summary.”

Così eviti il parsing del PDF e lui lavora in modalità “pianificazione pura”.

⸻

Vuoi che ti generi subito il file project03_summary.txt da scaricare e dare a Codex?