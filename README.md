Solar
============

Simulação de sistema solar
Trabalho Final do curso de Computação Gráfica
----------------------------------

### Introdução

O presente trabalho apresenta um simulador de sistema solar 
baseado em OpenGL e GLUT (freeglut), 
com escalas proporcionais, uso de texturas, animação de explosão, 
detecção de colisão, análise de desempenho 
assim como especificado pela descrição do trabalho proposto 
pelo professor Moisés Ramos.

### Instalação

O software foi elaborado no Eclipse, 
utilizando-se o compilador GCC e a biblioteca “freeglut”. 
Sua compatibilidade foi testada no Windows 7 (MinGW), 
OpenSuse 12.3 e Ubuntu 11.04. 
Para a compilação e “linkedição” no Linux, existe o programa “make.sh” 
(baseado na saída de console  construção do projeto do Eclipse)
. 

Obviamente, as bibliotecas do OpenGL e o “freeglut” devem estar instaladas 
para a criação do executável. 
Para o Windows, o executável foi “linkado” de forma estática e 
deve funcionar mesmo sem o “runtime” do “freeglut.dll”. 
Para a carga das texturas, que é opcional, 
o programa deve ser executado partindo-se 
de um diretório que contenha o subdiretório “texturas” com os bitmaps usados. 

### Como usar

- Teclas:
 - l: Liga/Desliga Luz
 - p: Pausa/Continua a movimentação (passagem do tempo)
 - q: Aumenta escala de raio dos planetas
 - a: Diminui escala de raio dos planetas
 - w: Aumenta distância dos planetas
 - s: Diminui distância dos planetas
 - z: Volta para escala padrão
 - x: Tamanho "ampliado"
 - o: Liga/Desliga órbitas
 - r: Volta os planetas explodidos
 - e: Ativa asteroide
 - d: Dispara/termina asteroide
 - c: Termina asteroide
 - t: Liga/Desliga textura
 - j: Finaliza programa


- Mouse:
 - Botão direito: Mover câmera
 - Botão esquerdo: Atirar asteroide e também mover câmera
 - Botão meio: Surpresinha ;)
 - Roda do mouse para frente: Aumenta zoom
 - Roda do mouse para trás: Diminui zoom

- Setas:
 - Cima: Aumenta zoom
 - Baixo: Diminui zoom
 - Direita: Diminui tempo
 - Esquerda: Aumenta tempo
 
- Focar:
 - 1: Sol
 - 2: Mercúrio
 - 3: Vênus
 - 4: Terra
 - !: Lua
 - 5: Marte
 - 6: Júpiter
 - 7: Saturno
 - @: Titan
 - #: Febe
 - $: Hiperion
 - 8: Urano
 - 9: Netuno
 - 0: Plutão
 - -: Asteroide

### AUTHORS
> ###### Guilherme Brandão Biber Sampaio <guilhermebiber@gmail.com>
