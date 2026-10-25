# Amazon Engineering Challenge: Árvores Balanceadas (AVL e Rubro-Negra)

Trabalho da Unidade II — **IMD0039 · Estruturas de Dados Básicas II** (Instituto Metrópole Digital / UFRN)

| | |
|---|---|
| **Autor** | _Sabrina Bezerra da Silva_ |
| **Matrícula** | _20250039468_ |
| **Linguagem** | C++ (compilado com g++ 15.2.0, Ubuntu) |
| **Repositório** | _LINK PÚBLICO DO REPOSITÓRIO_ |

---

## 1. Sobre o projeto

Protótipo de um sistema de logística para a **Black Friday** da Amazon, responsável por **registrar, localizar e remover identificadores de encomendas** (IDs inteiros) com eficiência.

Em uma árvore binária de busca comum, a ordem de chegada dos dados pode degenerar a árvore em uma lista encadeada (por exemplo, IDs inseridos em ordem crescente), levando as operações a custo O(n). Para evitar isso, o projeto implementa **duas árvores de busca autobalanceadas, escritas do zero** (sem bibliotecas prontas de balanceamento):

- **Árvore AVL** — balanceamento rígido, controlado pelo fator de balanceamento de cada nó.
- **Árvore Rubro-Negra** — balanceamento relaxado, controlado por cores dos nós e rotações.

O usuário escolhe, em tempo de execução, qual das duas estruturas deseja utilizar.

---

## 2. Funcionalidades implementadas

Em **ambas** as árvores:

| Operação | Descrição |
|---|---|
| Inserção | Insere um ID e rebalanceia a árvore |
| Remoção | Remove um ID (folha, nó com um filho ou nó com dois filhos) e rebalanceia |
| Busca | Informa se o ID existe no sistema |
| Percurso em ordem | Lista os IDs em ordem crescente |
| Impressão da estrutura | Desenha a árvore no terminal |

Detalhes da visualização:

- **AVL:** cada nó mostra o valor, a altura `h` e o fator de balanceamento `fb`.
- **Rubro-Negra:** cada nó mostra o valor e a cor — `[V]` = vermelho, `[P]` = preto.

### Decisões de projeto (tratamento de casos especiais)

| Situação | Decisão adotada |
|---|---|
| Inserir valor **repetido** | A inserção é **ignorada**: a árvore não armazena duplicatas e permanece inalterada |
| Remover valor **inexistente** | Nenhuma alteração é feita na árvore (operação sem efeito) |
| Buscar valor **inexistente** | O sistema informa que o ID **não foi localizado** (a função retorna `nullptr`) |
| Operações em árvore **vazia** | Percurso exibe lista vazia; impressão exibe "Árvore vazia"; busca e remoção não têm efeito |
| Remoção de nó com **dois filhos** | O valor do nó é substituído pelo **sucessor em ordem** (menor valor da subárvore direita), que é então removido |

---

## 3. Estrutura do repositório

```
.
├── main.cpp                    # Menu interativo (escolha da árvore e das operações)
├── arvores/
│   ├── arvoreAVL.cpp           # Classe ArvoreAVL<T>
│   └── arvoreRubroNegra.cpp    # Classe ArvoreRubroNegra<T>
├── nos/
│   ├── noAVL.cpp               # Nó da AVL (valor, filhos, altura)
│   └── noRN.cpp                # Nó da Rubro-Negra (valor, filhos, pai, cor)
└── README.md
```

> O `main.cpp` inclui diretamente os arquivos `.cpp` das árvores (classes *template*), por isso **basta compilar apenas o `main.cpp`**.

---

## 4. Como compilar e executar

### Requisitos

- Compilador C++ com suporte a C++11 ou superior.
- Desenvolvido e testado com: `g++ (Ubuntu 15.2.0-16ubuntu1) 15.2.0`.
- Nenhuma biblioteca externa é necessária.

### Compilação

```bash
g++ -Wall -o programa main.cpp
```

### Execução

```bash
./programa          # Linux / macOS
programa.exe        # Windows
```

### Versão compilada

O executável já compilado está disponível na raiz do repositório: _INDICAR NOME DO ARQUIVO (ex.: `programa` / `programa.exe`)_.

---

## 5. Como usar

Ao iniciar, o **menu principal** permite escolher a estrutura:

```
===================================================
   AMAZON ENGINEERING CHALLENGE - TECH LEAD MENU
===================================================
Selecione o prototipo de arvore balanceada:
1. Arvore AVL
2. Arvore Rubro-Negra
0. Encerrar Sistema
```

Após a escolha, o **menu da árvore** oferece as operações:

```
1. Inserir identificador de encomenda
2. Remover identificador de encomenda
3. Buscar identificador de encomenda
4. Listar encomendas (Percurso em Ordem)
5. Exibir estrutura da Arvore (Balanceamento/Cores)
0. Voltar ao Menu Principal
```

Ao voltar ao menu principal, é possível escolher a outra árvore (cada escolha cria uma árvore nova e vazia).

### Exemplos de uso

**AVL** — inserindo 10, 20, 30 (ordem crescente, que degeneraria uma BST comum). A inserção do 30 provoca uma rotação simples à esquerda (caso RR) e a árvore resultante é:

```
Estrutura da Árvore AVL:
└── [Raiz] 20 (h=2, fb=0)
    ├── [Esq] 10 (h=1, fb=0)
    └── [Dir] 30 (h=1, fb=0)
```

**Rubro-Negra** — mesma sequência 10, 20, 30. A raiz é preta e os filhos, vermelhos:

```
Estrutura da Árvore Rubro-Negra ([V]=Vermelho, [P]=Preto):
└── [Raiz] 20 [P]
    ├── [Esq]  10 [V]
    └── [Dir]  30 [V]
```

**Busca de elemento inexistente:**

```
[Aviso] Encomenda ID 99 NAO localizada no sistema.
```

**Percurso em ordem:**

```
Elementos em ordem: 10 20 30
```

---

## 6. Explicação dos algoritmos

### 6.1 Árvore AVL (`arvores/arvoreAVL.cpp`)

- Cada nó guarda sua **altura** (folha = 1; subárvore vazia = 0).
- **Fator de balanceamento:** `fb = altura(esquerda) − altura(direita)`.
- A árvore é considerada balanceada quando `fb ∈ {-1, 0, 1}` em **todos** os nós.
- Após cada inserção ou remoção, a função `balancear` recalcula a altura do nó e, se `|fb| > 1`, aplica uma das quatro correções:

| Caso | Condição | Correção |
|---|---|---|
| **LL** | `fb > 1` e `fb(esquerda) ≥ 0` | Rotação simples à direita |
| **LR** | `fb > 1` e `fb(esquerda) < 0` | Rotação à esquerda no filho esquerdo + rotação à direita |
| **RR** | `fb < -1` e `fb(direita) ≤ 0` | Rotação simples à esquerda |
| **RL** | `fb < -1` e `fb(direita) > 0` | Rotação à direita no filho direito + rotação à esquerda |

- Inserção e remoção são **recursivas**: o rebalanceamento acontece no retorno da recursão, de baixo para cima, até a raiz.
- Complexidade: busca, inserção e remoção em **O(log n)**.

### 6.2 Árvore Rubro-Negra (`arvores/arvoreRubroNegra.cpp`)

Propriedades mantidas:

1. Todo nó é vermelho ou preto.
2. A raiz é preta.
3. Folhas (`NIL`) são pretas. A implementação usa um **nó sentinela único** (`NIL`) no lugar de ponteiros nulos.
4. Um nó vermelho não pode ter filho vermelho.
5. Todo caminho de um nó até suas folhas descendentes contém o **mesmo número de nós pretos** (altura negra).

Novos nós entram **vermelhos**. A implementação segue o esquema clássico de inserção/remoção com *fix-up* (Cormen et al.):

- **Inserção (`balancearInsercao`)** — enquanto o pai de `z` for vermelho:
  - **Tio vermelho:** recolore pai e tio para preto, avô para vermelho, e sobe o problema para o avô.
  - **Tio preto, `z` "joelho":** rotação no pai para transformar em caso linha.
  - **Tio preto, `z` em linha:** recolore pai/avô e faz uma rotação no avô.
  - No final, a raiz é pintada de preto.
- **Remoção (`remover` + `balancearRemocao`)** — usa `transplantar` para reconectar subárvores. Se a cor original do nó removido (ou do sucessor) era **preta**, a propriedade 5 pode ser violada e o *fix-up* trata os casos:
  1. Irmão vermelho → recolore e rotaciona;
  2. Irmão preto com dois sobrinhos pretos → recolore o irmão e sobe o problema;
  3. Irmão preto, sobrinho distante preto e próximo vermelho → rotação no irmão;
  4. Irmão preto com sobrinho distante vermelho → rotação no pai e recoloração final.
- Complexidade: busca, inserção e remoção em **O(log n)**; a altura é limitada a **2·log₂(n+1)**.

### 6.3 AVL × Rubro-Negra (resumo teórico)

| Aspecto | AVL | Rubro-Negra |
|---|---|---|
| Critério de balanceamento | Diferença de alturas ≤ 1 | Cores + altura negra |
| Altura máxima | ≈ 1,44·log₂(n) | ≤ 2·log₂(n+1) |
| Árvore mais "baixa" | Sim (mais rígida) | Não (mais relaxada) |
| Busca | Tende a ser mais rápida | Ligeiramente mais lenta |
| Inserção/remoção | Mais rotações em média | Menos rotações (até 2 na inserção, até 3 na remoção) |
| Informação extra no nó | Altura (ou fator) | 1 bit de cor (e ponteiro para o pai nesta implementação) |

---

## 7. Testes

Os testes podem ser compilados usando o seguinte comando:
```bash
g++ -std=c++11 -Wall -o testes testes.cpp
```
---

## 8. Declaração de Uso de IA
Eu utilizei IA para desenvolver o método de imprimir a estrutura da árvore no terminal, me ajudar a desenvolver os métodos de rotação, pois foi um assunto que fiquei muito em dúvida quando estudei a teoria, formatação do menu no método main (esse visual de ==== e etc), criação do arquivo de testes e escrita do README.md
 
