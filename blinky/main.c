#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>

void delay(uint32_t count)
{
	while(count--);
}

void init_GPIO()
{
	GPIO_InitTypeDef GPIO_InitStruct = {
		.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1,
		.GPIO_Mode = GPIO_Mode_OUT,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_OType =GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_DOWN
	};

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

int main()
{
	init_GPIO();

	int digitStatus = 1;

	while(1) {
		GPIO_WriteBit(GPIOB, GPIO_Pin_0, digitStatus);
		delay(1000000L);
		GPIO_WriteBit(GPIOB, GPIO_Pin_1, digitStatus);	
		delay(1000000L);

		digitStatus = (digitStatus + 1) % 2;
	}

	return 0;
}
