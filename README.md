# Sortowanie pliku sekwencyjnego metodą scalania z użyciem wielkich buforów

Projekt z przedmiotu **Struktury baz danych**. Program sortuje wektory zapisane w pliku binarnym według ich długości. Dane są przetwarzane partiami, dzięki czemu rozmiar zbioru może przekraczać pojemność buforów w pamięci operacyjnej.

W eksperymencie dla **10 mln rekordów** zwiększenie liczby buforów z **11 do 1001** zmniejszyło liczbę faz scalania z **5 do 1**, a liczbę odczytów i zapisów stron z **12 mln do 4 mln**. Każdy bufor mieścił 10 rekordów.

## Wstęp

Tematyka projektu porusza problem sortowania zbiorów rekordów zbyt dużych, aby mogły zmieścić się w pamięci głównej. W celu sortowania tak dużych zbiorów stosowane są metody sortowania zewnętrznego (ang. *external sorting*).

Jedną z metod sortowania zewnętrznego jest sortowanie metodą scalania z użyciem wielkich buforów (ang. *merging with large buffers*).

## Przygotowanie danych

Program `SetUp` pozwala ręcznie wprowadzić składowe wektorów lub wygenerować je losowo za pomocą generatora Mersenne Twister. Konfigurację opisują cztery parametry:

| Parametr | Znaczenie |
|---|---|
| `N` | Liczba rekordów w pliku wejściowym. |
| `n` | Liczba buforów dostępnych w pamięci. |
| `b` | Liczba rekordów mieszczących się w jednym buforze i na jednej stronie dyskowej. |
| `d` | Liczba miejsc na składowe wektora w rekordzie. Składowe mogą być zerowe. |

Rekord zawiera `d + 1` wartości typu `int`. Pierwsza to klucz odpowiadający długości euklidesowej wektora, a pozostałe to jego składowe. Program sortuje rekordy rosnąco według zapisanego klucza. W plikach tekstowych każdy wiersz ma postać `klucz | składowa₁ … składowa_d`.

## Jak działa sortowanie

**1. Tworzenie posortowanych przebiegów.** Program `MergingWithLargeBuffers` wczytuje do pamięci najwyżej `n × b` rekordów z pliku sekwencyjnego, sortuje je algorytmem QuickSort i zapisuje jako jeden *przebieg*, czyli uporządkowany fragment pliku. Powtarza tę operację do wyczerpania wejścia. Jeśli wszystkie rekordy zmieszczą się w pamięci, plik jest już posortowany.

**2. Scalanie przebiegów.** Program przeznacza jeden bufor na wynik, a pozostałe `n − 1` na dane wejściowe, po jednym na przebieg. Kopiec przechowuje po jednym kandydacie z każdego aktywnego przebiegu. Najmniejszy rekord trafia do bufora wyjściowego, a jego miejsce w kopcu zajmuje następny rekord z tego samego przebiegu. Opróżniony bufor wejściowy jest uzupełniany kolejną stroną. Pełny bufor wyjściowy jest zapisywany na dysk, podobnie jak ostatnia niepełna strona po scaleniu grupy.

Jedna **faza scalania** obejmuje przetworzenie wszystkich grup przebiegów. Każda grupa daje jeden dłuższy przebieg. Fazy powtarzają się, aż pozostanie jeden przebieg zawierający cały zbiór.

## Pliki programu

| Plik | Zawartość |
|---|---|
| `config.txt` | Parametry rekordów i buforów odczytywane przez program sortujący. |
| `tape.bin` i `beforeSorting.txt` | Wejściowy plik binarny i jego reprezentacja tekstowa przed sortowaniem. |
| `beforeStage_2_0.bin` i `afterStageOne.txt` | Przebiegi utworzone w pierwszym etapie, w postaci binarnej i tekstowej. |
| `beforeStage_2_<…>.bin` i `afterStage_2_<…>.txt` | Dane po kolejnych cyklach scalania. Plik binarny stanowi wejście dla kolejnej fazy, a tekstowy pozwala sprawdzić wynik pośredni. |

Program wypisuje również liczbę odczytanych i zapisanych stron, sumę operacji wejścia/wyjścia, liczbę cykli scalania i czas wykonania w sekundach.

## Ile faz i operacji I/O potrzeba?

Po pierwszym etapie pozostaje $r = \lceil N/(nb) \rceil$ posortowanych przebiegów. Po każdej fazie scalania maksymalna długość przebiegu rośnie $n-1$ razy. Dla niepustego pliku i co najmniej trzech buforów liczba faz **drugiego etapu** wynosi:

$$
k = \left\lceil \log_{n-1} r \right\rceil
$$

Gdy $r=1$, scalanie wymaga **zera faz**, ale utworzenie pierwszego przebiegu nadal wymaga odczytu i zapisu danych. W modelu projektu każdy pełny przebieg przez plik obejmuje odczyt i zapis wszystkich stron. Łącznie dla obu etapów:

$$
I/O = 2 \left\lceil N/b \right\rceil (1+k)
$$

Jedną operacją jest tu odczyt albo zapis jednej strony. Dla 10 mln rekordów i 10 rekordów na stronę każda dodatkowa faza oznacza więc milion odczytów i milion zapisów.

## Wyniki eksperymentu

We wszystkich poniższych pomiarach strona mieściła 10 rekordów. Dla przedstawionych konfiguracji liczby faz scalania i operacji I/O zgadzają się z modelem teoretycznym.

### Liczba faz scalania a rozmiar zbioru

| Rekordy (N) | 11 buforów | 101 buforów | 1001 buforów |
|---:|---:|---:|---:|
| 100 | 0 | 0 | 0 |
| 1 000 | 1 | 0 | 0 |
| 10 000 | 2 | 1 | 0 |
| 100 000 | 3 | 1 | 1 |
| 1 000 000 | 4 | 2 | 1 |
| 10 000 000 | 5 | 2 | 1 |

![Liczba faz sortowania w zależności od liczby sortowanych rekordów](results/plots/Liczba%20faz%20sortowania%20w%20zale%C5%BCno%C5%9Bci%20od%20liczby%20sortowanych%20rekord%C3%B3w.png)

### Liczba operacji I/O a rozmiar zbioru

| Rekordy (N) | 11 buforów | 101 buforów | 1001 buforów |
|---:|---:|---:|---:|
| 100 | 20 | 20 | 20 |
| 1 000 | 400 | 200 | 200 |
| 10 000 | 6 000 | 4 000 | 2 000 |
| 100 000 | 80 000 | 40 000 | 40 000 |
| 1 000 000 | 1 000 000 | 600 000 | 400 000 |
| 10 000 000 | 12 000 000 | 6 000 000 | 4 000 000 |

![Liczba operacji dyskowych w zależności od liczby sortowanych rekordów](results/plots/Liczba%20operacji%20dyskowych%20w%20zale%C5%BCno%C5%9Bci%20od%20liczby%20sortowanych%20rekord%C3%B3w.png)

### Liczba operacji I/O a liczba buforów

| Bufory (n) | 1 mln rekordów | 10 mln rekordów |
|---:|---:|---:|
| 11 | 1 000 000 | 12 000 000 |
| 101 | 600 000 | 6 000 000 |
| 1 001 | 400 000 | 4 000 000 |
| 10 001 | 400 000 | 4 000 000 |
| 100 001 | 200 000 | 4 000 000 |
| 1 000 001 | 200 000 | 2 000 000 |

![Liczba operacji dyskowych w zależności od liczby buforów](results/plots/Liczba%20operacji%20dyskowych%20w%20zale%C5%BCno%C5%9Bci%20od%20liczby%20bufor%C3%B3w.png)

Koszt I/O maleje skokowo. Dla 10 mln rekordów konfiguracje z 1001, 10 001 i 100 001 buforów wymagają po jednej fazie scalania, więc każda wykonuje 4 mln operacji I/O. Dopiero przy 1 000 001 buforach cały zbiór mieści się w pamięci w pierwszym etapie, a koszt spada do 2 mln operacji.

[Pełny raport](raport.pdf?raw=true) · [Tabele ze sprawozdania](results/tables/) · [Pełne wyniki pomiarów i czasy wykonania](results/results.txt)

## Bibliografia

- Politechnika Gdańska, *Struktury baz danych – 2023*, materiały wykładowe, część I. [Kurs na platformie eNauczanie](https://enauczanie.pg.edu.pl/moodle/course/view.php?id=30716)
- OpenDSA, *External Sorting*. [Data Structures and Algorithms Modules Collection](https://opendsa-server.cs.vt.edu/ODSA/Books/Everything/html/ExternalSort.html)
