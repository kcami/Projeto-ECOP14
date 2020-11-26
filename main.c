#include "impressora3D.h"
#define t1 16
#define t2 4
#define t3 10

void main(void) {
    unsigned char opcao;
    int Rfuncionou = 1;
    //portas utilizadas pelo LCD
    ADCON1 = 0x06;
    TRISD = 0x00;
    TRISE = 0x00;
    //porta utilizada pela tecla
    TRISB = 0x00;
    //porta utilizada pelo cooler
    TRISC = 0x00;
    BitSet(PORTC, 1);
    //porta utilizado pelos 4 displays de 7 segmentos
    TRISA = 0x00;
    // Loop para ligar e desligar a impressora
    while (1) {
        opcao = -1;
        BitSet(PORTC, 1);
        lcd_init();
        lcd_cmd(L_CLR);
        lcd_cmd(L_L1);

        lcd_str("Digite 0 para ");
        lcd_cmd(L_L2);
        lcd_str("ligar");

        opcao = opcaoEscolhida();
        atraso_ms(1000);
        // Caso acione o botao de inicializacao
        if (opcao == '0') {
            lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("Inicializando");
            deslocarCursor();
            InicializarImpressora();
        }
        
        // Loop enquanto a impressora estiver ligada
        while (opcao != '4') {
            lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("Escolha a opcao");
            deslocarCursor();
            atraso_ms(2000);

            lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("1-Caixa 16seg   3-Chaveiro 10seg");
            lcd_cmd(L_L2);
            lcd_str("2-Parafuso 4seg 4-Desligar");
            opcao = opcaoEscolhida();

            // Casos para cada opcao escolhida pelo usuario
            switch (opcao) {
                case '1':
                    lcdMensagem(opcao);
                    deslocarCursor();
                    Rfuncionou = ativarRele(t1);
                    // Caso o rele nao foi ligado
                    if (Rfuncionou == 0) {
                        lcdMensagem(opcao);
                        ativarRele(t1);
                    }
                    break;
                case '2':
                    lcdMensagem(opcao);
                    deslocarCursor();
                    Rfuncionou = ativarRele(t2);
                    // Caso o rele nao foi ligado
                    if (Rfuncionou == 0) {
                        lcdMensagem(opcao);
                        ativarRele(t2);
                    }
                    break;
                case '3':
                    lcdMensagem(opcao);
                    deslocarCursor();
                    Rfuncionou = ativarRele(t3);
                    // Caso o rele nao foi ligado
                    if (Rfuncionou == 0) {
                        lcdMensagem(opcao);
                        ativarRele(t3);
                    }
                    break;
                case '4':
                    lcd_cmd(L_CLR);
                    lcd_cmd(L_L1);
                    lcd_str("Desligando");
                    deslocarCursor();
                    desligarImpressora();
                    PORTB = 0x00;
                    atraso_ms(3000);
                    break;
                default:
                    lcd_cmd(L_CLR);
                    lcd_cmd(L_L1);
                    lcd_str("Opcao invalida");
                    deslocarCursor();
            }
        }
    }
    
}
