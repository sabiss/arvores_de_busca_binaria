#ifndef NO_AVL_CPP
#define NO_AVL_CPP

using namespace std;

template <typename T>
class NoAVL {
public:
    T valor;
    NoAVL<T>* esquerda;
    NoAVL<T>* direita;
    int altura;

    NoAVL(T id) {
        this->valor = id;
        this->esquerda = nullptr;
        this->direita = nullptr;
        this->altura = 1;
    }

    ~NoAVL() {}

    T getValor() const { return valor; }
    NoAVL<T>* getEsquerda() const { return esquerda; }
    NoAVL<T>* getDireita() const { return direita; }
    int getAltura() const { return altura; }

    void setValor(T valor) { this->valor = valor; }
    void setEsquerda(NoAVL<T>* esq) { this->esquerda = esq; }
    void setDireita(NoAVL<T>* dir) { this->direita = dir; }
    void setAltura(int alt) { this->altura = alt; }
};

#endif