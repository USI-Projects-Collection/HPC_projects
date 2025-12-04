from mpi4py import MPI
import sys

# Inizializzazione MPI
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

# ---------------------------------------------------------
# 1. Compute dims (equivalente a MPI_Dims_create)
# ---------------------------------------------------------
# Vogliamo dividere i processi in una griglia 2D.
# Se size=16, dims diventerà [4, 4].
dims = MPI.Compute_dims(size, 2)

# ---------------------------------------------------------
# 2. Create Cartesian Topology
# ---------------------------------------------------------
# periods=[True, True] indica che la griglia è periodica in entrambe le direzioni (torus).
# reorder=False mantiene i rank originali se possibile
cart_comm = comm.Create_cart(dims, periods=[True, True], reorder=False)

# Otteniamo le coordinate cartesiane di questo processo (es. [row, col])
coords = cart_comm.Get_coords(rank)

# ---------------------------------------------------------
# 3. Determine neighboring processes
# ---------------------------------------------------------
# MPI_Cart_shift restituisce (source, dest) per uno spostamento di +1.
# source: il vicino nella direzione negativa ("da dove ricevo")
# dest:   il vicino nella direzione positiva ("a dove spedisco")

# Asse 0 (Y / Righe):
# Source = Nord (Row - 1), Dest = Sud (Row + 1)
nbr_north, nbr_south = cart_comm.Shift(direction=0, disp=1)

# Asse 1 (X / Colonne):
# Source = Ovest (Col - 1), Dest = Est (Col + 1)
nbr_west, nbr_east = cart_comm.Shift(direction=1, disp=1)

# ---------------------------------------------------------
# 4. Output Topology
# ---------------------------------------------------------
# Prepariamo una stringa con le informazioni topologiche
topo_info = (f"Rank {rank:02d} | Coords {coords} | "
                f"Neighbors (N, S, W, E): {nbr_north:02d}, {nbr_south:02d}, {nbr_west:02d}, {nbr_east:02d}")

# ---------------------------------------------------------
# 5. Exchange Ranks (Verifica Comunicazione)
# ---------------------------------------------------------
# Scambiamo il nostro rank con i 4 vicini per verificare la connessione.
# Usiamo sendrecv per evitare deadlock

# SCAMBIO ASSE VERTICALE
# Invio al SUD, Ricevo dal NORD
recv_from_north = cart_comm.sendrecv(sendobj=rank, dest=nbr_south, source=nbr_north)
# Invio al NORD, Ricevo dal SUD
recv_from_south = cart_comm.sendrecv(sendobj=rank, dest=nbr_north, source=nbr_south)

# SCAMBIO ASSE ORIZZONTALE
# Invio all'EST, Ricevo dall'OVEST
recv_from_west = cart_comm.sendrecv(sendobj=rank, dest=nbr_east, source=nbr_west)
# Invio all'OVEST, Ricevo dall'EST
recv_from_east = cart_comm.sendrecv(sendobj=rank, dest=nbr_west, source=nbr_east)

# ---------------------------------------------------------
# Raccolta e Stampa ordinata (solo per il report)
# ---------------------------------------------------------
# Raccogliamo tutto sul rank 0 per stampare in ordine e non accavallare l'output
all_topo = comm.gather(topo_info, root=0)

if rank == 0:
    for line in sorted(all_topo):
        print(line)