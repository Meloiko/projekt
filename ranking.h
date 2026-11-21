#pragma once
#include <string>
#include <vector>

struct WynikGracza {
    std::string nick;
    int punkty;

    // Przeciążenie operatora >: większa liczba punktów stoi wyżej w rankingu
    bool operator>(const WynikGracza& inny) const {
        return punkty > inny.punkty;
    }

    // Przeciążenie operatora ==: porównuje nick i liczbę punktów
    bool operator==(const WynikGracza& inny) const {
        return nick == inny.nick && punkty == inny.punkty;
    }
};

std::string odmiana(int p);
std::vector<WynikGracza> wczytajWyniki(std::string nazwaPliku);
void zapiszWyniki(std::vector<WynikGracza> wyniki, std::string nazwaPliku);
bool zapiszJesliTop5(std::string nazwaPliku, std::string nick, int punkty);