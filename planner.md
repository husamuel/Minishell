## Estrutura do projeto
```
minishell/
├── includes/           # Cabeçalhos (arquivos .h)
│   ├── minishell.h     # Cabeçalho principal do projeto
│   ├── builtins.h      # Declarações relacionadas aos builtins
│   ├── parser.h        # Declarações do parser
│   ├── executor.h      # Declarações do executor
│   ├── environment.h   # Manipulação de variáveis de ambiente
│   ├── signals.h       # Tratamento de sinais (Ctrl+C, Ctrl+D, etc.)
│   └── utils.h         # Funções auxiliares e genéricas
├── src/                # Código fonte principal
│   ├── main.c          # Entrada principal do programa
│   ├── builtins/       # Implementação dos builtins
│   │   ├── cd.c        # Comando `cd`
│   │   ├── echo.c      # Comando `echo`
│   │   ├── env.c       # Comando `env`
│   │   ├── exit.c      # Comando `exit`
│   │   ├── export.c    # Comando `export`
│   │   └── unset.c     # Comando `unset`
│   ├── parser/         # Parsing da entrada do usuário
│   │   ├── lexer.c     # Tokenização da entrada
│   │   ├── parser.c    # Análise sintática
│   │   └── syntax.c    # Validação de sintaxe
│   ├── executor/       # Execução de comandos
│   │   ├── executor.c  # Execução principal
│   │   ├── pipes.c     # Gerenciamento de pipes
│   │   └── redirections.c # Gerenciamento de redirecionamentos
│   ├── environment/    # Manipulação de variáveis de ambiente
│   │   ├── env_utils.c # Funções auxiliares para env
│   │   └── env_list.c  # Manipulação de lista de variáveis de ambiente
│   ├── signals/        # Tratamento de sinais
│   │   ├── signals.c   # Captura e gerenciamento de sinais
│   └── utils/          # Funções utilitárias
│       ├── str_utils.c # Manipulação de strings
│       ├── error.c     # Mensagens de erro
│       └── free.c      # Gerenciamento de memória
├── lib/                # Bibliotecas externas ou funções customizadas
│   ├── libft/          # Sua implementação da libft (ou versão reduzida dela)
│   └── readline/       # Readline, se necessário (caso sua máquina precise de instalação local)
├── Makefile            # Compilação do projeto
├── README.md           # Documentação do projeto
```

# Checklist Completa para o Projeto Minishell (42)

## 1. Configuração Inicial
- [x] **Criar estrutura de pastas e arquivos**
  - [x] Criar pastas: `includes/`, `src/`, `src/builtins/`, `src/parser/`, `src/executor/`, `src/environment/`, `src/signals/`, `src/utils/`, `lib/`.
  - [x] Adicionar `Makefile`.
  - [x] Criar arquivos básicos: `main.c`, `minishell.h`.

- [ ] **Configurar o Makefile**
  - [ ] Compilar todos os arquivos `.c` para um executável `minishell`.
  - [ ] Adicionar regras para:
    - [ ] `all` (compilar o projeto).
    - [x] `clean` (remover arquivos `.o`).
    - [x] `fclean` (remover binários e objetos).
    - [x] `re` (recompilar do zero).
  - [ ] Linkar a biblioteca `readline`.

---

## 2. Loop Principal do Shell
- [x] Implementar o loop principal no `main.c`:
  - [x] Exibir um prompt (`minishell>`).
  - [x] Usar `readline` para capturar a entrada do usuário.
  - [x] Adicionar histórico com `add_history()` do readline.
  - [x] Implementar o comando `EXIT` para sair do shell.
  - [x] Testar comportamento básico do prompt.

---

## 3. Parsing da Entrada
- [ ] **Tokenização (Lexer)**
  - [ ] Dividir a entrada do usuário em tokens:
    - [ ] Separar comandos, argumentos, pipes (`|`), redirecionamentos (`>`, `<`).
    - [ ] Tratar aspas simples (`'`) e duplas (`"`).
    - [ ] Expandir variáveis de ambiente (`$VAR`).
  - [ ] Validar tokens (e.g., não permitir `| |` ou redirecionamentos inválidos).

- [ ] **Estrutura de Comandos**
  - [ ] Criar uma estrutura para armazenar:
    - [ ] Nome do comando.
    - [ ] Lista de argumentos.
    - [ ] Tipo de redirecionamento.
    - [ ] Informações sobre pipes.
  - [ ] Converter os tokens na estrutura de comandos.

---

## 4. Implementação dos Builtins
- [ ] **Comandos Builtins**
  - [ ] `echo`:
    - [ ] Suportar a flag `-n`.
    - [ ] Testar com diferentes argumentos.
  - [ ] `cd`:
    - [ ] Mudar o diretório atual.
    - [ ] Lidar com erros (e.g., diretório inexistente).
  - [ ] `pwd`:
    - [ ] Imprimir o diretório atual.
  - [ ] `export`:
    - [ ] Adicionar ou modificar variáveis de ambiente.
    - [ ] Lidar com casos inválidos (e.g., nomes inválidos de variáveis).
  - [ ] `unset`:
    - [ ] Remover variáveis de ambiente.
    - [ ] Garantir que variáveis inexistentes não causem erro.
  - [ ] `env`:
    - [ ] Exibir todas as variáveis de ambiente.
  - [ ] `exit`:
    - [ ] Fechar o shell com um código de saída opcional.

---

## 5. Execução de Comandos
- [ ] **Comandos Simples**
  - [ ] Executar comandos externos usando `execve`.
  - [ ] Tratar erros (e.g., comando não encontrado).

- [ ] **Redirecionamentos**
  - [ ] `>`: Redirecionar saída para um arquivo (criar ou sobrescrever).
  - [ ] `>>`: Redirecionar saída para um arquivo (append).
  - [ ] `<`: Redirecionar entrada a partir de um arquivo.
  - [ ] `<<`: Here-doc (ler até encontrar uma string específica).

- [ ] **Pipes**
  - [ ] Suportar múltiplos pipes (`cmd1 | cmd2 | cmd3`).
  - [ ] Garantir que cada comando em um pipeline funcione corretamente.

---

## 6. Variáveis de Ambiente
- [ ] **Manipulação de Variáveis**
  - [ ] Implementar funções para:
    - [ ] Buscar variáveis (`getenv` ou similar).
    - [ ] Adicionar/modificar variáveis (`export`).
    - [ ] Remover variáveis (`unset`).

- [ ] **Expansão de Variáveis**
  - [ ] Expandir variáveis com `$` (e.g., `$HOME`).
  - [ ] Tratar casos como `$?` (status do último comando).

---

## 7. Tratamento de Sinais
- [ ] Implementar captura de sinais:
  - [ ] `Ctrl+C`: Exibir nova linha de prompt sem encerrar o shell.
  - [ ] `Ctrl+D`: Sair do shell.
  - [ ] `Ctrl+\`: Ignorar (não fazer nada).
- [ ] Configurar sinais para subprocessos:
  - [ ] Garantir que `Ctrl+C` e outros sinais funcionem corretamente nos subprocessos.

---

## 8. Validações e Erros
- [ ] **Validações de Sintaxe**
  - [ ] Detectar erros como:
    - [ ] `|` no início ou no final.
    - [ ] Redirecionamentos incompletos (`>`, `<` sem arquivo).
  - [ ] Exibir mensagens de erro claras.

- [ ] **Tratamento de Erros**
  - [ ] Comando não encontrado.
  - [ ] Diretório inexistente no `cd`.
  - [ ] Falhas de redirecionamento (arquivo não encontrado ou permissões).
  - [ ] Variáveis de ambiente inválidas.

---

## 9. Otimização e Refatoração
- [ ] Revisar o código para garantir:
  - [ ] Separação clara entre parsing, execução e manipulação de ambiente.
  - [ ] Redução de duplicação de código.
  - [ ] Uso eficiente de memória.
- [ ] Adicionar comentários claros.
- [ ] Testar extensivamente com diferentes casos.

---

## 10. Testes e Debug
- [ ] **Testes Manuais**
  - [ ] Comandos simples (`ls`, `echo`, etc.).
  - [ ] Comandos com pipes e redirecionamentos.
  - [ ] Comandos builtins com argumentos inválidos.
  - [ ] Variáveis de ambiente (`export`, `$VAR`, etc.).
  - [ ] Comportamento com sinais (`Ctrl+C`, `Ctrl+D`).

- [ ] **Testes Automatizados (Opcional)**
  - [ ] Criar um script para testar cenários comuns e edge cases.

---

## 11. Finalização
- [ ] Atualizar o `README.md` com:
  - [ ] Descrição do projeto.
  - [ ] Instruções de uso.
  - [ ] Dependências necessárias (e.g., `readline`).
- [ ] Verificar que o `Makefile` funciona corretamente.
- [ ] Revisar o código para cumprir todas as normas do projeto.

