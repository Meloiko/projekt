#include "ranking.hpp"
#include <fstream>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <iostream>

using namespace std;

// Pomocnicza funkcja do odmiany
string odmiana(int p) {
    if (p == 1) return "1 punkt";
    int r = p % 10, rr = p % 100;
    return to_string(p) + ((r >= 2 && r <= 4 && (rr < 12 || rr > 14)) ? " punkty" : " punktow");
}

// Wczytywanie wyników ze wskazanego pliku
vector<WynikGracza> wczytajWyniki(string nazwaPliku) {
    vector<WynikGracza> wyniki;
    ifstream plik(nazwaPliku);
    string nick;
    int punkty;
    while (plik >> nick >> punkty) {
        wyniki.push_back({nick, punkty});
    }
    return wyniki;
}

// Zapisywanie wyników do wskazanego pliku (rzuca wyjątek przy błędzie otwarcia)
void zapiszWyniki(vector<WynikGracza> wyniki, string nazwaPliku) {
    sort(wyniki.begin(), wyniki.end(), greater<WynikGracza>());
    if (wyniki.size() > 5) wyniki.resize(5);

    ofstream plik(nazwaPliku);
    if (!plik.is_open()) {
        throw runtime_error("Nie udalo sie otworzyc pliku rankingu do zapisu: " + nazwaPliku);
    }

    for (const auto& w : wyniki) {
        plik << w.nick << " " << w.punkty << "\n";
    }
}

// Sprawdza czy wynik łapie się do Top 5; jeśli tak - dopisuje i zapisuje do pliku
bool zapiszJesliTop5(string nazwaPliku, string nick, int punkty) {
    vector<WynikGracza> wyniki = wczytajWyniki(nazwaPliku);

    sort(wyniki.begin(), wyniki.end(), greater<WynikGracza>());

    bool lapieSieDoTop5 = (wyniki.size() < 5) || (punkty > wyniki.back().punkty);
    if (!lapieSieDoTop5) {
        return false;
    }

    wyniki.push_back({nick, punkty});

    try {
        zapiszWyniki(wyniki, nazwaPliku);
        return true;
    } catch (const exception& e) {
        cerr << "[Blad rankingu] " << e.what() << "\n";
        return false;
    }
}