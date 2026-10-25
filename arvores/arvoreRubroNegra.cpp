#ifndef ARVORE_RUBRO_NEGRA_CPP
#define ARVORE_RUBRO_NEGRA_CPP

#include <iostream>
#include <string>
#include "./../nos/noRN.cpp"

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

    NoRN<T>* buscarRecursivo(NoRN<T>* atual, T valor) const {
        if (atual == NIL || atual->getValor() == valor) {
            return atual;
        }
        if (valor < atual->getValor()) {
            return buscarRecursivo(atual->esquerda, valor);
        }
        return buscarRecursivo(atual->direita, valor);
    }

    NoRN<T>* encontrarMenor(NoRN<T>* atual) const {
        while (atual != NIL && atual->esquerda != NIL) {
            atual = atual->esquerda;
        }
        return atual;
    }

    void rotacaoEsquerda(NoRN<T>* x) {
        NoRN<T>* y = x->direita; //y é o filho direito de x (que vai subir)
        
        //A subárvore esquerda de y vira a nova subárvore direita de x
        x->direita = y->esquerda;
        if (y->esquerda != NIL) {
            y->esquerda->pai = x;
        }

        //Conecta o pai de x ao novo topo da subárvore (y)
        y->pai = x->pai;
        if (x->pai == NIL) {
            raiz = y; //x era a raiz da árvore inteira
        } else if (x == x->pai->esquerda) {
            x->pai->esquerda = y;
        } else {
            x->pai->direita = y;
        }

        //Coloca x como filho esquerdo de y
        y->esquerda = x;
        x->pai = y;
    }

    // Rotação à Direita em torno do nó y
    void rotacaoDireita(NoRN<T>* y) {
        NoRN<T>* x = y->esquerda; //=

        //A subárvore direita de x vira a nova subárvore esquerda de y
        y->esquerda = x->direita;
        if (x->direita != NIL) {
            x->direita->pai = y;
        }

        //Conecta o pai de y ao novo topo da subárvore (x)
        x->pai = y->pai;
        if (y->pai == NIL) {
            raiz = x; //y era a raiz da árvore inteira
        } else if (y == y->pai->direita) {
            y->pai->direita = x;
        } else {
            y->pai->esquerda = x;
        }

        //Coloca y como filho direito de x
        x->direita = y;
        y->pai = x;
    }

    void balancearInsercao(NoRN<T>* z) {
        while (z->pai->cor == VERMELHO) {
            NoRN<T>* pai = z->pai;
            NoRN<T>* avo = pai->pai;

            //O pai de z é o filho ESQUERDO do avô
            if (pai == avo->esquerda) {
                NoRN<T>* tio = avo->direita;

                //O Tio é VERMELHO -> Apenas recolorir Pai, Tio e Avô
                if (tio->cor == VERMELHO) {
                    pai->cor = PRETO;
                    tio->cor = PRETO;
                    avo->cor = VERMELHO;
                    z = avo; //Sobe o problema para o avô e continua o while
                } 
                else {
                    //O Tio é PRETO e z é filho DIREIT
                    if (z == pai->direita) {
                        z = pai;
                        rotacaoEsquerda(z);
                        pai = z->pai;
                        avo = pai->pai;
                    }
                    //O Tio é PRETO e z é filho ESQUERDO
                    pai->cor = PRETO;
                    avo->cor = VERMELHO;
                    rotacaoDireita(avo);
                }
            } 
            //O pai de z é o filho DIREITO do avô
            else {
                NoRN<T>* tio = avo->esquerda;

                //O Tio é VERMELHO -> Recoloração
                if (tio->cor == VERMELHO) {
                    pai->cor = PRETO;
                    tio->cor = PRETO;
                    avo->cor = VERMELHO;
                    z = avo; // Sobe para o avô
                } 
                else {
                    //O Tio é PRETO e z é filho ESQUERDO
                    if (z == pai->esquerda) {
                        z = pai;
                        rotacaoDireita(z);
                        pai = z->pai;
                        avo = pai->pai;
                    }
                    //O Tio é PRETO e z é filho DIREITO
                    pai->cor = PRETO;
                    avo->cor = VERMELHO;
                    rotacaoEsquerda(avo);
                }
            }
        }
        
        raiz->cor = PRETO;
    }

    void transplantar(NoRN<T>* u, NoRN<T>* v) {
        if (u->pai == NIL) {
            raiz = v;
        } else if (u == u->pai->esquerda) {
            u->pai->esquerda = v;
        } else {
            u->pai->direita = v;
        }
        v->pai = u->pai; // Mesmo que v seja NIL, atualizamos o pai temporariamente para o Fixup
    }

    void balancearRemocao(NoRN<T>* x) {
        while (x != raiz && x->cor == PRETO) {
            //x está à ESQUERDA do seu pai
            if (x == x->pai->esquerda) {
                NoRN<T>* irmao = x->pai->direita;

                //O irmão de x é VERMELHO
                if (irmao->cor == VERMELHO) {
                    irmao->cor = PRETO;
                    x->pai->cor = VERMELHO;
                    rotacaoEsquerda(x->pai);
                    irmao = x->pai->direita;
                }

                //Caso 2: O irmão é PRETO e ambos os filhos do irmão são PRETOS
                if (irmao->esquerda->cor == PRETO && irmao->direita->cor == PRETO) {
                    irmao->cor = VERMELHO;
                    x = x->pai; // Sobe a falta de cor preta para o pai
                } else {
                    //Caso 3: O irmão é PRETO, sobrinho esquerdo é VERMELHO e direito é PRETO
                    if (irmao->direita->cor == PRETO) {
                        irmao->esquerda->cor = PRETO;
                        irmao->cor = VERMELHO;
                        rotacaoDireita(irmao);
                        irmao = x->pai->direita;
                    }
                    //Caso 4: O irmão é PRETO e o sobrinho direito é VERMELHO
                    irmao->cor = x->pai->cor;
                    x->pai->cor = PRETO;
                    irmao->direita->cor = PRETO;
                    rotacaoEsquerda(x->pai);
                    x = raiz;
                }
            } 
            //x está à DIREITA do seu pai
            else {
                NoRN<T>* irmao = x->pai->esquerda;

                //Caso 1: O irmão de x é VERMELHO
                if (irmao->cor == VERMELHO) {
                    irmao->cor = PRETO;
                    x->pai->cor = VERMELHO;
                    rotacaoDireita(x->pai);
                    irmao = x->pai->esquerda;
                }

                //Caso 2: O irmão é PRETO e ambos os filhos do irmão são PRETOS
                if (irmao->direita->cor == PRETO && irmao->esquerda->cor == PRETO) {
                    irmao->cor = VERMELHO;
                    x = x->pai;
                } else {
                    //Caso 3: O irmão é PRETO, sobrinho direito é VERMELHO e esquerdo é PRETO
                    if (irmao->esquerda->cor == PRETO) {
                        irmao->direita->cor = PRETO;
                        irmao->cor = VERMELHO;
                        rotacaoEsquerda(irmao);
                        irmao = x->pai->esquerda;
                    }
                    //Caso 4: O irmão é PRETO e o sobrinho esquerdo (distante) é VERMELHO
                    irmao->cor = x->pai->cor;
                    x->pai->cor = PRETO;
                    irmao->esquerda->cor = PRETO;
                    rotacaoDireita(x->pai);
                    x = raiz;
                }
            }
        }

        x->cor = PRETO;
    }

    void imprimirRecursivo(NoRN<T>* atual, string prefixo, bool isEsquerda) const {
        if (atual != NIL) {
            string etiquetaCor = (atual->cor == VERMELHO) ? "[V]" : "[P]";
            cout << prefixo << (isEsquerda ? "├── " : "└── ") << atual->getValor() << " " << etiquetaCor << endl;
            imprimirRecursivo(atual->esquerda, prefixo + (isEsquerda ? "│   " : "    "), true);
            imprimirRecursivo(atual->direita, prefixo + (isEsquerda ? "│   " : "    "), false);
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

    NoRN<T>* buscar(T valor) const {
        NoRN<T>* resultado = buscarRecursivo(raiz, valor);
        return (resultado == NIL) ? nullptr : resultado;
    }

    void inserir(T valor) {
        NoRN<T>* pai = NIL;
        NoRN<T>* atual = raiz;

        while (atual != NIL) {
            pai = atual;
            if (valor < atual->getValor()) {
                atual = atual->esquerda;
            } else if (valor > atual->getValor()) {
                atual = atual->direita;
            } else {
                //Valor duplicado
                return;
            }
        }

        NoRN<T>* novo = new NoRN<T>(valor);
        novo->pai = pai;
        novo->esquerda = NIL;
        novo->direita = NIL;

        //Conecta o novo nó ao seu pai (ou define como raiz se a árvore estava vazia)
        if (pai == NIL) {
            raiz = novo;
        } else if (valor < pai->getValor()) {
            pai->esquerda = novo;
        } else {
            pai->direita = novo;
        }

        balancearInsercao(novo);
    }

    void remover(T valor) {
        NoRN<T>* z = buscarRecursivo(raiz, valor);
        if (z == NIL) return;

        NoRN<T>* y = z;
        NoRN<T>* x;
        Cor corOriginalDeY = y->cor;

        //z não tem filho à esquerda
        if (z->esquerda == NIL) {
            x = z->direita;
            transplantar(z, z->direita);
        } 
        //z não tem filho à direita
        else if (z->direita == NIL) {
            x = z->esquerda;
            transplantar(z, z->esquerda);
        } 
        //z tem 2 filhos
        else {
            y = encontrarMenor(z->direita);
            corOriginalDeY = y->cor;
            x = y->direita;

            if (y->pai == z) {
                x->pai = y;
            } else {
                transplantar(y, y->direita);
                y->direita = z->direita;
                y->direita->pai = y;
            }

            transplantar(z, y);
            y->esquerda = z->esquerda;
            y->esquerda->pai = y;
            y->cor = z->cor;
        }

        delete z;

        if (corOriginalDeY == PRETO) {
            balancearRemocao(x);
        }
    }

    void imprimir() const {
        if (raiz == NIL) {
            cout << "Árvore Rubro-Negra vazia." << endl;
            return;
        }
        cout << "Estrutura da Árvore Rubro-Negra ([V]=Vermelho, [P]=Preto):" << endl;
        imprimirRecursivo(raiz, "", true);
    }

};

#endif