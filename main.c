#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define STM32_RCC_BASE 0x40023800
#define STM32_GPIOC_BASE     0x40020800
#define STM32_GPIOA_BASE 0X40020000

#define STM32_RCC_AHB1ENR_OFFSET    0x0030
#define STM32_GPIO_MODER_OFFSET   0x0000
#define STM32_GPIO_OTYPER_OFFSET  0x0004
#define STM32_GPIO_PUPDR_OFFSET   0x000c
#define STM32_GPIO_IDR_OFFSET     0x0010
#define STM32_GPIO_ODR_OFFSET    0X0014
#define STM32_GPIO_BSRR_OFFSET    0x0018
#define STM32_RCC_AHB1ENR (STM32_RCC_BASE+STM32_RCC_AHB1ENR_OFFSET)
#define STM32_GPIOA_MODER (STM32_GPIOA_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOA_PUPDR (STM32_GPIOA_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIO_IDR (STM32_GPIOA_BASE+STM32_GPIO_IDR_OFFSET)
#define STM32_GPIOC_MODER (STM32_GPIOC_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOC_OTYPER (STM32_GPIOC_BASE+STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOC_PUPDR (STM32_GPIOC_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_ODR (STM32_GPIOC_BAASE+STM32_GPIO_ODR_OFFSET)
#define STM32_GPIOC_BSRR (STM32_GPIOC_BASE + STM32_GPIO_BSRR_OFFSET)

#define RCC_AHB1ENR_GPIOCEN (1 << 2)
#define RCC_AHB1ENR_GPIOAEN (1 << 0)
#define GPIO_MODER_INPUT (0)
#define GPIO_MODER_OUTPUT (1)
#define GPIO_MODER_ALT (2)
#define GPIO_MODER_ANALOG (3)
#define GPIO_MODER_SHIFT(n) ((n) << 1)
#define GPIO_MODER_MASK(n) (3 << GPIO_MODER_SHIFT(n))
#define GPIO_PUPDR_SHIFT(n) ((n) << 1) 
#define GPIO_PUPDR_MASK(n) (3 << GPIO_PUPDR_SHIFT(n))

#define GPIO_IDR(n) (1 << (n))

#define GPIO_OTYPER_PP (0)
#define GPIO_OTYPER_OD (1)

#define GPIO_OT13_SHIFT (13)
#define GPIO_OT13_MASK (1 << GPIO_OT13_SHIFT)

#define GPIO_PUPDR_NONE (0)
#define GPIO_PUPDR_PULLUP (1)
#define GPIO_PUPDR_PULLDOWN (2)

#define GPIO_BSRR_SET(n) (1 << (n))
#define GPIO_BSRR_RST(n) (1 << (n + 16))


static uint32_t led_status;
static const char fw_version[] = {'V','1','.','0'};
 
int main(int argc, char *argv[])
{

/* Vetor para piscagem */

static const uint32_t LED_PISCA [] = {200, 400, 600, 800};
uint8_t contador = 0; 

uint32_t reg;
uint32_t *pRCC_AHB1ENR  = (uint32_t *)STM32_RCC_AHB1ENR;
uint32_t *pGPIOC_MODER  = (uint32_t*)STM32_GPIOC_MODER;
uint32_t *pGPIOC_OTYPER = (uint32_t*)STM32_GPIOC_OTYPER;
uint32_t *pGPIOC_PUPDR  = (uint32_t*)STM32_GPIOC_PUPDR;
uint32_t *pGPIOC_BSRR   = (uint32_t*)STM32_GPIOC_BSRR;
uint32_t *pGPIOA_MODER  = (uint32_t*)STM32_GPIOA_MODER;
uint32_t *pGPIOA_PUPDR  = (uint32_t*)STM32_GPIOA_PUPDR;
uint32_t *pGPIOA_IDR  = (uint32_t*)STM32_GPIO_IDR;

reg  = *pRCC_AHB1ENR;
reg |= (RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN);
*pRCC_AHB1ENR = reg;

reg = *pGPIOA_MODER;
reg &= ~(GPIO_MODER_MASK(0));
reg |= (GPIO_MODER_OUTPUT << GPIO_MODER_SHIFT(0));
*pGPIOA_MODER = reg;

reg = *pGPIOA_PUPDR;
reg &= ~(GPIO_PUPDR_MASK(0));
reg |= (GPIO_PUPDR_NONE << GPIO_PUPDR_SHIFT(0));
*pGPIOA_PUPDR = reg;

reg = *pGPIOC_MODER;
reg &= ~(GPIO_MODER_MASK(13));
reg |= (GPIO_MODER_OUTPUT << GPIO_MODER_SHIFT(13));
*pGPIOC_MODER = reg;

reg = *pGPIOC_OTYPER;
reg &= ~(GPIO_OT13_MASK);
reg |= (GPIO_OTYPER_PP << GPIO_OT13_SHIFT);
*pGPIOC_OTYPER = reg;

reg = *pGPIOC_PUPDR;
reg &= ~(GPIO_PUPDR_MASK(13));
reg |= (GPIO_PUPDR_NONE << GPIO_PUPDR_SHIFT(13));
*pGPIOC_PUPDR = reg;



 while(1)
 {
    reg = *pGPIOA_IDR;
    if ( (reg & GPIO_IDR(0)) == 0) //botao pressionado
    {
        contador++;
        if (contador > 3)
        {
            contador = 0;
        }  
    }

     *pGPIOC_BSRR = GPIO_BSRR_SET(13);
     for(uint32_t i =0; i < LED_PISCA[contador]; i++);
     *pGPIOC_BSRR = GPIO_BSRR_RST(13);
     for(uint32_t i =0; i < LED_PISCA[contador]; i++);

 }

 return EXIT_SUCCESS;
}

















