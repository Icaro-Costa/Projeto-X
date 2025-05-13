# Projeto-X

## Equipe

* Icaro Costa - @Icaro-Costa
* Arthur Estevão - @ArthurEstevaum
* Alvaro Silva - @Alvaro5801
* Thays Costa - @idthy

## Informações da Disciplina

* **Disciplina:** Programação Imperativa e Funcional - 2025.1
* **Instituição:** CESAR School

## Sobre o Jogo

Projeto-X é um jogo de RPG de turnos desenvolvido em linguagem C utilizando a biblioteca CLI-lib para interação no terminal. O jogador assume o papel de um herói que deve enfrentar 5 inimigos em batalhas sequenciais para vencer.

O jogo implementa mecânicas de combate baseadas em atributos como HP, Ataque e Defesa, além de um sistema de "Raiva" que pode ser manipulado pelo jogador e afeta os inimigos de maneiras diferentes. As ações de combate utilizam um sistema de rolagem de dados (d20) para determinar o acerto dos ataques. Os status iniciais dos personagens são carregados a partir de arquivos de configuração na pasta `data/`.

## Como Jogar (Regras e Interação)

* Navegue pelos menus iniciais (Título, Dificuldade) usando as **Setas** e **Enter**.
* Em batalha, escolha suas ações a cada turno:
    * **Atacar:** Tenta causar dano ao inimigo baseado no seu Ataque e na rolagem d20 versus a Defesa do inimigo.
    * **Desdenhar/Taunt:** Aumenta a "Raiva" do inimigo, podendo causar efeitos variados (aumento de ataque, diminuição de defesa, ou até mesmo a derrota de um inimigo específico).
* Monitore seu HP e o HP do inimigo.
* Vença as 5 batalhas para completar o jogo!

## Compilação

**Pré-requisitos:**

* **Sistema Operacional:** Linux ou macOS (Este é um requisito da biblioteca CLI-lib).
    * *Nota: O desenvolvimento e testes foram realizados principalmente no **Ubuntu LTS** (ex: 22.04, 24.04). Outras distribuições Linux ou versões do macOS compatíveis devem funcionar.*
    * *Usuários de Windows precisarão usar uma Máquina Virtual (VM) com Linux ou o WSL (Windows Subsystem for Linux) para garantir a compatibilidade.*
* Compilador C (como `gcc`)
* Ferramenta `make`
* Biblioteca `CLI-lib` (arquivos devem estar no repositório).

**Passos para Compilar:**

1.  Clone o repositório:
    ```bash
    git clone [https://github.com/Icaro-Costa/Projeto-X](https://github.com/Icaro-Costa/Projeto-X)
    cd Projeto-X
    ```
2.  Execute o Makefile (ele deve compilar o código e colocar o executável na pasta `build/`):
    ```bash
    make
    ```

## Execução

Após a compilação bem-sucedida:

1.  Navegue até a pasta raiz do projeto (se já não estiver lá).
2.  Execute o jogo:
    ```bash
    ./build/projeto_x_rpg
    ```

## Estrutura do Projeto

/Projeto-X

├── src/        # Código-fonte (.c) dos módulos do jogo

├── include/    # Arquivos de cabeçalho (.h)

├── build/      # Saídas de compilação (.o, executáveis) - Ignorado pelo Git

├── README.md   # Este arquivo

├── Makefile    # Arquivo para automatizar a compilação

└── LICENSE     # Arquivo de licença (ex: MIT) 



## Ferramentas e Bibliotecas Utilizadas

* Linguagem: C (Padrão C99/C11, etc.)
* Compilador: GCC (ou outro compatível)
* Build System: Make
* Interface de Terminal: CLI-lib
