#pragma once
#include "raylib.h"
#include <vector>

class ObiektGry {
private:
    float x;
    float y;
    Rectangle hitbox;

protected:
    void ustawHitbox(Rectangle nowyHitbox);

public:
    ObiektGry();
    ObiektGry(float startX, float startY);
    virtual ~ObiektGry() = default;

    float getX() const;
    float getY() const;
    Rectangle getHitbox() const;

    void setPozycja(float nowyX, float nowyY);
    void setHitbox(Rectangle nowyHitbox);

    virtual void rysuj() const = 0;
};

class Gracz : public ObiektGry {
public:
    Gracz(float startX, float startY);
    void przesun(float dx, float dy, float minX, float maxX, float minY, float maxY,
                 const std::vector<Rectangle>& sciany, float skala);
    void rysuj(Color neonowyKolor, float skala) const;
    void rysuj() const override;
};

class Przedmiot : public ObiektGry {
public:
    Przedmiot(float startX, float startY);
    void losujNowaPozycja(float maxX, float maxY);
    void respawn(const std::vector<Rectangle>& sciany);
    void rysuj() const override;
};

class Krysztal : public ObiektGry {
public:
    Krysztal(float startX, float startY);
    void losujNowaPozycja(float maxX, float maxY);
    void rysuj() const override;
};

void rysujLabirynt(const std::vector<Rectangle>& sciany);