#include "./no.cpp"

enum Cor { VERMELHO, PRETO };

class NoRN : public No {
private:
    Cor cor;

public:
    NoRN(int id) : No(id), cor(VERMELHO) {}

    Cor getCor() const { return cor; }
    void setCor(Cor c) { cor = c; }
};