#include "config.h"
#include "lcd.h"
#include "pic18f4520.h"
#include "teclado.h"
#include "delay.h"

#ifndef IMPRESSORA3D_H
#define	IMPRESSORA3D_H
    // Liga os LEDs da impressora simulando sua inicializacao
    void InicializarImpressora(); 
    // Recebe o valor digitado pelas teclas e retorna na main
    unsigned char opcaoEscolhida();
    // Inicializa os displays do 7 segmentos (PORTA)
    void inicializarDisplay(int d);
    // Contabiliza o tempo por meio dos displays de 7 segmentos e chama o cooler
    void tempoCooler();
    // Ativa o rele que simula a impressora trabalhando
    int ativarRele(int);
    // Coloca no lcd as mensagens relacionadas aos produtos feitos pela impressora
    void lcdMensagem(int);
    // Funcao para deslocar o cursor do lcd
    void deslocarCursor();
    // Simula o desligamento da impressora (desliga os LEDs)
    void desligarImpressora();
#endif	/* IMPRESSORA3D_H */

