#include "raylib.h"
#include "obiekty.hpp"
#include "tryb_latwy.hpp"
#include <vector>

using namespace std;

int uruchomTrybLatwy() {
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
    rybka.respawn(sciany);

    int punkty = 0;
    float czasPozostaly = 25.0f;
    bool koniecGry = false;

    vector<Color> paletaKolorow = {SKYBLUE, GREEN, YELLOW, ORANGE, PINK, PURPLE};
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

        float distX = gracz.getX() - rybka.getX();
        float distY = gracz.getY() - rybka.getY();
        float promienZebrania = 35.0f * skalaKota;
        if (distX * distX + distY * distY < promienZebrania * promienZebrania) {
            punkty += 10;
            skalaKota += 0.05f;
            czasPozostaly += 0.7f;
            kolorIndex = (kolorIndex + 1) % paletaKolorow.size();

            rybka.respawn(sciany);
        }

        BeginDrawing();
        ClearBackground(Color{15, 15, 30, 255});

        DrawRectangleLines(50, 70, 925, 629, SKYBLUE);

        rysujLabirynt(sciany);

        ObiektGry* obiektDoRysowania = &rybka;
        obiektDoRysowania->rysuj();

        gracz.rysuj(paletaKolorow[kolorIndex], skalaKota);

        DrawText(TextFormat("PUNKTY: %d", punkty), 80, 30, 22, LIME);
        DrawText(TextFormat("CZAS: %.1f s", czasPozostaly), 800, 30, 22, czasPozostaly < 10 ? RED : YELLOW);
        DrawText("TRYB LATWY - Zjadaj rybki, zyskuj czas i rosnij!", 80, 675, 16, RAYWHITE);

        EndDrawing();
    }
    return punkty;
}