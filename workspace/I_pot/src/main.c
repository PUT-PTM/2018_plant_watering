#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
volatile double Uin=0;
int volatile ADC_Result=0;
volatile int glob=0,iter=0,status_pompy=0,liczba,j=0,liczba2=0;



void wyswietlacz_Led(){//LED
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	 GPIO_InitTypeDef  GPIO_InitStructure;
      GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 |GPIO_Pin_13| GPIO_Pin_14 | GPIO_Pin_15;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
          GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
          GPIO_Init(GPIOB, &GPIO_InitStructure);


          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	  	  	  	  	  	  	  	  //a 			b 			c 			d 			e			 f 			g 				h
   GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|  GPIO_Pin_1 |GPIO_Pin_2| GPIO_Pin_3 |GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
       GPIO_InitStructure.GPIO_OType =  GPIO_OType_PP;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
       GPIO_InitStructure.GPIO_PuPd =   GPIO_PuPd_NOPULL ;
       GPIO_Init(GPIOE, &GPIO_InitStructure);


}

uint16_t numbers [10] = {
	         	        		  /* -0- */  GPIO_Pin_0|  GPIO_Pin_1 |GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5,
								  /* -1- */	GPIO_Pin_1 |GPIO_Pin_2,
								  /* -2- */	GPIO_Pin_0|  GPIO_Pin_1 |GPIO_Pin_6 | GPIO_Pin_3| GPIO_Pin_4 ,
								  /* -3- */	GPIO_Pin_0|  GPIO_Pin_1 |GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_6  ,
								  /* -4- */	GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_2,
								  /* -5- */	 GPIO_Pin_0| GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_2| GPIO_Pin_3 ,
								  /* -6- */	 GPIO_Pin_0| GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4,
								  /* -7- */	GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2,
								  /* -8- */GPIO_Pin_0|  GPIO_Pin_1 |GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6,
								  /* -9- */	GPIO_Pin_0|GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_3
	         	          };


volatile int segments[]={GPIO_Pin_12 ,GPIO_Pin_13, GPIO_Pin_14,GPIO_Pin_15};





void DAC_Config(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); // zegar dla portu GPIO z którego wykorzystany zostanie pin jako wyjœcie DAC (PA4)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE); // zegar dla modu³u DAC

	GPIO_InitTypeDef GPIO_InitStructure;
	//inicjalizacja wyjœcia DAC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_InitTypeDef DAC_InitStructure;
	//wy³¹czenie zewnêtrznego wyzwalania
	//konwersja mo¿e byæ wyzwalana timerem, stanem wejœcia itd. (szczegó³y w dokumentacji)
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	//nast. 2 linie - wy³¹czamy generator predefiniowanych przebiegów //wyjœciowych (wartoœci zadajemy sami, za pomoc¹ odpowiedniej funkcji)
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	//w³¹czamy buforowanie sygna³u wyjœciowego
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);


	DAC_Cmd(DAC_Channel_1, ENABLE);

	DAC_SetChannel1Data(DAC_Align_12b_R, 0xFFF);

}
void ADC1_Config(){
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); // zegar dla portu GPIO z którego wykorzystany zostanie pin jako wejœcie ADC (PA1)
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // zegar dla modu³u ADC1
GPIO_InitTypeDef GPIO_InitStructure;
//inicjalizacja wejœcia ADC
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOA, &GPIO_InitStructure);
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;



//Wspólna konfiguracja dla wszystkich uk³adów ADC:
ADC_CommonInitTypeDef ADC_CommonInitStructure;
// niezale¿ny tryb pracy przetworników
ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
// zegar g³ówny podzielony przez 2
ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
// opcja istotna tylko dla trybu multi ADC
ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
// czas przerwy pomiêdzy kolejnymi konwersjami
ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
ADC_CommonInit(&ADC_CommonInitStructure);



ADC_InitTypeDef ADC_InitStructure;
//ustawienie rozdzielczoœci przetwornika na maksymaln¹ (12 bitów)
ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//wy³¹czenie trybu skanowania (odczytywaæ bêdziemy jedno wejœcie ADC
//w trybie skanowania automatycznie wykonywana jest konwersja na wielu //wejœciach/kana³ach)
ADC_InitStructure.ADC_ScanConvMode = DISABLE;
//w³¹czenie ci¹g³ego trybu pracy
ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//wy³¹czenie zewnêtrznego wyzwalania
//konwersja mo¿e byæ wyzwalana timerem, stanem wejœcia itd. (szczegó³y w //dokumentacji)
ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//wartoœæ binarna wyniku bêdzie podawana z wyrównaniem do prawej
//funkcja do odczytu stanu przetwornika ADC zwraca wartoœæ 16-bitow¹
//dla przyk³adu, wartoœæ 0xFF wyrównana w prawo to 0x00FF, w lewo 0x0FF0
ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//liczba konwersji równa 1, bo 1 kana³
ADC_InitStructure.ADC_NbrOfConversion = 1;
// zapisz wype³nion¹ strukturê do rejestrów przetwornika numer 1
ADC_Init(ADC1, &ADC_InitStructure);

ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_84Cycles);
ADC_Cmd(ADC1, ENABLE);



}
void InitializeTimer_OnehundredthSecond(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


	TIM_TimeBaseStructure.TIM_Period =99;//59999;
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


}

void TIM3_NVIC_Config(){
	NVIC_InitTypeDef NVIC_InitStructure;
			// numer przerwania
			NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
			// priorytet g³ówny
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
			// subpriorytet
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
			// uruchom dany kana³
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			// zapisz wype³nion¹ strukturê do rejestrów
			NVIC_Init(&NVIC_InitStructure);

			// wyczyszczenie przerwania od timera 3 (wyst¹pi³o przy konfiguracji timera)
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
			// zezwolenie na przerwania od przepe³nienia dla 3
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}



void TIM3_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{


		ADC_SoftwareStartConv(ADC1);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
			ADC_Result = ADC_GetConversionValue(ADC1);
			Uin=(ADC_Result*2.95)/4095;

			liczba=(int)((2.95-Uin)*10/1.45);
			liczba2=(int)( ( ((2.95-Uin)*10/1.45)-liczba )*10);


			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);




	}
}









void InitializeTimer_TIM5(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period =59999;
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM5, ENABLE);
}

volatile int zmiennaTenseconds=0;
void TIM5_NVIC_Config(){
	NVIC_InitTypeDef NVIC_InitStructure;
			// numer przerwania
			NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
			// priorytet g³ówny
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
			// subpriorytet
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
			// uruchom dany kana³
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			// zapisz wype³nion¹ strukturê do rejestrów
			NVIC_Init(&NVIC_InitStructure);

			// wyczyszczenie przerwania od timera 3 (wyst¹pi³o przy konfiguracji timera)
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
			// zezwolenie na przerwania od przepe³nienia dla timera 3
			TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
}

void TIM5_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		// miejsce na kod wywo³ywany w momencie wyst¹pienia przerwania

		TIM_Cmd(TIM2, ENABLE);
			status_pompy=0;
			}

		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}









void InitializeTimer2()
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    	TIM_TimeBaseStructure.TIM_Period =9999;
    	TIM_TimeBaseStructure.TIM_Prescaler = 8399;
    	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
    	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
}

void TIM2_NVIC_Config(){
	NVIC_InitTypeDef NVIC_InitStructure;
			// numer przerwania
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
			// priorytet g³ówny
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
			// subpriorytet
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
			// uruchom dany kana³
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			// zapisz wype³nion¹ strukturê do rejestrów
			NVIC_Init(&NVIC_InitStructure);

			// wyczyszczenie przerwania od timera 3 (wyst¹pi³o przy konfiguracji timera)
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			// zezwolenie na przerwania od przepe³nienia dla 3
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}



void TIM2_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		DAC_SetChannel1Data(DAC_Align_12b_R,4095);

		if(Uin>1.9 && status_pompy==0){//1.4 dla wody cale zanuzenie
			GPIO_ResetBits(GPIOD,GPIO_Pin_11);
			GPIO_ResetBits(GPIOD,GPIO_Pin_12);
			GPIO_SetBits(GPIOD,GPIO_Pin_14);

		DAC_SetChannel1Data(DAC_Align_12b_R,4095);
		}
		else if(Uin<=1.9  || status_pompy>0){
			GPIO_SetBits(GPIOD,GPIO_Pin_11);
			GPIO_SetBits(GPIOD,GPIO_Pin_12);
			GPIO_ResetBits(GPIOD,GPIO_Pin_14);
			DAC_SetChannel1Data(DAC_Align_12b_R,0);
		}








			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

			if(status_pompy>0){
			TIM_Cmd(TIM2, DISABLE);
			TIM_SetCounter(TIM2, 0);
			}
			++status_pompy;
	}






}
void InitializeLEDs()
{
    /* GPIOD Periph clock enable */
   	    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

   	    GPIO_InitTypeDef  GPIO_InitStructure;
   	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
   	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   	    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void InitializeButtons(){
	 GPIO_InitTypeDef  GPIO_Init_Button_Structure;
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	 GPIO_Init_Button_Structure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3;
	 GPIO_Init_Button_Structure.GPIO_Mode = GPIO_Mode_IN;
	 GPIO_Init_Button_Structure.GPIO_Speed = GPIO_Speed_100MHz;
	 GPIO_Init_Button_Structure.GPIO_PuPd = GPIO_PuPd_UP;//odwrotnien niz w drugich labach
	        GPIO_Init(GPIOB, & GPIO_Init_Button_Structure);
}






void InitializeTimer_TIM4(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period =24;
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM4, ENABLE);
}


void TIM4_NVIC_Config(){
	NVIC_InitTypeDef NVIC_InitStructure;
			// numer przerwania
			NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
			// priorytet g³ówny
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
			// subpriorytet
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
			// uruchom dany kana³
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			// zapisz wype³nion¹ strukturê do rejestrów
			NVIC_Init(&NVIC_InitStructure);

			// wyczyszczenie przerwania od timera 3 (wyst¹pi³o przy konfiguracji timera)
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			// zezwolenie na przerwania od przepe³nienia dla timera 3
			TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}





void TIM4_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{

		// miejsce na kod wywo³ywany w momencie wyst¹pienia przerwania
		GPIO_ResetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
		for(j%=3;;){
			GPIO_SetBits(GPIOB,segments[j]);
				GPIO_SetBits(GPIOE,GPIO_Pin_0|  GPIO_Pin_1 |GPIO_Pin_2| GPIO_Pin_3 |GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6|GPIO_Pin_7);
				if(j==0){
				if(liczba>9)
					GPIO_ResetBits(GPIOE,GPIO_Pin_1 |GPIO_Pin_2| GPIO_Pin_7);
					else
					GPIO_ResetBits(GPIOE, GPIO_Pin_0|  GPIO_Pin_1 |GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_7);
				}
				else if(j==1 ){
					if(liczba>9)
						GPIO_ResetBits(GPIOE,numbers[0]);
					else
						GPIO_ResetBits(GPIOE,numbers[liczba]);
				}
				else if(j==2 ){
					if(liczba>9)
						GPIO_ResetBits(GPIOE,numbers[0]);
					else
						GPIO_ResetBits(GPIOE,numbers[liczba2]);
				}
				++j;
			break;
		}

		// wyzerowanie flagi wyzwolonego przerwania
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}












int main(){
	wyswietlacz_Led();

	InitializeButtons();
	 InitializeLEDs();
		InitializeTimer_OnehundredthSecond();
		TIM_Cmd(TIM3, ENABLE);
		TIM3_NVIC_Config();
		 	 ADC1_Config();


		DAC_Config();
		 	InitializeTimer2();
		 			TIM2_NVIC_Config();

		 			 InitializeTimer_TIM4();
		 			TIM4_NVIC_Config();
		 			InitializeTimer_TIM5();
		 			TIM5_NVIC_Config();
		 			GPIO_SetBits(GPIOD,GPIO_Pin_11);
		 					GPIO_SetBits(GPIOD,GPIO_Pin_12);
		 	while(1){

		 	}

}


