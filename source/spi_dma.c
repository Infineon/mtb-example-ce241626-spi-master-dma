/******************************************************************************
* File Name:   spi_dma.c
*
* Description: This file contains function definitions for DMA operation.
*
* Related Document: See README.md
*
*
*******************************************************************************
********************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/

/*******************************************************************************
* Header Files
*******************************************************************************/
#include "spi_dma.h"
#include "interface.h"

/*******************************************************************************
 *                       Macro definitions
 ******************************************************************************/

/* Interrupt priority for RXDMA */
#define RXDMA_INTERRUPT_PRIORITY (7u)

/* Interrupt priority for TXDMA */
#define TXDMA_INTERRUPT_PRIORITY (7u)


/* variable to check the rx dma transaction status */
#if ((SPI_MODE == SPI_MODE_BOTH) || (SPI_MODE == SPI_MODE_SLAVE))

volatile bool rx_dma_done = false;

#endif

/* variable to check the tx dma transaction status */
#if ((SPI_MODE == SPI_MODE_BOTH) || (SPI_MODE == SPI_MODE_MASTER))

volatile bool tx_dma_done = false;

/******************************************************************************
* Function Name: configure_tx_dma
*******************************************************************************
*
* Summary:      This function configure the transmit DMA block 
*
* Parameters:   tx_buffer
*
* Return:       (uint32_t) INIT_SUCCESS or INIT_FAILURE
*
******************************************************************************/
uint32_t configure_tx_dma(uint32_t* tx_buffer)
{
     cy_en_dma_status_t dma_init_status;
     const cy_stc_sysint_t intTxDma_cfg =
     {
         .intrSrc      = txDma_IRQ,
         .intrPriority = 7u
     };
     /* Initialize descriptor */
     dma_init_status = Cy_DMA_Descriptor_Init(&txDma_Descriptor_0, &txDma_Descriptor_0_config);
     if (dma_init_status!=CY_DMA_SUCCESS)
     {
         return INIT_FAILURE;
     }

     dma_init_status = Cy_DMA_Channel_Init(txDma_HW, txDma_CHANNEL, &txDma_channelConfig);
     if (dma_init_status!=CY_DMA_SUCCESS)
     {
         return INIT_FAILURE;
     }

     /* Set source and destination for descriptor 1 */
     Cy_DMA_Descriptor_SetSrcAddress(&txDma_Descriptor_0, (uint8_t *)tx_buffer);
     Cy_DMA_Descriptor_SetDstAddress(&txDma_Descriptor_0, (void *)&mSPI_HW->TX_FIFO_WR);

     /* Initialize and enable the interrupt from TxDma */
     Cy_SysInt_Init(&intTxDma_cfg,&tx_dma_complete);
     NVIC_EnableIRQ((IRQn_Type)intTxDma_cfg.intrSrc);

      /* Enable DMA interrupt source. */
     Cy_DMA_Channel_SetInterruptMask(txDma_HW, txDma_CHANNEL, CY_DMA_INTR_MASK);
     /* Enable DMA block to start descriptor execution process */
     Cy_DMA_Enable(txDma_HW);
     return INIT_SUCCESS;
}

/******************************************************************************
* Function Name: tx_dma_complete
*******************************************************************************
*
* Summary:      This function check the tx DMA status
*
* Parameters:   None
*
* Return:       None
*
******************************************************************************/
void tx_dma_complete(void)
{
     /* Check tx DMA status */
     if ((CY_DMA_INTR_CAUSE_COMPLETION    != Cy_DMA_Channel_GetStatus(txDma_HW, txDma_CHANNEL)) &&
         (CY_DMA_INTR_CAUSE_CURR_PTR_NULL != Cy_DMA_Channel_GetStatus(txDma_HW, txDma_CHANNEL)))
     {
         /* DMA error occurred while TX operations */
         handle_error();
     }

     tx_dma_done = true;
     /* Clear tx DMA interrupt */
     Cy_DMA_Channel_ClearInterrupt(txDma_HW, txDma_CHANNEL);
}

#endif /* #if ((SPI_MODE == SPI_MODE_BOTH) || (SPI_MODE == SPI_MODE_MASTER)) */


#if ((SPI_MODE == SPI_MODE_BOTH) || (SPI_MODE == SPI_MODE_SLAVE))

/******************************************************************************
* Function Name: configure_rx_dma
*******************************************************************************
*
* Summary:      This function configure the receive DMA block 
*
* Parameters:   rx_buffer
*
* Return:       (uint32_t) INIT_SUCCESS or INIT_FAILURE
*
******************************************************************************/
uint32_t configure_rx_dma(uint32_t* rx_buffer)
{
     cy_en_dma_status_t dma_init_status;
     const cy_stc_sysint_t intRxDma_cfg =
     {
         .intrSrc      = rxDma_IRQ,
         .intrPriority = 7u
     };
     /* Initialize descriptor */
     dma_init_status = Cy_DMA_Descriptor_Init(&rxDma_Descriptor_0, &rxDma_Descriptor_0_config);
     if (dma_init_status!=CY_DMA_SUCCESS)
     {
         return INIT_FAILURE;
     }

     dma_init_status = Cy_DMA_Channel_Init(rxDma_HW, rxDma_CHANNEL, &rxDma_channelConfig);
     if (dma_init_status!=CY_DMA_SUCCESS)
     {
         return INIT_FAILURE;
     }

     /* Set source and destination for descriptor 1 */
     Cy_DMA_Descriptor_SetSrcAddress(&rxDma_Descriptor_0, (void *)&sSPI_HW->RX_FIFO_RD);
     Cy_DMA_Descriptor_SetDstAddress(&rxDma_Descriptor_0, (uint8_t *)rx_buffer);

      /* Initialize and enable the interrupt from RxDma */
     Cy_SysInt_Init(&intRxDma_cfg,&rx_dma_complete);
     NVIC_EnableIRQ((IRQn_Type)intRxDma_cfg.intrSrc);

      /* Enable DMA interrupt source. */
     Cy_DMA_Channel_SetInterruptMask(rxDma_HW, rxDma_CHANNEL, CY_DMA_INTR_MASK);
     /* Enable channel and DMA block to start descriptor execution process */
     Cy_DMA_Channel_Enable(rxDma_HW, rxDma_CHANNEL);
     Cy_DMA_Enable(rxDma_HW);
     return INIT_SUCCESS;
}

/******************************************************************************
* Function Name: rx_dma_complete
*******************************************************************************
*
* Summary:      This function check the rx DMA status
*
* Parameters:   None
*
* Return:       None
*
******************************************************************************/
void rx_dma_complete(void)
{
    /* Scenario: Inside the interrupt service routine for block DW0 channel 23: */
    if (CY_DMA_INTR_MASK == Cy_DMA_Channel_GetInterruptStatusMasked(rxDma_HW, rxDma_CHANNEL))
    {
        /* Get the interrupt cause */
        cy_en_dma_intr_cause_t cause = Cy_DMA_Channel_GetStatus(rxDma_HW, rxDma_CHANNEL);
        if (CY_DMA_INTR_CAUSE_COMPLETION != cause)
        {
            /* DMA error occurred while RX operations */
            handle_error();
        }
        else
        {
            rx_dma_done = true;
        }

        /* Clear the interrupt */
        Cy_DMA_Channel_ClearInterrupt(rxDma_HW, rxDma_CHANNEL);
    }

}

#endif /* #if ((SPI_MODE == SPI_MODE_BOTH) || (SPI_MODE == SPI_MODE_SLAVE)) */
