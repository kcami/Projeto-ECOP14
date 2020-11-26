#include "impressora3D.h"
#include "delay.h"

// Apos 25 segundos de uso o filamneto acaba
int quantFilamento = 0;

void inicializarDisplay(int d) {
    int i;
    for (i = 2; i < 6; i++) {
        if (i == d) {
            BitSet(PORTA, i);
        } else {
            BitClr(PORTA, i);
        }
    }
}

void tempoCooler(int t) {
    // Vetor com os valores em hexadecimal correspondes aos numeros de 0 a 9
    int values7seg[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
    int i, j, k;
    int unsigned delay = 2000;
    int cont = t;
    BitSet(ADCON1, 3);
    
    // Repete a quantidade de segundos passada pelo parametro t
    for (j = 0; j < t; j++) {
        // Uso rápido de cada display para gerar a impressao que todos estao ligados 
        for (i = 0; i < 35; i++) {
            PORTD = values7seg[(cont) % 10];
            inicializarDisplay(5);
            for (k = 0; k < delay; k++);

            PORTD = values7seg[(cont / 10) % 10];
            inicializarDisplay(4);
            for (k = 0; k < delay; k++);

            PORTD = values7seg[(cont / 100) % 10];
            inicializarDisplay(3);
            for (k = 0; k < delay; k++);

            PORTD = values7seg[(cont / 1000) % 10];
            inicializarDisplay(2);
            for (k = 0; k < delay; k++);

        }
        // Condicoes para ligar e desligar o cooler
        if ((j + 2) % 5 == 0) PORTC = BitSet(PORTC, 2); // Liga faltando 2 segundos para um múltiplo de 5
        if (j % 5 == 0)PORTC = BitClr(PORTC, 2); // Desliga quando chega no múltiplo de 5
        cont--;
    }
    // Retornando as condicoes iniciais
    BitClr(ADCON1, 3);
    PORTC = 0x00;
    PORTA = 0x00;
}

void InicializarImpressora(void) {
    int i;
    // Todos os LEDs estao apagados 
    PORTB = 0x00; 
    // Vetor com os valores para ligar os LEDs
    int sequencia[8] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
    // Ligando um LED por vez depois de um segundo
    for (i = 0; i < 8; i++) {
        PORTB = sequencia[i];
        atraso_ms(1000);
    }
    atraso_ms(1000);
}

unsigned char opcaoEscolhida() {
    unsigned int atrasoMin = 20;
    unsigned char i;

    TRISB = 0xF8; // teclado numerico
    TRISD = 0x0F;
    // Guarda a opcao feita pelo teclado
    unsigned char opcao;
    opcao = tc_tecla(0) + 0x30;
    // Retornar para as configurações iniciais
    TRISD = 0x00; 
    TRISB = 0x00;
    // Devolve a opcao escolhida 
    return opcao;
}

int ativarRele(int t) {
    unsigned char opcao = ' ';
    // Para cada modelo, soma-se o tempo que levou (considerando a quantidade de filamento usado por segundo)
    quantFilamento += t;
    int i;
    // Caso a quantidade de filamento não acabou
    if (quantFilamento < 25) {
        ADCON1 = 0x06;
        PORTE = 0x01;
        atraso_ms(500);
        tempoCooler(t);
        PORTE = 0x00;
        //liga e desliga o buzzer 
        BitClr(PORTC, 1);
        atraso_ms(3000);
        BitSet(PORTC, 1);
        // Retorna que o rele foi ligado
        return 1;
    } else { // Quando o filamento acaba
        lcd_cmd(L_CLR);
        lcd_cmd(L_L1);
        lcd_str("Nao ha filamentoQuando preencher");
        lcd_cmd(L_L2);
        lcd_str("suficente       aperte o 9");
        // Só aceita a opcao '9' como resposta
        while (opcao != '9') {
            // Liga o buzzer intermitente
            for (i = 0; i < 4; i++) {
                BitClr(PORTC, 1);
                atraso_ms(1000);
                BitSet(PORTC, 1);
                atraso_ms(500);
            }
            opcao = opcaoEscolhida();
        }
        // A quantidade de filamento retorna a 0
        quantFilamento = 0;
        // Retorna que o rele foi desligado
        return 0;
    }

}

void lcdMensagem(int opcao) {
    int i;   
    switch (opcao) {
        case '1':
            lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("1-Caixa 16seg");
            break;
        case '2':
            lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("2-Parafuso 4seg");       
            break;
        case '3':
            lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("3-Chaveiro 10seg");     
            break;
        default:
            break;
    }
}

void deslocarCursor(){
    int i;
    unsigned int atrasoMed = 150;
    for (i = 0; i < 15; i++) {
        atraso_ms(atrasoMed);
        lcd_cmd(0x10);
    }
    for (i = 0; i < 15; i++) {
        atraso_ms(atrasoMed);
        lcd_cmd(0x14);
    }
}

void desligarImpressora(){
    int i;
    //configurando o PORTB
    PORTB = 0x00; // Todos estao apagados 
    // Vetor com os valores para ligar os LEDs
    int sequencia[8] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
    // Ligando um LED por vez depois de um intervalo de tempo
    for (i = 8; i > 0; i--) {
        PORTB = sequencia[i];
        atraso_ms(1000);
    }
    atraso_ms(1000);
}