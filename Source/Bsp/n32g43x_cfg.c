/**
 * @file n32g43x_cfg.c
 * @author N32cube
 */

 #include "n32g43x_cfg.h"
/* NTFx CODE START */
__IO uint32_t mwTick;
void SysTick_Delayms(uint32_t Delayms)
{
    uint32_t tickstart = mwTick;
    uint32_t wait=Delayms;
    /* Add 1 to guarantee minimum wait */
    if (wait < 0xFFFFFFFFU)
    {
        wait +=1;
    }
    while ((mwTick - tickstart) < wait)
    {
    }
}
 /**
 *@name  DMA_SetPerMemAddr.
 *@brief Set peripher address and memory address of DMA
 *@param DMAChx (The input parameters must be the following values):
 *          - DMA_CH1
 *          - DMA_CH2
 *          - DMA_CH3
 *          - DMA_CH4
 *          - DMA_CH5
 *          - DMA_CH6
 *          - DMA_CH7
 *          - DMA_CH8
 *@param periphAddr   peripher address
 *@param memAddr   memory address
 *@param bufSize   buff size
 *@return status
 */
 void DMA_SetPerMemAddr(DMA_ChannelType* DMAChx, uint32_t periphAddr,uint32_t memAddr,uint32_t bufSize )
 {
     /* DMAy Channelx TXNUM Configuration */
    /* Write to DMAy Channelx TXNUM */
    DMAChx->TXNUM = bufSize;

    /* DMAy Channelx PADDR Configuration */
    /* Write to DMAy Channelx PADDR */
    DMAChx->PADDR = periphAddr;

    /* DMAy Channelx MADDR Configuration */
    /* Write to DMAy Channelx MADDR */
    DMAChx->MADDR = memAddr;
 }
/* NTFx CODE END */
/* NTFx CODE START */
/**
 *@brief Initializes the clock tree
 *@param null
 *@return status
 */
bool RCC_Configuration(void)
{
    ErrorStatus ClockStatus;
    RCC_DeInit();
    RCC_ConfigHclk(RCC_SYSCLK_DIV1);
    RCC_ConfigPclk2(RCC_HCLK_DIV4);
    RCC_ConfigPclk1(RCC_HCLK_DIV4);
     
    RCC_EnableHsi(ENABLE);
    /* Wait till HSI is ready */
    ClockStatus = RCC_WaitHsiStable();
    if (ClockStatus != SUCCESS) return false;
     
    RCC_ConfigHse(RCC_HSE_ENABLE);
    /* Wait till HSE is ready */
    ClockStatus = RCC_WaitHseStable();
    if (ClockStatus != SUCCESS) return false;
     
    RCC_ConfigPll(RCC_PLL_SRC_HSE_DIV1,RCC_PLL_MUL_27,RCC_PLLDIVCLK_ENABLE);
    /* Enable PLL */
    RCC_EnablePll(ENABLE);
    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_CTRL_FLAG_PLLRDF) != SET);
     
    /* Disable Prefetch Buffer */
    FLASH_PrefetchBufSet(FLASH_PrefetchBuf_DIS);
    /* Enable iCache */
    FLASH_iCacheCmd(FLASH_iCache_EN);
    /* Flash wait state */
    FLASH_SetLatency(FLASH_LATENCY_3);
     
    /*config RNG clock*/
    RCC_ConfigTrng1mClk(RCC_TRNG1MCLK_SRC_HSI, RCC_TRNG1MCLK_DIV16);
    RCC_EnableTrng1mClk(ENABLE);
    RCC_ConfigRngcClk(RCC_RNGCCLK_SYSCLK_DIV1);
     
    /* Select PLLCLK as system clock source */
    RCC_ConfigSysclk(RCC_SYSCLK_SRC_PLLCLK);
    /* Wait till PLLCLK is used as system clock source */
    while (RCC_GetSysclkSrc() != 0x0c) ;
    /*  Configure the SysTick to have interrupt in 1ms time basis*/
    SysTick_Config(108000);
/* NTFx CODE END */
    return true;
}
/* NTFx CODE START */
/**
 *@brief Initializes the NVIC
 *@param null
 *@return status
 */
bool NVIC_Configuration(void)
{
    /*Configure the preemption priority and subpriority:
    - 4 bits for pre-emption priority: possible value are 0..15
    - 0 bits for subpriority: possible value are 0
    - Lower values gives higher priority
    */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
/* NTFx CODE END */
    return true;
}
/* NTFx CODE START */
/**
 *@brief Initializes the DMA
 *@param null
 *@return status
 */
bool DMA_Configuration(void)
{
/* NTFx CODE END */
    return true;
}
/* NTFx CODE START */
/**
 *@brief Initializes the GPIO
 *@param null
 *@return status
 */
bool GPIO_Configuration(void)
{
     
    /* Enable the GPIO clock*/
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_AFIO, ENABLE);
    
     
/* NTFx CODE END */
    return true;
}
