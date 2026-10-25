#ifndef ARVORE_AVL_CPP
#define ARVORE_AVL_CPP

#include <iostream>
#include "./nos/noAVL.cpp"

using namespace std;

template <typename T>
class ArvoreAVL {
private:
    NoAVL<T>* raiz;

    void destruirArvore(NoAVL<T>* atual) {
        if (atual != nullptr) {
            destruirArvore(atual->esquerda);
            destruirArvore(atual->direita);
            delete atual;
        }
    }

    void percorrerEmOrdemRecursivo(NoAVL<T>* atual) const {
        if (atual != nullptr) {
            percorrerEmOrdemRecursivo(atual->esquerda);
            cout << atual->getValor() << " ";
            percorrerEmOrdemRecursivo(atual->direita);
        }
    }

public:
    ArvoreAVL() : raiz(nullptr) {}

    ~ArvoreAVL() {
        destruirArvore(raiz);
    }

    void percorrerEmOrdem() const {
        cout << "Elementos em ordem: ";
        percorrerEmOrdemRecursivo(raiz);
        cout << endl;
    }
};

#endif