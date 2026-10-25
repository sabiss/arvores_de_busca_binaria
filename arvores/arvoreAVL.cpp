#ifndef ARVORE_AVL_CPP
#define ARVORE_AVL_CPP

#include <iostream>
#include <algorithm>
#include "./../nos/noAVL.cpp"

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

    int obterAltura(NoAVL<T>* no) const {
        if (no == nullptr) {
            return 0;
        }
        return no->getAltura();
    }

    //FB = Altura(Esquerda) - Altura(Direita)
    int obterFatorBalanceamento(NoAVL<T>* no) const {
        if (no == nullptr) {
            return 0;
        }
        return obterAltura(no->esquerda) - obterAltura(no->direita);
    }

    //Recalcula a altura do nó com base na maior altura entre seus dois filhos
    void atualizarAltura(NoAVL<T>* no) {
        if (no != nullptr) {
            int altEsq = obterAltura(no->esquerda);
            int altDir = obterAltura(no->direita);
            no->setAltura(1 + max(altEsq, altDir));
        }
    }

    //Rotação Simples à Direita - Caso LL
    void rotacaoDireita(NoAVL<T>*& y) {
        NoAVL<T>* x = y->esquerda;
        NoAVL<T>* T2 = x->direita;

        x->direita = y;
        y->esquerda = T2;

        atualizarAltura(y);
        atualizarAltura(x);

        y = x;
    }

    //Rotação Simples à Esquerda - Caso RR
    void rotacaoEsquerda(NoAVL<T>*& x) {
        NoAVL<T>* y = x->direita;
        NoAVL<T>* T2 = y->esquerda;

        y->esquerda = x;
        x->direita = T2;

        atualizarAltura(x);
        atualizarAltura(y);

        x = y;
    }

    void balancear(NoAVL<T>*& atual) {
        if (atual == nullptr) return;

        atualizarAltura(atual);

        int fb = obterFatorBalanceamento(atual);

        //Desbalanceado para a ESQUERDA
        if (fb > 1) {
            //Caso LR
            if (obterFatorBalanceamento(atual->esquerda) < 0) {
                rotacaoEsquerda(atual->esquerda); // Alinha o joelho para virar Caso LL
            }
            //Caso LL
            rotacaoDireita(atual);
        }
        //Desbalanceado para a DIREITA
        else if (fb < -1) {
            //Caso RL
            if (obterFatorBalanceamento(atual->direita) > 0) {
                rotacaoDireita(atual->direita); // Alinha o joelho para virar Caso RR
            }
            //Caso RR
            rotacaoEsquerda(atual);
        }
    }

    void inserirRecursivo(NoAVL<T>*& atual, T valor) {
        if (atual == nullptr) {
            atual = new NoAVL<T>(valor);
            return;
        }

        if (valor < atual->getValor()) {
            inserirRecursivo(atual->esquerda, valor);
        } else if (valor > atual->getValor()) {
            inserirRecursivo(atual->direita, valor);
        } else {
            //Valor já existe
            return;
        }

        balancear(atual);
    }

    void removerRecursivo(NoAVL<T>*& atual, T valor) {
        if (atual == nullptr) return; //Elemento não encontrado

        if (valor < atual->getValor()) {
            removerRecursivo(atual->esquerda, valor);
        } else if (valor > atual->getValor()) {
            removerRecursivo(atual->direita, valor);
        } else {
            //Nó encontrado
            //Caso 2: Nó com 0 ou 1 filho
            if (atual->esquerda == nullptr) {
                NoAVL<T>* temp = atual;
                atual = atual->direita;
                delete temp;
            } else if (atual->direita == nullptr) {
                NoAVL<T>* temp = atual;
                atual = atual->esquerda;
                delete temp;
            } 
            //Caso 3: Nó com 2 filhos
            else { //busca a menor chave das maiores
                NoAVL<T>* menorChaveDasMaioresQueX = encontrarMenor(atual->direita);

                atual->setValor(menorChaveDasMaioresQueX->getValor());

                removerRecursivo(atual->direita, menorChaveDasMaioresQueX->getValor());
            }
        }

        //no removido era uma folha
        if (atual == nullptr) return;

        balancear(atual);
    }

    void imprimirRecursivo(NoAVL<T>* atual, string prefixo, bool isEsquerda, bool isRaiz = false) const {
        if (atual != nullptr) {
        cout << prefixo;
        
        if (isRaiz) {
            cout << "└── [Raiz] ";
        } else {
            cout << (isEsquerda ? "├── [Esq] " : "└── [Dir] ");
        }
        
        cout << atual->getValor() 
             << " (h=" << obterAltura(atual) 
             << ", fb=" << obterFatorBalanceamento(atual) << ")" << endl;

        string novoPrefixo = prefixo + (isEsquerda && !isRaiz ? "│   " : "    ");
        
        imprimirRecursivo(atual->esquerda, novoPrefixo, true, false);
        imprimirRecursivo(atual->direita, novoPrefixo, false, false);
    }
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

    void inserir(T valor) {
        inserirRecursivo(raiz, valor);
    }

    void remover(T valor) {
        removerRecursivo(raiz, valor);
    }

    void imprimir() const {
        if (raiz == nullptr) {
            cout << "Árvore AVL vazia." << endl;
            return;
        }
        cout << "Estrutura da Árvore AVL:" << endl;
        imprimirRecursivo(raiz, "", true, true);
    }
};

#endif