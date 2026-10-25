#ifndef NO_CPP
#define NO_CPP

using namespace std;

template <typename T>
class No {
public:
    T valor;
    No* esquerda;
    No* direita;
    
    No(T id) {
        valor = id;
        esquerda = nullptr;
        direita = nullptr;
    }

    virtual ~No() {}

    T getValor() const { return valor; }
    No* getEsquerda() const { return esquerda; }
    No* getDireita() const { return direita; }

    void setValor(T valor) { this->valor = valor; }
    void setEsquerda(No* esq) { esquerda = esq; }
    void setDireita(No* dir) { direita = dir; }
};

#endif