//Nomes: Felipe Miranda - Geovanne Nery - Nickolas Corrêa - Victor Rico

#define f_CPU 16000000
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

void InitAdc()
{
  ADMUX |= (1<<REFS0);
  ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADEN); //Habilitando o ADC e definindo prescaler para 128.
}

uint16_t ReadAdc(uint8_t ADCchanel)
{
  ADMUX = (ADMUX & 0xF0) | (ADCchanel & 0x0F);
  ADCSRA |= (1<<ADSC); //Single mode.
  
  while (ADCSRA & (1<<ADSC));
  return ADC;
}

int main(void){
  
  DDRD |= (1<<PD6); //Define a porta D6 como saída.
  DDRD |= (1<<PD5); //Define a porta D5 como saída.
  
  OCR0A = 172;  //Define o ciclo de operação.
  OCR0B = 172;  //Define o ciclo de operação.
  
  TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00); //Non-inverting fast pwm modo 3 Motor Esquerda.
  TCCR0B |= (1<<CS00); //No prescaler.
  
  TCCR0A |= (1<<COM0B1) | (1<<WGM01) | (1<<WGM00); //Non-inverting fast pwm modo 3 Motor Direita.
  TCCR1B |= (1<<CS00); //No prescaler.
  
  InitAdc();
  
  while(1){
    
    Serial.print("motorEsq: ");
    Serial.println(ReadAdc(0));
    Serial.print("motorDir: ");
    Serial.println(ReadAdc(1));
//    delay(5000);
    
    if (ReadAdc(0) > 500 && ReadAdc(1) > 500){ //Sensor Direita e Esquerda = Preto
      //Anda Reto
      OCR0A = 80;
      OCR0B = 80;
    }
    else if (ReadAdc(0) < 500 && ReadAdc(1) < 500){ //Sensor Direita e Esquerda = Branco
      //Parado
      _delay_ms(1000);
      OCR0A = 0;
      OCR0B = 0;
    }
    
    else if (ReadAdc(0) >= 500 && ReadAdc(1) < 500){ //Sensor da direita = **** | Sensor da esquerda = ****
      //Vira Direita 
      OCR0A = 150;
      OCR0B = 40;
    }
    else if (ReadAdc(0) >= 500 && ReadAdc(1) < 500){ //Sensor da direita = **** | Sensor da esquerda = ****
      //Vira Esquerda
      OCR0A = 40;
      OCR0B = 150;
    }
  }
}