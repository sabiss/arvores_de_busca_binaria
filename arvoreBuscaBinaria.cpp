#include <iostream>
#include <string>

using namespace std;

template <typename T>
class ArvoreBuscaBinaria {
protected:
    No<T>* raiz;

    void inserirRecursivo(No<T>*& atual, T valor) {
        if (atual == nullptr) {
            //O espaço está vazio, então cria um nó
            atual = new No<T>(valor);
            return;
        }
        
        if (valor < atual->getValor()) {
            inserirRecursivo(atual->esquerda, valor);
        } else if (valor > atual->getValor()) {
            inserirRecursivo(atual->direita, valor);
        }
        //escolhi por ignorar as repetições, então se acharmos um nó que já existe quiser ser inserido ele não vai, pois não fiz nenhum tratamento pra ele
    }

    No<T>* buscarRecursivo(No<T>* atual, T valor) {
        if (atual == nullptr || atual->getValor() == valor) {
            return atual;
        }
        if (valor < atual->getValor()) {
            return buscarRecursivo(atual->esquerda, valor);
        }
        return buscarRecursivo(atual->direita, valor);
    }

    void removerRecursivo(No<T>*& atual, T valor) {
        if (atual == nullptr) return; //não encontr

        if (valor < atual->getValor()) {
            removerRecursivo(atual->esquerda, valor);
        } else if (valor > atual->getValor()) {
            removerRecursivo(atual->direita, valor);
        } else {
            //Nó encontrado. 
            //Caso 1 e 2: Zero ou um filho
            if (atual->esquerda == nullptr) {
                No<T>* temp = atual;
                atual = atual->direita; //troco os valores pelo da filho da direita
                delete temp; //apago o antigo
            } 
            else if (atual->direita == nullptr) {
                No<T>* temp = atual;
                atual = atual->esquerda;
                delete temp;
            } 
            //Caso 3: Nó com dois filhos
            else {
                //Encontra a menor das chaves maiores que x ou
                No<T>* menorDasChavesMaioresQueX = encontrarMenor(atual->direita);
                
                atual->setValor(menorDasChavesMaioresQueX->getValor());
                //removo o valor
                removerRecursivo(atual->direita, menorDasChavesMaioresQueX->getValor());
            }
        }
    }

    No<T>* encontrarMenor(No<T>* atual) {
        while (atual != nullptr && atual->esquerda != nullptr) {
            atual = atual->esquerda;
        }
        return atual;
    }

    void percorrerEmOrdemRecursivo(No<T>* atual) {
        if (atual != nullptr) {
            percorrerEmOrdemRecursivo(atual->esquerda);
            cout << atual->getValor() << " ";
            percorrerEmOrdemRecursivo(atual->direita);
        }
    }

    void imprimirRecursivo(No<T>* atual, string prefixo, bool isEsquerda) {
        if (atual != nullptr) {
            cout << prefixo << (isEsquerda ? "├── " : "└── ") << atual->getValor() << endl;
            imprimirRecursivo(atual->esquerda, prefixo + (isEsquerda ? "│   " : "    "), true);
            imprimirRecursivo(atual->direita, prefixo + (isEsquerda ? "│   " : "    "), false);
        }
    }

    //Método auxiliar para destruir a árvore recursivamente
    void destruirArvore(No<T>* atual) {
        if (atual != nullptr) {
            destruirArvore(atual->esquerda);
            destruirArvore(atual->direita);
            delete atual;
        }
    }

public:
    ArvoreBuscaBinaria() : raiz(nullptr) {}

    //destrutor
    virtual ~ArvoreBuscaBinaria() {
        destruirArvore(raiz);
    }

    void inserir(T valor) {
        inserirRecursivo(raiz, valor);
    }

    No<T>* buscar(T valor) {
        return buscarRecursivo(raiz, valor);
    }

    void remover(T valor) {
        removerRecursivo(raiz, valor);
    }

    void percorrerEmOrdem() {
        cout << "Elementos: ";
        percorrerEmOrdemRecursivo(raiz);
        cout << endl;
    }

    void imprimir() {
        if (raiz == nullptr) {
            cout << "Árvore vazia." << endl;
            return;
        }
        cout << "Estrutura da Árvore:" << endl;
        imprimirRecursivo(raiz, "", true);
    }
};