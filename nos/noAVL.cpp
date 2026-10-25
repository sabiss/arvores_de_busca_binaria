#include "./no.cpp"

class NoAVL : public No {
private:
    int altura;

public:
    NoAVL(int id) : No(id), altura(1) {}

    int getAltura() const { return altura; }
    void setAltura(int alt) { altura = alt; }
};