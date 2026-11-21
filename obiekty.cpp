#include "obiekty.h"
#include <cmath>

// --- ObiektGry ---
ObiektGry::ObiektGry() : x(0.0f), y(0.0f), hitbox{0.0f, 0.0f, 0.0f, 0.0f} {}

ObiektGry::ObiektGry(float startX, float startY) : x(startX), y(startY) {
    hitbox = { x - 15.0f, y - 15.0f, 30.0f, 30.0f };
}

void ObiektGry::ustawHitbox(Rectangle nowyHitbox) {
    hitbox = nowyHitbox;
}

float ObiektGry::getX() const {
    return x;
}

float ObiektGry::getY() const {
    return y;
}

Rectangle ObiektGry::getHitbox() const {
    return hitbox;
}

void ObiektGry::setPozycja(float nowyX, float nowyY) {
    x = nowyX;
    y = nowyY;
}

void ObiektGry::setHitbox(Rectangle nowyHitbox) {
    hitbox = nowyHitbox;
}

// --- Gracz ---
Gracz::Gracz(float startX, float startY) : ObiektGry(startX, startY) {}

void Gracz::przesun(float dx, float dy, float minX, float maxX, float minY, float maxY,
                    const std::vector<Rectangle>& sciany, float skala) {
    float aktX = getX();
    float aktY = getY();

    if (dx != 0.0f) {
        float nx = aktX + dx;
        if (nx < minX) nx = minX;
        if (nx > maxX) nx = maxX;

        Rectangle hx = { nx - 15.0f * skala, aktY - 15.0f * skala, 30.0f * skala, 30.0f * skala };
        bool kolX = false;
        for (const auto& s : sciany) {
            if (CheckCollisionRecs(hx, s)) { kolX = true; break; }
        }
        if (!kolX) aktX = nx;
    }

    if (dy != 0.0f) {
        float ny = aktY + dy;
        if (ny < minY) ny = minY;
        if (ny > maxY) ny = maxY;

        Rectangle hy = { aktX - 15.0f * skala, ny - 15.0f * skala, 30.0f * skala, 30.0f * skala };
        bool kolY = false;
        for (const auto& s : sciany) {
            if (CheckCollisionRecs(hy, s)) { kolY = true; break; }
        }
        if (!kolY) aktY = ny;
    }

    setPozycja(aktX, aktY);
    setHitbox({ aktX - 15.0f * skala, aktY - 15.0f * skala, 30.0f * skala, 30.0f * skala });
}

void Gracz::rysuj(Color neonowyKolor, float skala) const {
    float gx = getX();
    float gy = getY();

    bool lewo = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
    bool prawo = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
    bool gora = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
    bool dol = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
    bool czyRuch = lewo || prawo || gora || dol;

    static float dir = 1.0f;
    if (lewo) dir = -1.0f;
    if (prawo) dir = 1.0f;

    static float klatka = 0.0f;
    klatka = czyRuch ? (klatka + 0.25f) : 0.0f;
    float offsetNog = czyRuch ? sin(klatka) * 8.4f : 0.0f;

    Color glow = {neonowyKolor.r, neonowyKolor.g, neonowyKolor.b, 45};
    Color core = {240, 255, 255, 255};

    Vector2 pts[20];
    int n = 0;

    if (!czyRuch) {
        Vector2 siedzacy[] = {
            { -8.4f,  -19.6f }, { -16.8f, -11.2f }, { -14.0f,  2.8f  },
            { -19.6f,  22.4f }, { -14.0f,  36.4f }, {  14.0f,  36.4f },
            {  19.6f,  22.4f }, {  16.8f,  2.8f  }, {  15.4f, -11.2f },
            {  19.6f, -25.2f }, {  11.2f, -16.8f }, {  0.0f,  -22.4f },
            { -11.2f, -16.8f }, { -19.6f, -25.2f }, { -8.4f,  -11.2f }
        };
        n = 15;
        for (int i = 0; i < n; i++) {
            pts[i] = { gx + siedzacy[i].x * dir * skala, gy + siedzacy[i].y * skala };
        }
    } else {
        Vector2 idacy[] = {
            { -11.2f, -19.6f }, { -8.4f,   8.4f  }, {  22.4f,  5.6f  },
            {  26.6f, -5.6f  }, {  32.2f,  0.0f  }, {  36.4f, -5.6f  },
            {  43.4f,  4.2f  }, {  37.8f, 11.2f  }, {  50.4f, 30.8f + offsetNog },
            {  33.6f, 16.8f }, {  29.4f, 33.6f - offsetNog }, {  25.2f, 18.2f },
            {  4.2f,  18.2f }, {  5.6f,  33.6f + offsetNog }, { -2.8f,  18.2f },
            { -9.8f,  33.6f - offsetNog }, { -8.4f,   8.4f  }
        };
        n = 17;
        for (int i = 0; i < n; i++) {
            pts[i] = { gx + idacy[i].x * dir * skala, gy + idacy[i].y * skala };
        }
    }

    for (int i = 0; i < n - 1; i++) {
        DrawLineEx(pts[i], pts[i + 1], 10.5f * skala, glow);
        DrawCircleV(pts[i], 5.25f * skala, glow);
        DrawLineEx(pts[i], pts[i + 1], 4.2f * skala, neonowyKolor);
        DrawCircleV(pts[i], 2.1f * skala, neonowyKolor);
        DrawLineEx(pts[i], pts[i + 1], 1.5f * skala, core);
    }
}

void Gracz::rysuj() const {
    rysuj(Color{0, 240, 255, 255}, 1.0f);
}

// --- Przedmiot ---
Przedmiot::Przedmiot(float startX, float startY) : ObiektGry(startX, startY) {}

void Przedmiot::losujNowaPozycja(float maxX, float maxY) {
    float nx = static_cast<float>(GetRandomValue(120, static_cast<int>(maxX)));
    float ny = static_cast<float>(GetRandomValue(130, static_cast<int>(maxY)));
    setPozycja(nx, ny);
    setHitbox({ nx - 12.0f, ny - 12.0f, 24.0f, 24.0f });
}

void Przedmiot::respawn(const std::vector<Rectangle>& sciany) {
    bool kolizja = true;
    while (kolizja) {
        losujNowaPozycja(880.0f, 580.0f);
        kolizja = false;
        for (const auto& s : sciany) {
            if (CheckCollisionRecs(getHitbox(), s)) {
                kolizja = true;
                break;
            }
        }
    }
}

void Przedmiot::rysuj() const {
    float rx = getX();
    float ry = getY();

    Vector2 pts[] = {
        { rx + 18.0f, ry }, { rx + 7.0f,  ry - 11.0f }, { rx - 14.0f, ry - 14.0f },
        { rx - 7.0f,  ry }, { rx - 14.0f, ry + 14.0f }, { rx + 7.0f,  ry + 11.0f }, { rx + 18.0f, ry }
    };

    for (int i = 0; i < 6; i++) {
        DrawLineEx(pts[i], pts[i + 1], 9.0f, Color{255, 140, 0, 45});
        DrawLineEx(pts[i], pts[i + 1], 3.5f, ORANGE);
        DrawLineEx(pts[i], pts[i + 1], 1.5f, Color{255, 255, 220, 255});
    }
    DrawCircle(static_cast<int>(rx + 9.0f), static_cast<int>(ry - 3.5f), 2.8f, Color{255, 255, 220, 255});
}

// --- Krysztal ---
Krysztal::Krysztal(float startX, float startY) : ObiektGry(startX, startY) {}

void Krysztal::losujNowaPozycja(float maxX, float maxY) {
    float nx = static_cast<float>(GetRandomValue(120, static_cast<int>(maxX)));
    float ny = static_cast<float>(GetRandomValue(130, static_cast<int>(maxY)));
    setPozycja(nx, ny);
    setHitbox({ nx - 12.0f, ny - 12.0f, 24.0f, 24.0f });
}

void Krysztal::rysuj() const {
    float kx = getX();
    float ky = getY();

    Vector2 pts[] = {
        { kx, ky - 14.0f }, { kx + 10.0f, ky }, { kx, ky + 14.0f }, { kx - 10.0f, ky }, { kx, ky - 14.0f }
    };

    for (int i = 0; i < 4; i++) {
        DrawLineEx(pts[i], pts[i + 1], 9.0f, Color{180, 0, 255, 45});
        DrawLineEx(pts[i], pts[i + 1], 3.5f, Color{200, 50, 255, 255});
        DrawLineEx(pts[i], pts[i + 1], 1.5f, Color{240, 220, 255, 255});
    }
}

// --- Labirynt ---
void rysujLabirynt(const std::vector<Rectangle>& sciany) {
    DrawRectangleLines(50, 70, 924, 628, Color{0, 240, 255, 150});
    for (const auto& s : sciany) {
        DrawRectangleRec(s, Color{0, 150, 200, 100});
        DrawRectangleLinesEx(s, 2.0f, Color{0, 240, 255, 200});
    }
}