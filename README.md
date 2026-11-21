# NeonCat

Gra zręcznościowa 2D w języku C++ z wykorzystaniem biblioteki **raylib**, przygotowana z naciskiem na programowanie obiektowe (OOP), podział na moduły nagłówkowe (`.h`) i źródłowe (`.cpp`), przeciążanie operatorów oraz obsługę wyjątków.

---

## 1. Architektura projektu

Projekt podzielony jest na osobne moduły:

* **`main.cpp`** — główna pętla aplikacji, obsługa stanów gry (`MENU_GLOWNE`, `TRYB_LABIRYNT_GRA`, `TRYB_KRYSZTALY_GRA`, `WPISZ_NICK`, `RANKINGI`), obsługa wpisywania nicku po osiągnięciu wyniku z Top 5 oraz zewnętrzny blok `try / catch`.
* **`obiekty.h` / `obiekty.cpp`** — hierarchia obiektów gry oraz rysowanie ścian labiryntu:
  * **`ObiektGry`** (klasa bazowa) — prywatne pola `x`, `y`, `hitbox`, konstruktor domyślny i parametryczny, wirtualny destruktor, metody dostępowe (`getX`, `getY`, `getHitbox`), metody modyfikujące (`setPozycja`, `setHitbox`, `ustawHitbox`) oraz czysto wirtualna metoda `rysuj() const = 0`.
  * **`Gracz`** — dziedziczy publicznie po `ObiektGry`; obsługuje ruch z uwzględnieniem granic planszy i kolizji ze ścianami (`przesun`) oraz rysowanie postaci (`rysuj() const override`).
  * **`Przedmiot`** — dziedziczy publicznie po `ObiektGry`; losuje nową pozycję z uwzględnieniem kolizji (`losujNowaPozycja`, `respawn`) oraz rysuje przedmiot (`rysuj() const override`).
  * **`Krysztal`** — dziedziczy publicznie po `ObiektGry`; losuje nową pozycję (`losujNowaPozycja`) oraz rysuje kryształ (`rysuj() const override`).
* **`tryb_latwy.h` / `tryb_latwy.cpp`** — logika i pętla pierwszego trybu rozgrywki (`uruchomTrybLatwy()`), z wykorzystaniem polimorfizmu przez wskaźnik na klasę bazową (`ObiektGry*`).
* **`tryb_trudny.h` / `tryb_trudny.cpp`** — logika i pętla drugiego trybu rozgrywki (`uruchomTrybTrudny()`), zarządzająca kolekcją obiektów `std::vector<Krysztal>` (dodawanie, usuwanie, modyfikacja pozycji i sprawdzanie kolizji).
* **`ranking.h` / `ranking.cpp`** — struktura `WynikGracza` z przeciążonymi operatorami `>` i `==`, wczytywanie i zapisywanie wyników Top 5 do plików (`wyniki_labirynt.txt`, `wyniki_krysztaly.txt`), obsługa wyjątków (`throw std::runtime_error` oraz `try ... catch`) oraz funkcja odmiany `odmiana(int)`.

---

## 2. Kompilacja i uruchomienie

Projekt korzysta z biblioteki **raylib** oraz standardu **C++17**.

### Kompilacja (MSVC / Developer Command Prompt, x64)

W katalogu projektu uruchom:

```bat
cl /EHsc /std:c++17 /MD main.cpp obiekty.cpp tryb_latwy.cpp tryb_trudny.cpp ranking.cpp /I"sciezka\do\raylib\include" /Fe:NeonCat.exe /link /LIBPATH:"sciezka\do\raylib\lib" raylib.lib opengl32.lib gdi32.lib winmm.lib user32.lib shell32.lib
```
A następnie uruchom grę:

.\NeonCat.exe
---

## 3. Przykłady działania programu

### Przykład 1: Uruchomienie gry i wybór trybu z menu głównego
1. Po uruchomieniu `NeonCat.exe` otwiera się okno gry (`1024x768`) z ekranem **NEONCAT**.
2. Naciśnięcie klawisza **`1`** uruchamia **Tryb Łatwy** (`uruchomTrybLatwy()`), a klawisza **`2`** — **Tryb Trudny** (`uruchomTrybTrudny()`).
3. Naciśnięcie klawisza **`3`** przełącza widok na **Tablicę Wyników (TOP 5)** dla obu trybów, a **`ESC`** zamyka aplikację.

### Przykład 2: Sterowanie i mechanika w trakcie rozgrywki
**Poruszanie postacią**: gracz steruje kotem za pomocą strzałek lub klawiszy **`W`**, **`A`**, **`S`**, **`D`**. Ruch uwzględnia granice planszy oraz kolizje ze ścianami labiryntu.
* **Tryb trudny i operacje na kolekcji**: po zdobyciu celu do wektora kryształów dodawany jest nowy obiekt `Krysztal` (`push_back`), natomiast kolizja z kryształem powoduje jego usunięcie z wektora (`erase`).

### Przykład 3: Nowy rekord Top 5, zapis do pliku i obsługa błędów
1. Po zakończeniu rundy program sprawdza, czy zdobyta liczba punktów mieści się w **Top 5** dla danego trybu (`wyniki_labirynt.txt` lub `wyniki_krysztaly.txt`).
2. Jeśli wynik łapie się do Top 5, pojawia się ekran **NOWY REKORD TOP 5!** — wpisujesz nick (lub naciskasz `ENTER` bez wpisywania, aby użyć domyślnego `"Kot"`).
3. Wynik trafia do rankingu sortowanego malejąco (z wykorzystaniem przeciążonego `operator>`) i jest zapisywany do pliku. Gdyby plik nie dał się otworzyć do zapisu, zgłaszany i łapany jest wyjątek (`std::runtime_error`), dzięki czemu program kończy działanie bezpiecznie zamiast się wysypać.