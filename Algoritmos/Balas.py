import numpy as np
from scipy.optimize import linprog

class BalasAlgorithm:
    def __init__(self, c, A, b):
        self.c = -np.array(c)  # Convertir maximización a minimización para linprog
        self.A = np.array(A)
        self.b = np.array(b)
        self.best_solution = None
        self.best_value = -np.inf

    def solve(self):
        self.branch_and_bound([], [])
        return self.best_solution, self.best_value  # Aquí ya no invertimos el signo

    def branch_and_bound(self, fixed_zeros, fixed_ones):
        # Construir restricciones para las variables fijadas
        bounds = [(0, 1) for _ in range(len(self.c))]
        for i in fixed_zeros:
            bounds[i] = (0, 0)
        for i in fixed_ones:
            bounds[i] = (1, 1)

        # Resolver relajación lineal
        result = linprog(self.c, A_ub=self.A, b_ub=self.b, bounds=bounds, method='highs')
        if not result.success:
            return  # Subproblema no factible

        solution = result.x
        value = -result.fun  # Convertir de minimización a maximización

        # Poda: Si la cota superior es menor que la mejor solución actual
        if value <= self.best_value:
            return

        # Si la solución es entera y factible, actualizar mejor solución
        if np.all(np.abs(solution - solution.round()) < 1e-6) and np.all(np.dot(self.A, solution.round()) <= self.b):
            if value > self.best_value:
                self.best_value = value
                self.best_solution = solution.round()
            return

        # Elegir una variable no entera para ramificar
        for i in range(len(solution)):
            if not np.isclose(solution[i], solution[i].round()):
                break

        # Crear subproblemas: fijar x[i] = 0 y x[i] = 1
        self.branch_and_bound(fixed_zeros + [i], fixed_ones)
        self.branch_and_bound(fixed_zeros, fixed_ones + [i])

# Caso de prueba
# Maximizar: Z = x1 + x2
# Restricciones:
# x1 + x2 <= 1
# x1, x2 ∈ {0, 1}
c = [1, 1]
A = [[1, 1]]
b = [1]

balas = BalasAlgorithm(c, A, b)
solution, value = balas.solve()
print("Mejor solución:", solution)
print("Valor óptimo:", value)