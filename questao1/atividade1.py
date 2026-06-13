import time
import argparse

class BinPackingSolution:
    """Representação de uma solução para Bin Packing."""

    def __init__(self, bins=None):
        """
        bins: lista de bins, cada bin é uma lista de índices de itens.
        """
        self.bins = bins if bins is not None else []

    def copy(self):
        """Retorna uma cópia profunda da solução."""
        return BinPackingSolution([bin_.copy() for bin_ in self.bins])

    def num_bins(self):
        """Retorna o número de bins usados."""
        return len(self.bins)

    def waste(self, items):
        """Calcula o desperdício total (capacidade não utilizada)."""
        total_waste = 0
        for bin_ in self.bins:
            used = sum(items[i] for i in bin_)
            total_waste += (1.0 - used)
        return total_waste

    def fitness(self, items):
        """
        Função de avaliação: minimizar bins é prioritário, depois minimizar desperdício.
        Retorna tupla (num_bins, waste) para ordenação lexicográfica.
        """
        return (self.num_bins(), self.waste(items))


def is_feasible(solution, items):
    """Verifica se a solução é viável (nenhum bin ultrapassa capacidade 1)."""
    for bin_ in solution.bins:
        if sum(items[i] for i in bin_) > 1.0 + 1e-9:
            return False
    return True


def first_fit_decreasing(items):
    """
    Heurística construtiva: First Fit Decreasing.
    Retorna uma solução inicial para o Bin Packing.
    """
    indexed_items = [(items[i], i) for i in range(len(items))]
    indexed_items.sort(reverse=True)

    bins = []

    for size, idx in indexed_items:
        placed = False
        for bin_ in bins:
            bin_used = sum(items[i] for i in bin_)
            if bin_used + size <= 1.0 + 1e-9:
                bin_.append(idx)
                placed = True
                break

        if not placed:
            bins.append([idx])

    return BinPackingSolution(bins)


def get_neighbors(solution, items, max_neighbors=50):
    """
    Gera vizinhos da solução usando dois tipos de movimentos:
    1. Mover um item de um bin para outro
    2. Trocar dois itens entre bins diferentes
    """
    neighbors = []

    all_items = []
    for bin_idx, bin_ in enumerate(solution.bins):
        for item_idx in bin_:
            all_items.append((bin_idx, item_idx))

    for source_bin_idx, item_idx in all_items:
        for target_bin_idx in range(len(solution.bins)):
            if source_bin_idx == target_bin_idx:
                continue

            neighbor = solution.copy()
            neighbor.bins[source_bin_idx].remove(item_idx)

            target_used = sum(items[i] for i in neighbor.bins[target_bin_idx])
            if target_used + items[item_idx] <= 1.0 + 1e-9:
                neighbor.bins[target_bin_idx].append(item_idx)
                neighbors.append(neighbor)

    for i, (bin1_idx, item1_idx) in enumerate(all_items):
        for j, (bin2_idx, item2_idx) in enumerate(all_items):
            if i >= j or bin1_idx == bin2_idx:
                continue

            neighbor = solution.copy()

            bin1_without_item1 = sum(items[k] for k in neighbor.bins[bin1_idx] if k != item1_idx)
            bin2_without_item2 = sum(items[k] for k in neighbor.bins[bin2_idx] if k != item2_idx)

            if (bin1_without_item1 + items[item2_idx] <= 1.0 + 1e-9 and
                bin2_without_item2 + items[item1_idx] <= 1.0 + 1e-9):

                neighbor.bins[bin1_idx].remove(item1_idx)
                neighbor.bins[bin1_idx].append(item2_idx)
                neighbor.bins[bin2_idx].remove(item2_idx)
                neighbor.bins[bin2_idx].append(item1_idx)
                neighbors.append(neighbor)

    return neighbors[:max_neighbors]


def local_search(items, time_limit=60):
    """
    Meta-heurística de busca local com First Fit Decreasing como solução inicial.
    Utiliza busca em vizinhança com Best Improvement.
    """
    start_time = time.time()

    current_solution = first_fit_decreasing(items)
    best_solution = current_solution.copy()

    iteration = 0

    while time.time() - start_time < time_limit:
        neighbors = get_neighbors(current_solution, items)

        if not neighbors:
            break

        best_neighbor = min(neighbors, key=lambda sol: sol.fitness(items))
        best_neighbor_fitness = best_neighbor.fitness(items)
        current_fitness = current_solution.fitness(items)

        if best_neighbor_fitness < current_fitness:
            current_solution = best_neighbor

            if best_neighbor_fitness < best_solution.fitness(items):
                best_solution = best_neighbor.copy()
        else:
            break

        iteration += 1

    elapsed_time = time.time() - start_time
    return best_solution, elapsed_time, iteration


def solve(items, time_limit=60):
    """
    Utiliza busca local como heurística principal.
    """
    n = len(items)

    heuristic_solution, elapsed_time, iterations = local_search(items, time_limit)

    best_bins = heuristic_solution.num_bins()
    best_waste = heuristic_solution.waste(items)

    print(f"\n")
    print(f"Bins utilizados: {best_bins}")
    print(f"Desperdício total: {best_waste:.4f}")
    print(f"Tempo decorrido: {elapsed_time:.2f}s")
    print(f"Iterações: {iterations}")




def main():
    parser = argparse.ArgumentParser(description="Bin Packing com Busca Local")
    parser.add_argument("--items", type=str, help="Lista de tamanhos de itens (ex: '0.5,0.3,0.8')")
    parser.add_argument("--time", type=float, default=60, help="Limite de tempo em segundos (padrão: 60)")

    args = parser.parse_args()

    items = []
    if args.items:
        items = [float(x) for x in args.items.split(',')]
    else:
        items = [0.5, 0.3, 0.8, 0.2, 0.7, 0.4, 0.6, 0.1, 0.9, 0.25]

    print(f"Bin Packing")
    print(f"Número de itens: {len(items)}")
    print(f"Tamanhos: {items}")
    print(f"Limite de tempo: {args.time}s")

    solve(items, args.time)


if __name__ == "__main__":
    main()
