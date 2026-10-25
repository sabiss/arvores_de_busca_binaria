#include <iostream>
#include <string>
#include "./arvores/arvoreAVL.cpp"
#include "./arvores/arvoreRubroNegra.cpp"

using namespace std;

template <typename Arvore>
void interagirComArvore(Arvore& arvore, const string& nomeArvore) {
    int opcao;
    int id;

    do {
        cout << "\n===================================================\n";
        cout << "   AMAZON LOGISTICS - BLACK FRIDAY (" << nomeArvore << ")\n";
        cout << "===================================================\n";
        cout << "1. Inserir identificador de encomenda\n";
        cout << "2. Remover identificador de encomenda\n";
        cout << "3. Buscar identificador de encomenda\n";
        cout << "4. Listar encomendas (Percurso em Ordem)\n";
        cout << "5. Exibir estrutura da Arvore (Balanceamento/Cores)\n";
        cout << "0. Voltar ao Menu Principal\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                cout << "Digite o ID da encomenda para inserir: ";
                cin >> id;
                cout << "[Log] Inserindo encomenda ID " << id << "...\n";
                arvore.inserir(id);
                break;
            case 2:
                cout << "Digite o ID da encomenda para remover: ";
                cin >> id;
                cout << "[Log] Tentando remover encomenda ID " << id << "...\n";
                arvore.remover(id);
                break;
            case 3:
                cout << "Digite o ID da encomenda para buscar: ";
                cin >> id;
                if (arvore.buscar(id) != nullptr) {
                    cout << "[Sucesso] Encomenda ID " << id << " encontrada no sistema de logistica!\n";
                } else {
                    cout << "[Aviso] Encomenda ID " << id << " NAO localizada no sistema.\n";
                }
                break;
            case 4:
                arvore.percorrerEmOrdem();
                break;
            case 5:
                arvore.imprimir();
                break;
            case 0:
                cout << "Retornando ao menu principal...\n";
                break;
            default:
                cout << "[Erro] Opcao invalida. Tente novamente.\n";
        }
    } while (opcao != 0);
}

int main() {
    int tipoArvore;

    do {
        cout << "\n===================================================\n";
        cout << "   AMAZON ENGINEERING CHALLENGE - TECH LEAD MENU\n";
        cout << "===================================================\n";
        cout << "Selecione o prototipo de arvore balanceada:\n";
        cout << "1. Arvore AVL\n";
        cout << "2. Arvore Rubro-Negra\n";
        cout << "0. Encerrar Sistema\n";
        cout << "Escolha: ";
        cin >> tipoArvore;

        if (tipoArvore == 1) {
            ArvoreAVL<int> avl;
            interagirComArvore(avl, "AVL");
        } else if (tipoArvore == 2) {
            ArvoreRubroNegra<int> rn;
            interagirComArvore(rn, "RUBRO-NEGRA");
        } else if (tipoArvore != 0) {
            cout << "[Erro] Selecao invalida.\n";
        }
    } while (tipoArvore != 0);

    cout << "Sistema encerrado. Obrigado!\n";
    return 0;
}
