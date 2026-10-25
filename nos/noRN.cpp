#ifndef NO_RN_CPP
#define NO_RN_CPP

using namespace std;

enum Cor { VERMELHO, PRETO };

template <typename T>
class NoRN {
public:
    T valor;
    NoRN<T>* esquerda;
    NoRN<T>* direita;
    NoRN<T>* pai;
    Cor cor;

    NoRN(T id) {
        valor = id;
        esquerda = nullptr;
        direita = nullptr;
        pai = nullptr;
        cor = VERMELHO;
    }

    ~NoRN() {}

    T getValor() const { return valor; }
    void setValor(T v) { valor = v; }

    NoRN<T>* getEsquerda() const { return esquerda; }
    void setEsquerda(NoRN<T>* esq) { esquerda = esq; }

    NoRN<T>* getDireita() const { return direita; }
    void setDireita(NoRN<T>* dir) { direita = dir; }

    NoRN<T>* getPai() const { return pai; }
    void setPai(NoRN<T>* p) { pai = p; }

    Cor getCor() const { return cor; }
    void setCor(Cor c) { cor = c; }
};

#endif