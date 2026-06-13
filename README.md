# Lista de Pesquisa Operacional — 2026.1

**Disciplina:** Pesquisa Operacional  
**Professores:** Rian Pinheiro e Bruno Nogueira  
**Instituição:** UFAL — Universidade Federal de Alagoas  
**Período:** 2026.1  
**Alunos:** Plácido Cordeiro e Vinícius Cortez

---

## Estrutura

```
lista-po/
├── questao1/          # Questão 1 — Bin Packing com Busca Local (Python)
│   └── atividade1.py
└── questao2/          # Questão 2 — Modelagens PL/PLI com CPLEX (C++)
    ├── Makefile
    ├── 01_racao.cpp
    ├── 02_dieta.cpp
    ├── 03_plantio.cpp
    ├── 04_tintas.cpp
    ├── 05_transporte.cpp
    ├── 06_fluxo_maximo.cpp
    ├── 07_enfermeiras.cpp
    ├── 08_cobertura.cpp
    ├── 09_mochila.cpp
    ├── 10_clique.cpp
    ├── 11_padroes.cpp
    ├── 12_facilidades.cpp
    └── 13_frequencia.cpp
```

---

## Questão 1 — Bin Packing com Busca Local

Implementação em Python de uma meta-heurística de **busca local** para o problema de Bin Packing. A solução inicial é construída pela heurística **First Fit Decreasing (FFD)** e refinada por uma busca em vizinhança com estratégia *Best Improvement*.

### Como executar

```bash
cd questao1

# Com os itens padrão de exemplo
python atividade1.py

# Passando itens customizados
python atividade1.py --items "0.5,0.3,0.8,0.2,0.7"

# Definindo limite de tempo (padrão: 60s)
python atividade1.py --items "0.5,0.3,0.8" --time 30
```

---

## Questão 2 — Modelagens PL/PLI com CPLEX

13 problemas clássicos de Programação Linear e Inteira implementados em **C++ com a API Concert do IBM CPLEX**, baseados nos slides do OptLab/UFAL.

### Dependência

**IBM CPLEX Studio** deve estar instalado em `/opt/ibm/ILOG/CPLEX_Studio2211`.  
Se estiver em outro caminho, edite a variável `CPLEX_DIR` no `Makefile`.

### Compilação e execução

```bash
cd questao2

# Compilar todos os 13 problemas
make

# Compilar apenas um problema
make 09_mochila

# Rodar todos em sequência
make run

# Rodar um problema individualmente
./01_racao

# Remover os executáveis
make clean
```

### Problemas implementados

| Executável | Problema | Tipo |
|---|---|---|
| `01_racao` | Ração (AMGS e RE) | PL — max lucro |
| `02_dieta` | Dieta (6 ingredientes, 2 vitaminas) | PL — min custo |
| `03_plantio` | Plantio (3 fazendas × 3 culturas) | PL — max lucro |
| `04_tintas` | Tintas (mistura SR/SN) | PL — min custo |
| `05_transporte` | Transporte (3 fábricas × 3 depósitos) | PL — min custo |
| `06_fluxo_maximo` | Fluxo máximo (rede com 8 nós) | PL — max fluxo |
| `07_enfermeiras` | Escalonamento de enfermeiras (7 dias) | PLI — min pessoal |
| `08_cobertura` | Cobertura de conjuntos (escolas) | PLI 0-1 — min escolas |
| `09_mochila` | Mochila 0-1 (6 itens, W=10) | PLI 0-1 — max valor |
| `10_clique` | Clique máxima (grafo 6 vértices) | PLI 0-1 — max clique |
| `11_padroes` | Padrões de latinhas | PLI misto — max lucro |
| `12_facilidades` | Facility Location (3 depósitos, 4 clientes) | PLI 0-1 — min custo |
| `13_frequencia` | Frequência / Coloração de grafos (5 antenas) | PLI 0-1 — min frequências |
