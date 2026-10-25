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
        this->valor = id;
        this->esquerda = nullptr;
        this->direita = nullptr;
        this->pai = nullptr;
        this->cor = VERMELHO;//todo nó novo é vermelho
    }

    //para quando precisar criar um nó preto folha
    NoRN(T id, Cor corInicial) {
        this->valor = id;
        this->esquerda = nullptr;
        this->direita = nullptr;
        this->pai = nullptr;
        this->cor = corInicial;
    }

    ~NoRN() {}

    T getValor() const { return valor; }
    NoRN<T>* getEsquerda() const { return esquerda; }
    NoRN<T>* getDireita() const { return direita; }
    NoRN<T>* getPai() const { return pai; }
    Cor getCor() const { return cor; }

    bool ehVermelho() const { return cor == VERMELHO; }
    bool ehPreto() const { return cor == PRETO; }

    void setValor(T valor) { this->valor = valor; }
    void setEsquerda(NoRN<T>* esq) { this->esquerda = esq; }
    void setDireita(NoRN<T>* dir) { this->direita = dir; }
    void setPai(NoRN<T>* p) { this->pai = p; }
    void setCor(Cor c) { this->cor = c; }
    
    void pintarDeVermelho() { this->cor = VERMELHO; }
    void pintarDePreto() { this->cor = PRETO; }
};

#endif