// -- Inclusão de bibliotecas
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "inc/ssd1306.h"
#include "inc/font.h"


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
#define led_pin_green 11 // Define o pino do LED Verde no GPIO 11
#define led_pin_blue 12 // Define o pino do LED Azul no GPIO 12

// Variáveis globais
volatile bool usb_conexao = true; // Auxilia a mensagem inicial aparecer uma única vez no início do programa
static volatile uint32_t last_time = 0; // Armazena o tempo do último clique dos botões 

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
            }
        }
        sleep_ms(50); //
    }
}
