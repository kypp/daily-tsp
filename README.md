# daily-tsp

Opis problemu:
Mamy dany graf z wagami na krawędziach.
Chcemy znaleźć cykl o minimalnej sumie długości dróg.
Ponadto obowiązuje podział na dni - jednego dnia można przebyć co najwyżej M.
Chcemy, żeby każdego dnia przejechać drogę podobnej długości.


Kryterium minimalizacji:
Koszt = C^2+\sigma_i(M - d_i)^2
gdzie:
C - suma wag w cyklu
M - maksymalna odległość do przebycia w jeden dzień
d_i - odległość przebyta w dniu i-tym


Ogólna struktura algorytmu:
Dla danego grafu G:
1. Usuwamy krawędzie niedopuszczalne (powyżej M)
2. Tworzymy graf pełny G' taki, że waga krawędzi z a do b w G' jest równa najkrótszej ścieżce z a do b w G
3. Rozwiązujemy zwyczajny TSP na G'
4. Uzupełniamy miasta na ścieżce z G
5. Na wybranej ścieżce analizujemy możliwe podziały na dni
6. Zwracamy ścieżkę z podziałem

Fazy optymalizacji:
3 (znalezienie możliwie optymalnego cyklu)
5 (znalezienie możliwie optymalnego podziału na dni)

Bardziej pseudokodowo:

    solve_tsp_daily(G, M, tsp_solver):
      1. preprocessing(G, M) -> G'
      2. solve(G', tsp_solver) -> R'
      3. reconstruct(G, R') -> R
      4. divide(R, M) -> D