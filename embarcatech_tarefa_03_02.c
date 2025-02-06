// -- Inclusão de bibliotecas
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "hardware/pio.h"

// -- Definição de constantes
// Display I2C
#define display_i2c_port i2c1 // Define a porta I2C
#define display_i2c_sda 14 // Define o pino SDA na GPIO 14
#define display_i2c_scl 15 // Define o pino SCL na GPIO 15
#define display_i2c_endereco 0x3C // Define o endereço do I2C

volatile bool usb_conexao = true; // Auxilia a mensagem inicial aparecer uma única vez no início do programa

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
    ssd1306_t ssd; // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, display_i2c_endereco, display_i2c_port); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display
    ssd1306_fill(&ssd, false); // Limpa o display
    ssd1306_send_data(&ssd); // Atualiza o display

    ssd1306_draw_string(&ssd, "Tarefa   03 02", 8, 4); // Desenha uma string
    ssd1306_draw_string(&ssd, "Caractere  ", 8, 22); // Desenha uma string
    ssd1306_draw_string(&ssd, "LED Green  Off", 8, 40); // Desenha uma string
    ssd1306_draw_string(&ssd, "LED Blue   Off", 8, 52); // Desenha uma string
    ssd1306_send_data(&ssd); // Atualiza o display

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
