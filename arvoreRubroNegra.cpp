#ifndef ARVORE_RUBRO_NEGRA_CPP
#define ARVORE_RUBRO_NEGRA_CPP

#include <iostream>
#include "./nos/noRN.cpp"

using namespace std;

template <typename T>
class ArvoreRubroNegra {
private:
    NoRN<T>* raiz;
    NoRN<T>* NIL; //folhas nulas

    void destruirArvore(NoRN<T>* atual) {
        if (atual != NIL) {
            destruirArvore(atual->esquerda);
            destruirArvore(atual->direita);
            delete atual;
        }
    }

    void percorrerEmOrdemRecursivo(NoRN<T>* atual) const {
        if (atual != NIL) {
            percorrerEmOrdemRecursivo(atual->esquerda);
            cout << atual->getValor() << " ";
            percorrerEmOrdemRecursivo(atual->direita);
        }
    }

public:
    ArvoreRubroNegra() {
        NIL = new NoRN<T>(T());
        NIL->cor = PRETO;
        NIL->esquerda = NIL;
        NIL->direita = NIL;
        NIL->pai = NIL;
        raiz = NIL;
    }

    ~ArvoreRubroNegra() {
        destruirArvore(raiz);
        delete NIL;
    }

    void percorrerEmOrdem() const {
        cout << "Elementos em ordem: ";
        percorrerEmOrdemRecursivo(raiz);
        cout << endl;
    }
};

#endif