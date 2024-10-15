# Jogo Palavras-Cruzadas utilizando CSP

Este projeto é um jogo de palavras-cruzadas resolvido como um problema de Satisfação de Restrições (CSP). O jogo consiste em preencher um grid de palavras cruzadas usando um banco de palavras fornecido em um arquivo de texto (localizado na pasta `./resources`). O projeto também utiliza grids de palavras-cruzadas obtidos do site [wordsup.co.uk](https://wordsup.co.uk).

## Crosswords V1
Na primeira versão do algoritmo, a solução ocorre de forma direta, atribuindo palavras do banco ao grid de palavras-cruzadas conforme as restrições de posição e encaixe. No entanto, essa versão apresenta uma limitação importante: o algoritmo espera que as palavras fornecidas no banco sejam exatamente as que cabem no grid. Caso isso não aconteça, o algoritmo entra em um ciclo de recursão excessiva, tentando encaixar todas as palavras, mesmo que não sejam adequadas, o que pode causar travamentos ou resultados incorretos.

Essa versão não lida bem com situações em que o banco de palavras contém palavras extras ou insuficientes para preencher o grid, ou quando existem múltiplas soluções possíveis. Isso exigiu uma nova abordagem, levando à implementação da **Crosswords V2**.

### Implementação

A implementação da **Crosswords V1** segue o fluxo básico de um problema de CSP. O algoritmo lê o grid e o banco de palavras, e em seguida tenta alocar as palavras no grid de acordo com as restrições de posição e orientação (horizontal ou vertical). Para cada espaço vazio no grid, ele tenta encaixar uma palavra do banco que tenha o tamanho correto.

A lógica de verificação de restrições nesta versão é simples: o algoritmo checa se as palavras se encaixam nas interseções e se têm o tamanho adequado para o espaço disponível. Quando uma palavra não se encaixa, o algoritmo tenta retroceder e testar outra palavra (backtracking).

## Crosswords V2
Na segunda versão, o algoritmo foi aprimorado para lidar de maneira mais flexível com as palavras do banco. O principal diferencial é a implementação de uma lógica que filtra as palavras do banco com base nas restrições do grid antes de tentar alocá-las. Agora, o algoritmo evita a recursão desnecessária ao realizar verificações antecipadas que garantem que apenas palavras possíveis sejam testadas no grid.

Essa versão também introduz a capacidade de lidar com cenários onde o banco de palavras pode conter palavras extras que não se encaixam, ou menos palavras que o necessário, garantindo que o grid seja preenchido apenas com palavras que se ajustem às restrições e possibilidades de encaixe.

No entanto, essa solução apresentou problemas tanto na complexidade do algoritmo quanto na exigência de ter uma letra inicial já preenchida no grid para começar a alocação das palavras. Se não houver nenhuma letra no grid, o algoritmo retorna que não há solução. Além disso, o algoritmo funciona corretamente apenas na IDE Code Blocks no Windows. Ao utilizá-lo no VS Code em um ambiente Linux, ocorre o erro 'Illegal instruction (core dumped)', e no VS Code no Windows, o erro 'Segmentation fault (core dumped)' é gerado.

### Implementação

A **Crosswords V2** foi implementada com várias melhorias em relação à primeira versão. O principal avanço foi a introdução de um pré-processamento para filtrar o banco de palavras antes da tentativa de alocação no grid.

1. **Filtragem de palavras**: Antes de tentar alocar as palavras no grid, o algoritmo realiza uma análise do grid para identificar os tamanhos e orientações necessários para cada espaço. Em seguida, ele filtra o banco de palavras para garantir que apenas palavras com o tamanho correto sejam consideradas para cada espaço específico. Isso reduz drasticamente o número de tentativas inválidas e elimina a necessidade de recursão desnecessária.

2. **Verificação antecipada de interseções**: Outra melhoria foi a implementação de uma verificação antecipada das interseções de palavras. Em vez de simplesmente tentar encaixar palavras e verificar a validade depois, o algoritmo agora analisa antecipadamente se uma palavra pode se encaixar em uma posição, considerando também as interseções com outras palavras já alocadas.

3. **Backtracking otimizado**: O processo de backtracking foi otimizado para evitar tentativas redundantes. Se o algoritmo percebe que não há palavras compatíveis para uma posição específica, ele imediatamente retrocede e tenta outra combinação sem recorrer a tentativas repetitivas.
