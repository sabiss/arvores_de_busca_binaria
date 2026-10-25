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

    NoAVL<T>* buscarRecursivo(NoAVL<T>* atual, T valor) const {
        if (atual == nullptr || atual->getValor() == valor) {
            return atual;
        }
        if (valor < atual->getValor()) {
            return buscarRecursivo(atual->esquerda, valor);
        }
        return buscarRecursivo(atual->direita, valor);
    }

    NoAVL<T>* encontrarMenor(NoAVL<T>* atual) const {
        while (atual != nullptr && atual->esquerda != nullptr) {
            atual = atual->esquerda;
        }
        return atual;
    }

public:
    ArvoreAVL() : raiz(nullptr) {}

    ~ArvoreAVL() {
        destruirArvore(raiz);
    }

    NoAVL<T>* buscar(T valor) const {
        return buscarRecursivo(raiz, valor);
    }

    void percorrerEmOrdem() const {
        cout << "Elementos em ordem: ";
        percorrerEmOrdemRecursivo(raiz);
        cout << endl;
    }
};

#endif