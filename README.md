# O Mundo do Wumpus

![World of Wumpus](https://github.com/Durfan/ufsj-wumpus/blob/master/docs/peek.gif)

Trabalho prático desenvolvido para a disciplina de Inteligência Artificial da UFSJ.

>O Mundo de Wumpus é um  jogo antigo de computador considerado um domínio (ambiente) artificial que fornece grande motivação para o raciocínio lógico. Apesar de parecer um jogo muito simples quando comparado aos jogos modernos de computador, o Mundo de Wumpus é um excelente ambiente de teste para agentes inteligentes.(...) [ime.usp.br/~leliane](https://www.ime.usp.br/~leliane/IAcurso2000/Wumpus.html)

## Compilando

Compilar com raylib pre-instalado como library:

``` bash
make
make debug # binario com debug
make clean # remover binario
./wumpus # binario
```

Compilar com o bash script incluindo o path do raylib source :

``` bash
./build.sh
```

``` console
"Usage: ./build.sh [-hdusrcqq]"
-h  Show this information"
-d  Faster builds that have debug symbols, and enable warnings"
-u  Run upx* on the executable after compilation (before -r)"
-s  Run strip on the executable after compilation (before -r)"
-r  Run the executable after compilation"
-c  Remove the temp/(debug|release) directory, ie. full recompile"
-q  Suppress this script's informational prints"
-qq Suppress all prints, complete silence (> /dev/null 2>&1)"
```

## Dependências/libraries

[![raylib](https://github.com/Durfan/ufsj-wumpus/blob/master/docs/logo_raylib.png)](https://github.com/raysan5/raylib)
