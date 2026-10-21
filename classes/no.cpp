using namespace std;

class No {
protected:
    int valor;
    No* esquerda;
    No* direita;

public:
    No(int id) {
        valor = id;
        esquerda = nullptr;
        direita = nullptr;
    }

    virtual ~No() {}

    int getvalor() const { return valor; }
    No* getEsquerda() const { return esquerda; }
    No* getDireita() const { return direita; }

    void setEsquerda(No* esq) { esquerda = esq; }
    void setDireita(No* dir) { direita = dir; }
};