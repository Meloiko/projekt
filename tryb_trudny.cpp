#include "raylib.h"
#include "obiekty.h"
#include "tryb_trudny.h"
#include <vector>
#include <cmath>

using namespace std;

static bool czyWScianie(float x, float y, float promien, const vector<Rectangle>& sciany) {
    Rectangle rectPrzedmiotu = { x - promien, y - promien, promien * 2, promien * 2 };
    for (const auto& sciana : sciany) {
        if (CheckCollisionRecs(rectPrzedmiotu, sciana)) {
            return true;
        }
    }
    return false;
}

int uruchomTrybTrudny() {
    Gracz gracz(200, 350);
    float skalaKota = 1.0f;

    vector<Rectangle> sciany = {
        {250, 160, 30, 140},
        {250, 450, 30, 140},
        {700, 160, 30, 140},
        {700, 450, 30, 140},
        {430, 350, 150, 22}
    };

    Przedmiot rybka(0, 0);

    float startRX, startRY;
    do {
        startRX = (float)GetRandomValue(120, 880);
        startRY = (float)GetRandomValue(130, 580);
    } while (czyWScianie(startRX, startRY, 25.0f, sciany));
    rybka.setPozycja(startRX, startRY);

    vector<Krysztal> krysztaly;

    float startKX, startKY;
    do {
        startKX = (float)GetRandomValue(120, 880);
        startKY = (float)GetRandomValue(130, 580);
    } while (czyWScianie(startKX, startKY, 25.0f, sciany));
    krysztaly.push_back(Krysztal(startKX, startKY));

    int punkty = 0;
    float czasPozostaly = 25.0f;
    bool koniecGry = false;

    vector<Color> paletaKolorow = {MAGENTA, PURPLE, PINK, RED, ORANGE, YELLOW};
    int kolorIndex = 0;

    while (!WindowShouldClose() && !koniecGry) {
        if (IsKeyPressed(KEY_ESCAPE)) koniecGry = true;

        czasPozostaly -= GetFrameTime();
        if (czasPozostaly <= 0) {
            czasPozostaly = 0;
            koniecGry = true;
        }

        float dx = 0.0f, dy = 0.0f;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dx = 4.0f;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) dx = -4.0f;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) dy = 4.0f;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) dy = -4.0f;

        gracz.przesun(dx, dy, 75, 940, 75, 640, sciany, skalaKota);

        float dRybkaX = gracz.getX() - rybka.getX();
        float dRybkaY = gracz.getY() - rybka.getY();
        float promienZebrania = 35.0f * skalaKota;
        if (dRybkaX * dRybkaX + dRybkaY * dRybkaY < promienZebrania * promienZebrania) {
            punkty += 10;
            skalaKota += 0.05f;
            czasPozostaly += 0.8f;
            kolorIndex = (kolorIndex + 1) % paletaKolorow.size();

            float rx, ry;
            do {
                rx = (float)GetRandomValue(120, 880);
                ry = (float)GetRandomValue(130, 580);
            } while (czyWScianie(rx, ry, 25.0f, sciany));
            rybka.setPozycja(rx, ry);

            float kx, ky;
            bool bezpiecznie = false;
            int proby = 0;
            do {
                kx = (float)GetRandomValue(120, 880);
                ky = (float)GetRandomValue(130, 580);

                float distRybka = sqrt((kx - rybka.getX()) * (kx - rybka.getX()) +
                                       (ky - rybka.getY()) * (ky - rybka.getY()));
                if (!czyWScianie(kx, ky, 25.0f, sciany) && distRybka > 140.0f) {
                    bezpiecznie = true;
                }
                proby++;
                if (proby > 50) { bezpiecznie = true; }
            } while (!bezpiecznie);

            krysztaly.push_back(Krysztal(kx, ky));
        }

        for (int i = (int)krysztaly.size() - 1; i >= 0; i--) {
            float dKrysztalX = gracz.getX() - krysztaly[i].getX();
            float dKrysztalY = gracz.getY() - krysztaly[i].getY();
            if (dKrysztalX * dKrysztalX + dKrysztalY * dKrysztalY < 42.0f * 42.0f) {
                punkty -= 1;
                if (punkty < 0) punkty = 0;

                skalaKota -= 0.08f;
                if (skalaKota < 1.0f) skalaKota = 1.0f;

                krysztaly.erase(krysztaly.begin() + i);
            }
        }

        BeginDrawing();
        ClearBackground(Color{25, 10, 25, 255});

        DrawRectangleLines(50, 70, 925, 629, MAGENTA);

        rysujLabirynt(sciany);

        ObiektGry* obiektRybka = &rybka;
        obiektRybka->rysuj();

        for (const auto& k : krysztaly) {
            const ObiektGry* obiektKrysztal = &k;
            obiektKrysztal->rysuj();
        }

        gracz.rysuj(paletaKolorow[kolorIndex], skalaKota);

        DrawText(TextFormat("PUNKTY: %d", punkty), 80, 30, 22, RED);
        DrawText(TextFormat("CZAS: %.1f s", czasPozostaly), 800, 30, 22, czasPozostaly < 10 ? RED : YELLOW);
        DrawText("TRYB TRUDNY - Zjadaj rybki i unikaj krysztalow!", 80, 675, 16, RAYWHITE);

        EndDrawing();
    }
    return punkty;
}