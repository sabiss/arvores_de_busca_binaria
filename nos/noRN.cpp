#ifndef NO_RN_CPP
#define NO_RN_CPP

#include "./no.cpp"

using namespace std;

enum Cor { VERMELHO, PRETO };

template <typename T>
class NoRN : public No<T> {
private:
    Cor cor;

public:
    NoRN(int id) : No(id), cor(VERMELHO) {}

    virtual ~NoRN() {}

    Cor getCor() const { return cor; }
    void setCor(Cor c) { cor = c; }
};

#endif