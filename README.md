<div align="center">
    <img src="https://moodle.embarcatech.cepedi.org.br/pluginfile.php/1/theme_moove/logo/1733422525/Group%20658.png" alt="Logo Embarcatech" height="100">
</div>

<br>

# Comunicação serial - UART e I2C

## Sumário

- [Descrição](#descrição)
- [Funcionalidades Implementadas](#funcionalidades-implementadas)
- [Ferramentas utilizadas](#ferramentas-utilizadas)
- [Objetivos](#objetivos)
- [Instruções de uso](#instruções-de-uso)
- [Vídeo de apresentação](#vídeo-de-apresentação)
- [Aluno e desenvolvedor do projeto](#aluno-e-desenvolvedor-do-projeto)
- [Licensa](#licença)

## Descrição

Este projeto implementa um sistema interativo utilizando o microcontrolador Raspberry Pi Pico W, a placa de desenvolvimento BitDogLab, e um display OLED SSD1306 conectado via I2C. O projeto permite a entrada de caracteres via Monitor Serial do VS Code, que são exibidos no display OLED e, no caso de números de 0 a 9, representados visualmente, também, em uma matriz de LEDs WS2812. Além disso, dois botões físicos controlam o estado dos LEDs verde e azul, com mensagens informativas sendo exibidas no display.

## Funcionalidades Implementadas

1. Modificação da Biblioteca *font.h*:

   - Adição de caracteres minúsculos personalizados para serem utilizados no display SSD1306.

2. Entrada de Caracteres via UART:

   - O usuário pode digitar caracteres no Monitor Serial do VS Code.
   - O caractere digitado é exibido no display OLED SSD1306.
   - Se um número entre 0 e 9 for enviado, um símbolo correspondente será exibido na matriz 5x5 de LEDs WS2812.
  
3. Interação com o Botão A:

   - Pressionar o botão A alterna o estado do LED Verde do LED RGB.
   - A ação é registrada de duas formas:
     - Uma mensagem informativa é exibida no display OLED.
     - Um texto descritivo é enviado ao Monitor Serial.
    
4. Interação com o Botão B:

   - Pressionar o botão B alterna o estado do LED Azul do LED RGB.
   - A ação é registrada de duas formas:
     - Uma mensagem informativa é exibida no display OLED.
     - Um texto descritivo é enviado ao Monitor Serial.


## Ferramentas utilizadas

- **Simulador de eletrônica wokwi**: Ambiente utilizado para simular o hardware e validar o funcionamento do sistema.
- **Ferramenta educacional BitDogLab (versão 6.3)**: Placa de desenvolvimento utilizada para programar o microcontrolador.
- **Microcontrolador Raspberry Pi Pico W**: Responsável pelo controle da matriz de LEDs, botões, UART e I2C.
- **Matriz de LEDs RGB 5x5 (WS2812)**: Exibe símbolos numéricos correspondentes aos números digitados.
- **LED RGB**: Permite interação visual ao alternar as cores verde e azul por meio dos botões.
- **Botões físicos (2 unidades)**: Utilizados para alternar o estado dos LEDs RGB.
- **Display OLED SSD1306**: Exibe caracteres recebidos via UART e mensagens informativas.
- **Visual Studio Code (VS Code)**: IDE utilizada para o desenvolvimento do código com integração ao Pico SDK.
- **Pico SDK**: Kit de desenvolvimento de software utilizado para programar o Raspberry Pi Pico W em linguagem C.
- **Monitor serial do VS Code**: Ferramenta utilizada para enviar e monitorar caracteres via UART.

## Objetivos

1. Compreender o funcionamento e a aplicação de comunicação serial em microcontroladores.
2. Aplicar os conhecimentos adquiridos sobre UART e I2C na prática.
3. Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812.
4. Fixar o estudo do uso botões de acionamento, interrupções e Debounce.
5. Desenvolver um projeto funcional que combine hardware e software.


## Instruções de uso

1. **Clonar o Repositório**:

```bash
git clone https://github.com/bigodinhojf/embarcatech_tarefa_03_02.git
```

2. **Compilar e Carregar o Código**:
   No VS Code, configure o ambiente e compile o projeto com os comandos:

```bash	
cmake -G Ninja ..
ninja
```

3. **Interação com o Sistema**:
   - Conecte a placa ao computador.
   - Clique em run usando a extensão do raspberry pi pico.
   - Abra o monitor serial para fazer o envio de caracteres para visualização no display SSD1306 e acompanhar as mensagens informativas.
   - Clique os botões A e B para acender e apagar os LEDs verde e azul, visualize as mensagens informativas no monitor serial e no display.
   - Envie os caracteres de 0 a 9 para visualizar o símbolo correspondente na matriz de LEDs.

## Vídeo de apresentação

O vídeo apresentando o projeto pode ser assistido [clicando aqui](https://youtu.be/dop5YxkN4AU).

## Aluno e desenvolvedor do projeto

<a href="https://github.com/bigodinhojf">
        <img src="https://github.com/bigodinhojf.png" width="150px;" alt="João Felipe"/><br>
        <sub>
          <b>João Felipe</b>
        </sub>
</a>

## Licença

Este projeto está licenciado sob a licença MIT.
