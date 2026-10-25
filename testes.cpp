#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>

#define private public
#include "./arvores/arvoreAVL.cpp"
#include "./arvores/arvoreRubroNegra.cpp"
#undef private

using namespace std;

// ============================================================================
// Infraestrutura de testes
// ============================================================================

static int totalVerificacoes = 0;
static int totalFalhas = 0;

#define CHECK(cond, desc)                                                      \
    do {                                                                       \
        totalVerificacoes++;                                                   \
        if (!(cond)) {                                                         \
            totalFalhas++;                                                     \
            cout << "      [FALHOU] " << desc << " (linha " << __LINE__        \
                 << ")" << endl;                                               \
        }                                                                      \
    } while (0)

static void executar(const string& titulo, void (*teste)()) {
    int falhasAntes = totalFalhas;
    teste();
    cout << "  " << (totalFalhas == falhasAntes ? "[OK]     " : "[FALHOU] ")
         << titulo << endl;
}

// Captura tudo que for impresso em cout durante a execução de f()
template <class F>
static string capturarSaida(F f) {
    ostringstream oss;
    streambuf* antigo = cout.rdbuf(oss.rdbuf());
    f();
    cout.rdbuf(antigo);
    return oss.str();
}

// ============================================================================
// Validadores de invariantes (AVL)
// ============================================================================

// Verifica ordem BST, altura armazenada correta e |fb| <= 1 em TODOS os nós.
static bool validarNoAVL(NoAVL<int>* n, long lo, long hi, int& altReal,
                         string& erro) {
    if (n == nullptr) { altReal = 0; return true; }

    if (!(n->valor > lo && n->valor < hi)) {
        erro = "violacao da ordem BST no no " + to_string(n->valor);
        return false;
    }
    int he = 0, hd = 0;
    if (!validarNoAVL(n->esquerda, lo, n->valor, he, erro)) return false;
    if (!validarNoAVL(n->direita, n->valor, hi, hd, erro)) return false;

    altReal = 1 + max(he, hd);
    if (n->altura != altReal) {
        erro = "altura armazenada incorreta no no " + to_string(n->valor) +
               " (armazenada=" + to_string(n->altura) +
               ", real=" + to_string(altReal) + ")";
        return false;
    }
    if (abs(he - hd) > 1) {
        erro = "fator de balanceamento invalido no no " + to_string(n->valor) +
               " (fb=" + to_string(he - hd) + ")";
        return false;
    }
    return true;
}

static bool validar(const ArvoreAVL<int>& a, string& erro) {
    int h = 0;
    return validarNoAVL(a.raiz, -1000000000L, 1000000000L, h, erro);
}

static int alturaReal(const ArvoreAVL<int>& a) {
    // altura calculada recursivamente (não confia no campo armazenado)
    struct R {
        static int h(NoAVL<int>* n) {
            return n == nullptr ? 0 : 1 + max(h(n->esquerda), h(n->direita));
        }
    };
    return R::h(a.raiz);
}

static void coletar(NoAVL<int>* n, vector<int>& v) {
    if (n == nullptr) return;
    coletar(n->esquerda, v);
    v.push_back(n->valor);
    coletar(n->direita, v);
}

static vector<int> emOrdem(const ArvoreAVL<int>& a) {
    vector<int> v;
    coletar(a.raiz, v);
    return v;
}

// Limite teórico da altura de uma AVL com n nós: ~1,44 * log2(n + 2)
static int limiteAltura(const ArvoreAVL<int>&, int n) {
    return (int)floor(1.4405 * log2(n + 2.0));
}

// ============================================================================
// Validadores de invariantes (Rubro-Negra)
// ============================================================================

// Verifica ordem BST, ponteiros de pai, "sem vermelho-vermelho" e
// mesma altura negra em todos os caminhos.
static bool validarNoRN(const ArvoreRubroNegra<int>& a, NoRN<int>* n, long lo,
                        long hi, int& alturaNegra, string& erro) {
    if (n == a.NIL) { alturaNegra = 1; return true; }

    if (!(n->valor > lo && n->valor < hi)) {
        erro = "violacao da ordem BST no no " + to_string(n->valor);
        return false;
    }
    if (n->esquerda != a.NIL && n->esquerda->pai != n) {
        erro = "ponteiro pai incorreto (filho esquerdo de " + to_string(n->valor) + ")";
        return false;
    }
    if (n->direita != a.NIL && n->direita->pai != n) {
        erro = "ponteiro pai incorreto (filho direito de " + to_string(n->valor) + ")";
        return false;
    }
    if (n->cor == VERMELHO &&
        (n->esquerda->cor == VERMELHO || n->direita->cor == VERMELHO)) {
        erro = "no vermelho com filho vermelho: " + to_string(n->valor);
        return false;
    }

    int be = 0, bd = 0;
    if (!validarNoRN(a, n->esquerda, lo, n->valor, be, erro)) return false;
    if (!validarNoRN(a, n->direita, n->valor, hi, bd, erro)) return false;

    if (be != bd) {
        erro = "alturas negras diferentes no no " + to_string(n->valor) +
               " (esq=" + to_string(be) + ", dir=" + to_string(bd) + ")";
        return false;
    }
    alturaNegra = be + (n->cor == PRETO ? 1 : 0);
    return true;
}

static bool validar(const ArvoreRubroNegra<int>& a, string& erro) {
    if (a.NIL->cor != PRETO) { erro = "sentinela NIL nao e preta"; return false; }
    if (a.raiz != a.NIL) {
        if (a.raiz->cor != PRETO) { erro = "a raiz nao e preta"; return false; }
        if (a.raiz->pai != a.NIL) { erro = "o pai da raiz nao e NIL"; return false; }
    }
    int bh = 0;
    return validarNoRN(a, a.raiz, -1000000000L, 1000000000L, bh, erro);
}

static int alturaReal(const ArvoreRubroNegra<int>& a) {
    struct R {
        static int h(const ArvoreRubroNegra<int>& t, NoRN<int>* n) {
            return n == t.NIL ? 0 : 1 + max(h(t, n->esquerda), h(t, n->direita));
        }
    };
    return R::h(a, a.raiz);
}

static void coletar(const ArvoreRubroNegra<int>& a, NoRN<int>* n, vector<int>& v) {
    if (n == a.NIL) return;
    coletar(a, n->esquerda, v);
    v.push_back(n->valor);
    coletar(a, n->direita, v);
}

static vector<int> emOrdem(const ArvoreRubroNegra<int>& a) {
    vector<int> v;
    coletar(a, a.raiz, v);
    return v;
}

// Limite teórico da altura de uma Rubro-Negra com n nós: 2 * log2(n + 1)
static int limiteAltura(const ArvoreRubroNegra<int>&, int n) {
    return (int)floor(2.0 * log2(n + 1.0));
}

// ============================================================================
// Utilitários genéricos (funcionam para as duas árvores)
// ============================================================================

template <class A>
static void verificar(const A& a, const string& contexto) {
    string erro;
    bool ok = validar(a, erro);
    CHECK(ok, contexto + ": " + erro);
}

template <class A>
static void verificarConteudo(const A& a, const vector<int>& esperado,
                              const string& contexto) {
    CHECK(emOrdem(a) == esperado, contexto + ": conteudo em ordem inesperado");
}

template <class A>
static void verificarAltura(const A& a, int n, const string& contexto) {
    int h = alturaReal(a);
    CHECK(h <= limiteAltura(a, n),
          contexto + ": altura " + to_string(h) + " acima do limite teorico " +
              to_string(limiteAltura(a, n)) + " para n=" + to_string(n));
}

static vector<int> sequenciaCrescente(int n) {
    vector<int> v;
    for (int i = 1; i <= n; i++) v.push_back(i);
    return v;
}

static vector<int> sequenciaDecrescente(int n) {
    vector<int> v;
    for (int i = n; i >= 1; i--) v.push_back(i);
    return v;
}

// Permutação determinística de 1..p-1 (p primo): (i*37) mod p
static vector<int> sequenciaNaoOrdenada(int n) {
    int p = 1009;  // primo; n deve ser < p
    vector<int> v;
    for (int i = 1; i <= n; i++) v.push_back((i * 37) % p);
    return v;
}

// ============================================================================
// CENÁRIO 1 - Inserção em ordem crescente, decrescente e não ordenada
// ============================================================================

template <class A>
static void inserirSequenciaVerificando(const vector<int>& seq, const string& nome) {
    A a;
    set<int> esperado;
    for (size_t i = 0; i < seq.size(); i++) {
        a.inserir(seq[i]);
        esperado.insert(seq[i]);
        verificar(a, nome + " apos inserir " + to_string(seq[i]));
    }
    vector<int> ordenado(esperado.begin(), esperado.end());
    verificarConteudo(a, ordenado, nome);
    verificarAltura(a, (int)ordenado.size(), nome);
}

template <class A>
static void testeInsercaoCrescente() {
    inserirSequenciaVerificando<A>(sequenciaCrescente(500), "crescente");
}

template <class A>
static void testeInsercaoDecrescente() {
    inserirSequenciaVerificando<A>(sequenciaDecrescente(500), "decrescente");
}

template <class A>
static void testeInsercaoNaoOrdenada() {
    inserirSequenciaVerificando<A>(sequenciaNaoOrdenada(500), "nao ordenada (500 valores)");
    int pequena[] = {40, 10, 70, 20, 60, 30, 50};
    inserirSequenciaVerificando<A>(vector<int>(pequena, pequena + 7), "nao ordenada (7 valores)");
}

// ============================================================================
// CENÁRIO 2 - Busca de elementos existentes e inexistentes
// ============================================================================

template <class A>
static void testeBusca() {
    A a;
    for (int v = 10; v <= 100; v += 10) a.inserir(v);

    for (int v = 10; v <= 100; v += 10) {
        auto no = a.buscar(v);
        CHECK(no != nullptr, "busca deveria encontrar " + to_string(v));
        if (no != nullptr) CHECK(no->getValor() == v, "no retornado com valor errado");
    }
    int ausentes[] = {-1, 0, 5, 15, 55, 101, 999};
    for (int v : ausentes)
        CHECK(a.buscar(v) == nullptr, "busca NAO deveria encontrar " + to_string(v));

    verificar(a, "busca nao deve alterar a arvore");
}

// ============================================================================
// CENÁRIO 3 - Remoção de nós com diferentes configurações de filhos
// ============================================================================

template <class A>
static void testeRemocaoConfiguracoes() {
    A a;
    int base[] = {50, 30, 70, 20, 40, 60, 80};  // arvore completa de 7 nos
    for (int v : base) a.inserir(v);
    verificar(a, "arvore base");

    // (a) FOLHA: 20 nao tem filhos
    a.remover(20);
    CHECK(a.buscar(20) == nullptr, "20 deveria ter sido removido");
    verificarConteudo(a, {30, 40, 50, 60, 70, 80}, "remocao de folha");
    verificar(a, "apos remover folha 20");

    // (b) UM FILHO: agora 30 so possui o filho 40
    a.remover(30);
    CHECK(a.buscar(30) == nullptr, "30 deveria ter sido removido");
    verificarConteudo(a, {40, 50, 60, 70, 80}, "remocao de no com um filho");
    verificar(a, "apos remover no com um filho (30)");

    // (c) DOIS FILHOS + RAIZ: 50 tem subarvores esquerda e direita
    a.remover(50);
    CHECK(a.buscar(50) == nullptr, "50 (raiz) deveria ter sido removido");
    verificarConteudo(a, {40, 60, 70, 80}, "remocao da raiz com dois filhos");
    verificar(a, "apos remover raiz com dois filhos (50)");

    // (d) DOIS FILHOS interno
    a.remover(70);
    verificarConteudo(a, {40, 60, 80}, "remocao de no interno com dois filhos");
    verificar(a, "apos remover no interno com dois filhos (70)");

    // Esvazia o restante
    a.remover(40);
    a.remover(80);
    a.remover(60);
    verificarConteudo(a, {}, "arvore esvaziada");
    verificar(a, "arvore esvaziada");
}

// Remove todos os elementos em uma dada ordem, validando a cada passo
template <class A>
static void removerTudoVerificando(const vector<int>& insercao,
                                   const vector<int>& remocao,
                                   const string& nome) {
    A a;
    set<int> esperado;
    for (int v : insercao) { a.inserir(v); esperado.insert(v); }
    verificar(a, nome + " (estado inicial)");

    for (size_t i = 0; i < remocao.size(); i++) {
        a.remover(remocao[i]);
        esperado.erase(remocao[i]);
        verificar(a, nome + " apos remover " + to_string(remocao[i]));
        CHECK(a.buscar(remocao[i]) == nullptr,
              nome + ": " + to_string(remocao[i]) + " ainda encontrado apos remocao");
        // confere o conteudo completo a cada 10 remocoes (custo O(n))
        if (i % 10 == 0)
            verificarConteudo(a, vector<int>(esperado.begin(), esperado.end()),
                              nome + " conteudo");
    }
    CHECK(emOrdem(a).empty(), nome + ": arvore deveria terminar vazia");
}

template <class A>
static void testeRemocaoEmMassa() {
    vector<int> ins = sequenciaNaoOrdenada(300);
    vector<int> asc = ins;
    sort(asc.begin(), asc.end());
    vector<int> desc(asc.rbegin(), asc.rend());

    removerTudoVerificando<A>(ins, asc, "remocao crescente");
    removerTudoVerificando<A>(ins, desc, "remocao decrescente");
    removerTudoVerificando<A>(ins, ins, "remocao na ordem de insercao");

    vector<int> embaralhada = asc;
    mt19937 gen(2024);
    shuffle(embaralhada.begin(), embaralhada.end(), gen);
    removerTudoVerificando<A>(asc, embaralhada, "remocao embaralhada (insercao crescente)");
}

// ============================================================================
// CENÁRIO 4 - Remoção de elementos inexistentes
// ============================================================================

template <class A>
static void testeRemocaoInexistente() {
    A a;
    for (int v : {10, 20, 30}) a.inserir(v);

    int ausentes[] = {99, 0, 25, -5, 15};
    for (int v : ausentes) {
        a.remover(v);
        verificarConteudo(a, {10, 20, 30}, "remover inexistente " + to_string(v));
        verificar(a, "remover inexistente " + to_string(v));
    }

    // remover repetidamente o mesmo valor: so a primeira vez tem efeito
    a.remover(20);
    a.remover(20);
    verificarConteudo(a, {10, 30}, "remover 20 duas vezes");
    verificar(a, "remover 20 duas vezes");
}

// ============================================================================
// CENÁRIO 5 - Árvore vazia e árvore com um único elemento
// ============================================================================

template <class A>
static void testeArvoreVazia() {
    A a;
    verificar(a, "arvore recem-criada");
    CHECK(alturaReal(a) == 0, "altura da arvore vazia deveria ser 0");
    CHECK(emOrdem(a).empty(), "arvore vazia nao deveria ter elementos");
    CHECK(a.buscar(1) == nullptr, "busca em arvore vazia deveria retornar nullptr");

    a.remover(1);  // nao deve quebrar
    verificar(a, "remover em arvore vazia");
    CHECK(emOrdem(a).empty(), "arvore continua vazia apos remover");

    string saidaImprimir = capturarSaida([&]() { a.imprimir(); });
    CHECK(saidaImprimir.find("vazia") != string::npos,
          "imprimir em arvore vazia deveria avisar que esta vazia");

    string saidaPercurso = capturarSaida([&]() { a.percorrerEmOrdem(); });
    CHECK(saidaPercurso.find("Elementos em ordem:") != string::npos,
          "percurso em arvore vazia deveria imprimir o cabecalho");
}

template <class A>
static void testeUmElemento() {
    A a;
    a.inserir(42);
    verificar(a, "apos inserir unico elemento");
    verificarConteudo(a, {42}, "unico elemento");
    CHECK(alturaReal(a) == 1, "altura com um elemento deveria ser 1");
    CHECK(a.buscar(42) != nullptr, "deveria encontrar 42");
    CHECK(a.buscar(7) == nullptr, "nao deveria encontrar 7");

    a.inserir(42);  // duplicata
    verificarConteudo(a, {42}, "duplicata no unico elemento");

    string saida = capturarSaida([&]() { a.imprimir(); });
    CHECK(saida.find("42") != string::npos, "imprimir deveria mostrar o 42");

    a.remover(7);  // inexistente
    verificarConteudo(a, {42}, "remover inexistente com um elemento");

    a.remover(42);
    verificarConteudo(a, {}, "remover o unico elemento");
    verificar(a, "apos remover o unico elemento");
    CHECK(alturaReal(a) == 0, "altura deveria voltar a 0");

    a.inserir(7);  // a arvore deve continuar utilizavel
    verificarConteudo(a, {7}, "reinsercao apos esvaziar");
    verificar(a, "reinsercao apos esvaziar");
}

// ============================================================================
// CENÁRIO 6 - Valores repetidos
// ============================================================================

template <class A>
static void testeDuplicatas() {
    A a;
    for (int v = 1; v <= 20; v++) a.inserir(v);
    for (int v = 1; v <= 20; v++) a.inserir(v);  // todas repetidas
    verificarConteudo(a, sequenciaCrescente(20), "duplicatas ignoradas");
    verificar(a, "apos inserir duplicatas");
}

// ============================================================================
// CENÁRIO 7 - Propriedades após sequência mista de inserções e remoções
// ============================================================================

template <class A>
static void testeOperacoesMistas() {
    A a;
    set<int> oraculo;  // modelo de referencia (apenas para conferir conteudo)
    mt19937 gen(12345);
    uniform_int_distribution<int> valor(0, 199);
    uniform_int_distribution<int> moeda(0, 99);

    for (int op = 0; op < 3000; op++) {
        int v = valor(gen);
        if (moeda(gen) < 60) {
            a.inserir(v);
            oraculo.insert(v);
        } else {
            a.remover(v);
            oraculo.erase(v);
        }
        string erro;
        if (!validar(a, erro)) {
            CHECK(false, "operacao " + to_string(op) + ": " + erro);
            return;  // evita inundar a saida
        }
        if (emOrdem(a) != vector<int>(oraculo.begin(), oraculo.end())) {
            CHECK(false, "operacao " + to_string(op) + ": conteudo diverge do modelo");
            return;
        }
        CHECK((a.buscar(v) != nullptr) == (oraculo.count(v) == 1),
              "busca inconsistente com o modelo");
    }
    verificarAltura(a, (int)oraculo.size(), "operacoes mistas");
}

// ============================================================================
// Testes específicos da Rubro-Negra: cores
// ============================================================================

static void testeCoresRN() {
    ArvoreRubroNegra<int> a;
    a.inserir(10);
    CHECK(a.raiz->cor == PRETO, "raiz unica deve ser preta");

    a.inserir(20);
    CHECK(a.buscar(20)->cor == VERMELHO, "novo no deve ser vermelho");

    a.inserir(30);  // desbalanceio -> rotacao; 20 vira raiz
    CHECK(a.raiz->valor == 20, "apos 10,20,30 a raiz deveria ser 20");
    CHECK(a.raiz->cor == PRETO, "raiz 20 deveria ser preta");
    CHECK(a.buscar(10)->cor == VERMELHO && a.buscar(30)->cor == VERMELHO,
          "filhos 10 e 30 deveriam ser vermelhos");

    // tio vermelho -> recoloracao: inserir 5 pinta 10 e 30 de preto
    a.inserir(5);
    CHECK(a.buscar(10)->cor == PRETO && a.buscar(30)->cor == PRETO,
          "recoloracao: pai e tio deveriam ficar pretos");
    CHECK(a.buscar(5)->cor == VERMELHO, "no 5 deveria ser vermelho");
    verificar(a, "apos recoloracao");

    string saida = capturarSaida([&]() { a.imprimir(); });
    CHECK(saida.find("[P]") != string::npos && saida.find("[V]") != string::npos,
          "impressao deveria identificar as cores [P] e [V]");
}

// Verifica que, para entrada ordenada, a AVL fica estritamente
// logaritmica (nao degenera como uma BST comum)
template <class A>
static void testeNaoDegenera() {
    A a;
    for (int i = 1; i <= 1000; i++) a.inserir(i);
    CHECK(alturaReal(a) <= limiteAltura(a, 1000),
          "altura " + to_string(alturaReal(a)) + " com 1000 nos ordenados");
    CHECK(alturaReal(a) < 25, "uma BST comum teria altura 1000; esta tem " + to_string(alturaReal(a)));
}

// ============================================================================
// Execução das suítes
// ============================================================================

template <class A>
static void suite(const string& nome) {
    cout << "\n=== " << nome << " ===" << endl;
    executar("Insercao em ordem crescente", testeInsercaoCrescente<A>);
    executar("Insercao em ordem decrescente", testeInsercaoDecrescente<A>);
    executar("Insercao em ordem nao ordenada", testeInsercaoNaoOrdenada<A>);
    executar("Busca de elementos existentes e inexistentes", testeBusca<A>);
    executar("Remocao: folha, um filho, dois filhos e raiz", testeRemocaoConfiguracoes<A>);
    executar("Remocao em massa (varias ordens) com validacao", testeRemocaoEmMassa<A>);
    executar("Remocao de elementos inexistentes", testeRemocaoInexistente<A>);
    executar("Operacoes em arvore vazia", testeArvoreVazia<A>);
    executar("Operacoes com um unico elemento", testeUmElemento<A>);
    executar("Insercao de valores repetidos", testeDuplicatas<A>);
    executar("Sequencia mista de insercoes/remocoes (3000 ops)", testeOperacoesMistas<A>);
    executar("Entrada ordenada nao degenera a arvore", testeNaoDegenera<A>);
}

// ============================================================================
// Comparação AVL x Rubro-Negra (mesmos dados nas duas estruturas)
// ============================================================================

static void inserirTodos(ArvoreAVL<int>& a, const vector<int>& v) { for (int x : v) a.inserir(x); }
static void inserirTodos(ArvoreRubroNegra<int>& a, const vector<int>& v) { for (int x : v) a.inserir(x); }
static void removerTodos(ArvoreAVL<int>& a, const vector<int>& v) { for (int x : v) a.remover(x); }
static void removerTodos(ArvoreRubroNegra<int>& a, const vector<int>& v) { for (int x : v) a.remover(x); }

static void relatorioComparativo() {
    cout << "\n=== COMPARACAO DE ALTURAS (mesmas sequencias nas duas arvores) ===" << endl;
    cout << left << setw(26) << "Cenario" << setw(8) << "n"
         << setw(14) << "AVL (final)" << setw(14) << "RN (final)"
         << setw(16) << "AVL (-metade)" << setw(16) << "RN (-metade)"
         << "log2(n)" << endl;
    cout << string(100, '-') << endl;

    int tamanhos[] = {100, 1000, 10000};
    for (int n : tamanhos) {
        vector<int> crescente = sequenciaCrescente(n);
        vector<int> decrescente = sequenciaDecrescente(n);
        vector<int> aleatoria = sequenciaCrescente(n);
        mt19937 gen(7);
        shuffle(aleatoria.begin(), aleatoria.end(), gen);

        struct Caso { const char* nome; const vector<int>* seq; };
        Caso casos[] = {{"Crescente", &crescente},
                        {"Decrescente", &decrescente},
                        {"Aleatoria (seed 7)", &aleatoria}};

        for (const Caso& c : casos) {
            ArvoreAVL<int> avl;
            ArvoreRubroNegra<int> rn;
            inserirTodos(avl, *c.seq);
            inserirTodos(rn, *c.seq);
            int hAvl = alturaReal(avl), hRn = alturaReal(rn);

            // remove os valores pares (metade dos elementos) e mede de novo
            vector<int> pares;
            for (int x : *c.seq) if (x % 2 == 0) pares.push_back(x);
            removerTodos(avl, pares);
            removerTodos(rn, pares);

            cout << left << setw(26) << c.nome << setw(8) << n
                 << setw(14) << hAvl << setw(14) << hRn
                 << setw(16) << alturaReal(avl) << setw(16) << alturaReal(rn)
                 << fixed << setprecision(1) << log2((double)n) << endl;
        }
    }
    cout << "\n(Uma BST comum receberia altura n nos cenarios crescente/decrescente.)" << endl;
}

int main() {
    cout << "########## TESTES - ARVORES BALANCEADAS ##########" << endl;

    suite<ArvoreAVL<int> >("ARVORE AVL");
    suite<ArvoreRubroNegra<int> >("ARVORE RUBRO-NEGRA");

    cout << "\n=== TESTES ESPECIFICOS DA RUBRO-NEGRA (cores) ===" << endl;
    executar("Cores: raiz preta, novo no vermelho, recoloracao e rotacao", testeCoresRN);

    relatorioComparativo();

    cout << "\n##################################################" << endl;
    cout << "Verificacoes executadas: " << totalVerificacoes << endl;
    cout << "Falhas: " << totalFalhas << endl;
    cout << (totalFalhas == 0 ? "RESULTADO: TODOS OS TESTES PASSARAM"
                              : "RESULTADO: HA TESTES FALHANDO")
         << endl;
    return totalFalhas == 0 ? 0 : 1;
}
