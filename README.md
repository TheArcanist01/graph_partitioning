# Program do podziału grafu
## Wstęp 
### - działanie programu
Program napisany został w języku C w środowisku Visual Studio Code. Aplikacja zaczyna swoje działanie od przyjęcia argumentów z linii wywołania. Argumenty to nazwa pliku, liczba części do podziału, margines procentowy oraz format pliku wyjściowego. Rezultatem działania aplikacji jest plik tekstowy lub binarny.
### - program main
W funkcji main zostały wywołane funkcje zdefiniowane w tym samym pliku (graph.c). Funkcja main spełnia założone cele programu tj. czyta graf zdefiniowany w pliku tekstowym, przetrzymuje go w strukturze graph_t, dzieli go na zadaną przez użytkownika liczbę części oraz zapisuje graf do odpowiedniego pliku (binarnego lub tekstowego).
### - pseudokod
Poniżej zostają zamieszczone zdjęcia kodu zawierające cały main.
## Funkcje
### - opis tekstowy
Program dzieli się na kilka ważnych podfunkcji. Są to:
#### -load_from_file - jest to funkcja czytająca graf z pliku tekstowego. Funkcja ta przyjmuje wskaźnik do podanego przez użytkownika pliku. Zostały tutaj użyte struktury graph_t oraz node_t w celu poprawienia przejrzystości kodu. Funkcja zwraca obiekt typu graph_t, na podstawie którego będzie można później np wypisać graf na konsolę.
#### -partition_graph - funkcja dzieląca graf na zadaną liczbę części. Funkcja ptzyjmuje następujące argumenty: wskaźnik do obiektu, liczbę części do podziału (typ int) oraz margines błędu (liczba typu double). Funkcja zwraca obiekt typu graph_t.
#### -print_graph - funkcja wypisująca graf na konsolę. Funkcja przyjmuje wskaźnik do obiektu typu graph_t i wyświetla go w terminalu w postaci macierzy oraz listy wszystkich połączeń. Macierz zawiera tylko numery węzłów i ich pozycję.
### - struktury
W programie zostały zdefiniowane dwie struktury: node_t oraz graph_t. W skład pierwszej struktury składa się liczba całkowita Number (liczba porządkowa węzła) i tablica Connections (przechowuje połączenia danego wezła). Struktura graph_t zawiera pola Height (wysokość grafu), Width (szerokość grafu), tablicę Nodes (przechowuje listę wszystkich węzłów w grafie) oraz tablicę NodeIndexes (lista indeksów kolejnych węzłów).
### - pseudokod
Poniżej zostają zamieszczone zdjęcia kodu przedstawiające funkcje oraz struktury programu.
## Dodatkowo
### - screenshoty z konsoli
## Podsumowanie
Program spełnia swoje założone cele. Aplikacja zaczyna działanie od przyjęcia poprawnych argumentów od użytkownika. Następnie tworzony jest obiekt, który przechowuje informacje potrzebne do stworzenia grafu. Efekt końcowy to plik tekstowy lub binarny zawierający stworzony graf. Działanie poszczególnych funkcji przedstawiają odpowiednie zdjęcia z terminala.

