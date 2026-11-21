#include "raylib.h"
#include "tryb_latwy.h"
#include "tryb_trudny.h"
#include "ranking.h"
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <iostream>

using namespace std;

enum StanGry {
    MENU_GLOWNE,
    TRYB_LABIRYNT_GRA,
    TRYB_KRYSZTALY_GRA,
    WPISZ_NICK,
    RANKINGI
};

static bool czyLapieSieDoTop5(const string& nazwaPliku, int punkty) {
    vector<WynikGracza> wyniki = wczytajWyniki(nazwaPliku);
    sort(wyniki.begin(), wyniki.end(), greater<WynikGracza>());
    if (wyniki.size() < 5) return true;
    return punkty > wyniki.back().punkty;
}

int main() {
    const int oknoSzerokosc = 1024;
    const int oknoWysokosc = 768;

    InitWindow(oknoSzerokosc, oknoWysokosc, "NeonCat - Modular Game");
    SetTargetFPS(60);

    StanGry stan = MENU_GLOWNE;
    int ostatniePunkty = 0;
    string ostatniPlikRankingu = "wyniki_labirynt.txt";
    string nickInput = "";

    try {
        while (!WindowShouldClose()) {
            if (stan == MENU_GLOWNE) {
                if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                    stan = TRYB_LABIRYNT_GRA;
                }
                if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                    stan = TRYB_KRYSZTALY_GRA;
                }
                if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                    stan = RANKINGI;
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    break;
                }
            }
            else if (stan == TRYB_LABIRYNT_GRA) {
                ostatniePunkty = uruchomTrybLatwy();
                ostatniPlikRankingu = "wyniki_labirynt.txt";
                if (czyLapieSieDoTop5(ostatniPlikRankingu, ostatniePunkty)) {
                    nickInput.clear();
                    stan = WPISZ_NICK;
                } else {
                    stan = RANKINGI;
                }
            }
            else if (stan == TRYB_KRYSZTALY_GRA) {
                ostatniePunkty = uruchomTrybTrudny();
                ostatniPlikRankingu = "wyniki_krysztaly.txt";
                if (czyLapieSieDoTop5(ostatniPlikRankingu, ostatniePunkty)) {
                    nickInput.clear();
                    stan = WPISZ_NICK;
                } else {
                    stan = RANKINGI;
                }
            }
            else if (stan == WPISZ_NICK) {
                int znak = GetCharPressed();
                while (znak > 0) {
                    if (znak >= 32 && znak <= 125 && nickInput.size() < 16 && znak != ' ') {
                        nickInput.push_back((char)znak);
                    }
                    znak = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE) && !nickInput.empty()) {
                    nickInput.pop_back();
                }

                if (IsKeyPressed(KEY_ENTER)) {
                    string finalnyNick = nickInput.empty() ? "Kot" : nickInput;
                    zapiszJesliTop5(ostatniPlikRankingu, finalnyNick, ostatniePunkty);
                    stan = RANKINGI;
                }
            }
            else if (stan == RANKINGI) {
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) {
                    stan = MENU_GLOWNE;
                }
            }

            BeginDrawing();
            ClearBackground(Color{10, 10, 25, 255});

            if (stan == MENU_GLOWNE) {
                DrawText("NEONCAT", oknoSzerokosc / 2 - MeasureText("NEONCAT", 50) / 2, 180, 50, Color{0, 240, 255, 255});
                DrawText("Wybierz tryb rozgrywki:", oknoSzerokosc / 2 - MeasureText("Wybierz tryb rozgrywki:", 20) / 2, 260, 20, RAYWHITE);

                DrawText("[ 1 ] Tryb Łatwy", 320, 340, 20, GOLD);
                DrawText("[ 2 ] Tryb Trudny", 320, 390, 20, MAGENTA);
                DrawText("[ 3 ] Tablica Wyników (Rankingi)", 320, 440, 20, SKYBLUE);
                DrawText("[ ESC ] Wyjście z gry", 320, 510, 20, LIGHTGRAY);

                DrawText("Sterowanie w grze: Strzalki / W, A, S, D", 320, 620, 18, GRAY);
            }
            else if (stan == WPISZ_NICK) {
                DrawText("NOWY REKORD TOP 5!", oknoSzerokosc / 2 - MeasureText("NOWY REKORD TOP 5!", 32) / 2, 170, 32, GOLD);

                string infoPunkty = "Twój wynik: " + odmiana(ostatniePunkty);
                DrawText(infoPunkty.c_str(), oknoSzerokosc / 2 - MeasureText(infoPunkty.c_str(), 24) / 2, 230, 24, SKYBLUE);

                DrawText("Wpisz swoj nick i nacisnij ENTER:", oknoSzerokosc / 2 - MeasureText("Wpisz swoj nick i nacisnij ENTER:", 20) / 2, 300, 20, RAYWHITE);

                DrawRectangle(oknoSzerokosc / 2 - 180, 345, 360, 50, Color{25, 25, 50, 255});
                DrawRectangleLines(oknoSzerokosc / 2 - 180, 345, 360, 50, GOLD);

                string pokazNick = nickInput + "_";
                DrawText(pokazNick.c_str(), oknoSzerokosc / 2 - 165, 358, 24, WHITE);

                DrawText("(Zostaw puste, aby uzyc domyslnego: Kot)", oknoSzerokosc / 2 - MeasureText("(Zostaw puste, aby uzyc domyslnego: Kot)", 16) / 2, 420, 16, GRAY);
            }
            else if (stan == RANKINGI) {
                DrawText("TABLICA WYNIKOW (TOP 5)", oknoSzerokosc / 2 - MeasureText("TABLICA WYNIKOW (TOP 5)", 30) / 2, 80, 30, GOLD);

                vector<WynikGracza> rankLabirynt = wczytajWyniki("wyniki_labirynt.txt");
                DrawText("--- Tryb Labirynt ---", 180, 160, 20, SKYBLUE);
                int yOffset = 200;
                if (rankLabirynt.empty()) {
                    DrawText("Brak wynikow.", 180, yOffset, 16, GRAY);
                } else {
                    int m = 1;
                    for (const auto& w : rankLabirynt) {
                        string linia = to_string(m) + ". " + w.nick + " - " + odmiana(w.punkty);
                        DrawText(linia.c_str(), 180, yOffset, 16, WHITE);
                        yOffset += 25;
                        m++;
                    }
                }

                vector<WynikGracza> rankKrysztaly = wczytajWyniki("wyniki_krysztaly.txt");
                DrawText("--- Tryb Krysztaly ---", 580, 160, 20, MAGENTA);
                yOffset = 200;
                if (rankKrysztaly.empty()) {
                    DrawText("Brak wynikow.", 580, yOffset, 16, GRAY);
                } else {
                    int m = 1;
                    for (const auto& w : rankKrysztaly) {
                        string linia = to_string(m) + ". " + w.nick + " - " + odmiana(w.punkty);
                        DrawText(linia.c_str(), 580, yOffset, 16, WHITE);
                        yOffset += 25;
                        m++;
                    }
                }

                DrawText("Nacisnij BACKSPACE, aby wrocic do menu.", oknoSzerokosc / 2 - MeasureText("Nacisnij BACKSPACE, aby wrocic do menu.", 16) / 2, 650, 16, LIGHTGRAY);
            }

            EndDrawing();
        }
    } catch (const exception& e) {
        cerr << "[Blad krytyczny] " << e.what() << "\n";
    }

    CloseWindow();
    return 0;
}