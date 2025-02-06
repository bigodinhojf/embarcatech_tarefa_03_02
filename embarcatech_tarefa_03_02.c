// -- Inclusão de bibliotecas
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812.pio.h"


// -- Definição de constantes
// Display I2C
#define display_i2c_port i2c1 // Define a porta I2C
#define display_i2c_sda 14 // Define o pino SDA na GPIO 14
#define display_i2c_scl 15 // Define o pino SCL na GPIO 15
#define display_i2c_endereco 0x3C // Define o endereço do I2C
ssd1306_t ssd; // Inicializa a estrutura do display

// GPIO
#define button_pin_A 5 // Define o pino do botão A no GPIO 5
#define button_pin_B 6 // Define o pino do botão B no GPIO 6
#define matriz_leds 7 // Define o pino da matriz de LEDs no GPIO 7
#define NUM_LEDS 25 // Número de LEDs na matriz
#define led_pin_green 11 // Define o pino do LED Verde no GPIO 11
#define led_pin_blue 12 // Define o pino do LED Azul no GPIO 12

// Variáveis globais
volatile bool usb_conexao = true; // Auxilia a mensagem inicial aparecer uma única vez no início do programa
static volatile uint32_t last_time = 0; // Armazena o tempo do último clique dos botões 


// --- Funções necessária para a manipulação da matriz de LEDs
// Estrutura do pixel GRB (Padrão do WS2812)
struct pixel_t {
    uint8_t G, R, B; // Define variáveis de 8-bits (0 a 255) para armazenar a cor
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Permite declarar variáveis utilizando apenas "npLED_t"

// Declaração da Array que representa a matriz de LEDs
npLED_t leds[NUM_LEDS];

// Variáveis para máquina PIO
PIO np_pio;
uint sm;

// Função para definir a cor de um LED específico
void cor(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}

// Função para desligar todos os LEDs
void desliga() {
    for (uint i = 0; i < NUM_LEDS; ++i) {
        cor(i, 0, 0, 0);
    }
}

// Função para enviar o estado atual dos LEDs ao hardware  - buffer de saída
void buffer() {
    for (uint i = 0; i < NUM_LEDS; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    // sleep_us(100);
}

// Função para converter a posição da matriz para uma posição do vetor.
int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return 24-(y * 5 + x); // Linha par (esquerda para direita).
    } else {
        return 24-(y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

// Função que guarda os frames dos números
void num_matriz_leds(char num){
    desliga();
    switch (num){
        case '0': {
            // Frame 0
            int frame0[5][5][3] = {
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},    
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}}
            };
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                int posicao = getIndex(linha, coluna);
                cor(posicao, frame0[coluna][linha][0], frame0[coluna][linha][1], frame0[coluna][linha][2]);
                }
            };
            buffer();
            break;
        }
        case '1':{
            // Frame 1
            int frame1[5][5][3] = {
                {{0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}}
            };
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                int posicao = getIndex(linha, coluna);
                cor(posicao, frame1[coluna][linha][0], frame1[coluna][linha][1], frame1[coluna][linha][2]);
                }
            };
            buffer();
            break;
        }
        case '2':{
            // Frame 2
            int frame2[5][5][3] = {
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},        
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}}
            };
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                int posicao = getIndex(linha, coluna);
                cor(posicao, frame2[coluna][linha][0], frame2[coluna][linha][1], frame2[coluna][linha][2]);
                }
            };
            buffer();
            break;
        }
        case '3':{
            // Frame 3
            int frame3[5][5][3] = {
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}}
            };
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                int posicao = getIndex(linha, coluna);
                cor(posicao, frame3[coluna][linha][0], frame3[coluna][linha][1], frame3[coluna][linha][2]);
                }
            };
            buffer();
            break;
        }
        case '4':{
            // Frame 4
            int frame4[5][5][3] = {
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},    
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}} 
            };
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                int posicao = getIndex(linha, coluna);
                cor(posicao, frame4[coluna][linha][0], frame4[coluna][linha][1], frame4[coluna][linha][2]);
                }
            };
            buffer();
            break;
        }
        case '5':{
            // Frame 5
            int frame5[5][5][3] = {
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}}
            };
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                int posicao = getIndex(linha, coluna);
                cor(posicao, frame5[coluna][linha][0], frame5[coluna][linha][1], frame5[coluna][linha][2]);
                }
            };
            buffer();
            break;
        }
        case '6':{
            // Frame 6
            int frame6[5][5][3] = {
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},        
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},    
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}}
            };
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                int posicao = getIndex(linha, coluna);
                cor(posicao, frame6[coluna][linha][0], frame6[coluna][linha][1], frame6[coluna][linha][2]);
                }
            };
            buffer();
            break;
        }
        case '7':{
            // Frame 7
            int frame7[5][5][3] = {
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}} 
            };
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                int posicao = getIndex(linha, coluna);
                cor(posicao, frame7[coluna][linha][0], frame7[coluna][linha][1], frame7[coluna][linha][2]);
                }
            };
            buffer();
            break;
        }
        case '8':{
            // Frame 8
            int frame8[5][5][3] = {
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},    
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}}
            };
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                int posicao = getIndex(linha, coluna);
                cor(posicao, frame8[coluna][linha][0], frame8[coluna][linha][1], frame8[coluna][linha][2]);
                }
            };
            buffer();
            break;
        }
        case '9':{
            // Frame 9
            int frame9[5][5][3] = {
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 245, 255}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},    
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 245, 255}, {0, 0, 0}},       
                {{0, 0, 0}, {0, 245, 255}, {0, 245, 255}, {0, 245, 255}, {0, 0, 0}}
            };
            for (int linha = 0; linha < 5; linha++)
            {
                for (int coluna = 0; coluna < 5; coluna++)
                {
                int posicao = getIndex(linha, coluna);
                cor(posicao, frame9[coluna][linha][0], frame9[coluna][linha][1], frame9[coluna][linha][2]);
                }
            };
            buffer();
            break;
        }
        default:
            break;
    }
}

// Função de callback da interrupção dos botões
void gpio_irq_handler(uint gpio, uint32_t events){

    // Debouncing
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // Pega o tempo atual e transforma em us
    // 200ms
    if(current_time - last_time > 200000){
        last_time = current_time; // Atualização de tempo do último clique

        if(gpio == button_pin_A){
            gpio_put(led_pin_green, !gpio_get(led_pin_green));
            if(gpio_get(led_pin_green) == 1){
                printf("LED Verde Aceso!\n");
                ssd1306_draw_string(&ssd, "LED Green   On", 8, 40); // Desenha uma string
                ssd1306_send_data(&ssd); // Atualiza o display
            }else{
                printf("LED Verde apagado!\n");
                ssd1306_draw_string(&ssd, "LED Green  Off", 8, 40); // Desenha uma string
                ssd1306_send_data(&ssd); // Atualiza o display
            }
        }else if(gpio == button_pin_B){
            gpio_put(led_pin_blue, !gpio_get(led_pin_blue));
            if(gpio_get(led_pin_blue) == 1){
                printf("LED Azul aceso!\n");
                ssd1306_draw_string(&ssd, "LED Blue    On", 8, 52); // Desenha uma string
                ssd1306_send_data(&ssd); // Atualiza o display
            }else{
                printf("LED Azul apagado!\n");
                ssd1306_draw_string(&ssd, "LED Blue   Off", 8, 52); // Desenha uma string
                ssd1306_send_data(&ssd); // Atualiza o display
            }
        }
    }
}

int main()
{
    // -- Inicializações
    // Monitor serial
    stdio_init_all();

    // Display I2C
    i2c_init(display_i2c_port, 400 * 1000); // Inicializa o I2C usando 400kHz
    gpio_set_function(display_i2c_sda, GPIO_FUNC_I2C); // Define o pino SDA (GPIO 14) na função I2C
    gpio_set_function(display_i2c_scl, GPIO_FUNC_I2C); // Define o pino SCL (GPIO 15) na função I2C
    gpio_pull_up(display_i2c_sda); // Ativa o resistor de pull up para o pino SDA (GPIO 14)
    gpio_pull_up(display_i2c_scl); // Ativa o resistor de pull up para o pino SCL (GPIO 15)
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, display_i2c_endereco, display_i2c_port); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display
    ssd1306_fill(&ssd, false); // Limpa o display
    ssd1306_send_data(&ssd); // Atualiza o display

    // GPIO
    gpio_init(button_pin_A); // Inicia o botão A (GPIO 5)
    gpio_set_dir(button_pin_A, GPIO_IN); // Define o botão A (GPIO 5) como entrada
    gpio_pull_up(button_pin_A); // Ativa o resistor de pull up para o botão A (GPIO 5)

    gpio_init(button_pin_B); // Inicia o botão B (GPIO 6)
    gpio_set_dir(button_pin_B, GPIO_IN); // Define o botão B (GPIO 6) como entrada
    gpio_pull_up(button_pin_B); // Ativa o resistor de pull up para o botão B (GPIO 6)

    gpio_init(led_pin_green); // Inicia o LED Verde (GPIO 11)
    gpio_set_dir(led_pin_green, GPIO_OUT); // Define o LED Verde (GPIO 11) como saída

    gpio_init(led_pin_blue); // Inicia o LED Azul (GPIO 12)
    gpio_set_dir(led_pin_blue, GPIO_OUT); // Define o LED Azul (GPIO 12) como saída

    // PIO
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true);
    uint offset = pio_add_program(pio0, &ws2818b_program);
    ws2818b_program_init(np_pio, sm, offset, matriz_leds, 800000);
    desliga(); // Para limpar o buffer dos LEDs

    // Desenha o estado inicial do display
    ssd1306_draw_string(&ssd, "Tarefa   03 02", 8, 4); // Desenha uma string
    ssd1306_draw_string(&ssd, "Caractere  ", 8, 22); // Desenha uma string
    ssd1306_draw_string(&ssd, "LED Green  Off", 8, 40); // Desenha uma string
    ssd1306_draw_string(&ssd, "LED Blue   Off", 8, 52); // Desenha uma string
    ssd1306_send_data(&ssd); // Atualiza o display

    // Função de interrupção dos botões A e B
    gpio_set_irq_enabled_with_callback(button_pin_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_pin_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true) {
        if(stdio_usb_connected()){
            // Faz a mensagem inicial aparecer uma única vez no ínicio do programa, colocar o printf antes do While não está funcionando
            if(usb_conexao){
                printf("\n\n--- USB Conectado - Programa escutando ---\n - Envie um caractere por vez para aparecer no display \n - Entre 0-9, A-Z e a-z\n");
                usb_conexao = false;
            }
            // Recebe o caractere digitado mostra no monitor serial e desenha no display
            char c;
            if (scanf("%c", &c) == 1){
                printf("Caractere recebido: '%c'\n", c);
                ssd1306_draw_char(&ssd, c, 96, 22); // Desenha um caractere
                ssd1306_send_data(&ssd); // Atualiza o display
                if(c >= '0' && c <= '9'){
                    num_matriz_leds(c);
                }
            }
        }
        sleep_ms(50); //
    }
}
