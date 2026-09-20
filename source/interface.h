/******************************************************************************
* File Name:   interface.h
*
* Description: This file contains all the macros used by the source files in this
*               application.
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
#ifndef SOURCE_INTERFACE_H_
#define SOURCE_INTERFACE_H_

#include "cy_pdl.h"
#include "cycfg.h"

/* This code example requires two SPI ports, one as master and the other one 
 * as slave. Set the SPI_MODE macro depending on the kit. Some kits support two 
 * SPI ports which you can configure in SPI_MODE_BOTH whereas some support only
 * one port in which case you can configure the kit in either SPI_MODE_MASTER or 
 * SPI_MODE_SLAVE. See README.md to know more on the kit specific configuration.
 */
#define SPI_MODE_BOTH    0
#define SPI_MODE_MASTER  1
#define SPI_MODE_SLAVE   2

#define SPI_MODE SPI_MODE_BOTH


#if (SPI_MODE == SPI_MODE_BOTH)
    #if defined (TARGET_CYW9P62S1_43012EVB_01) || defined (TARGET_CY8CPROTO_062S3_4343W) || \
    defined (TARGET_CY8CKIT_062S4)
        #warning For the current TARGET, this example requires the SPI_MODE macro to be \
                 configured either in SPI_MODE_MASTER or SPI_MODE_SLAVE, not in SPI_MODE_BOTH.
    #endif
#endif

/* Initialization status */
#define INIT_SUCCESS            (0)
#define INIT_FAILURE            (1)

/* Communication status */
#define TRANSFER_COMPLETE       (0)
#define TRANSFER_FAILURE        (1)
#define TRANSFER_IN_PROGRESS    (2)
#define IDLE                    (3)

/* TX Packet Head and Tail */
#define PACKET_SOP              (0x01UL)
#define PACKET_EOP              (0x17UL)

/* Element index in the packet */
#define PACKET_SOP_POS          (0UL)
#define PACKET_CMD_POS          (1UL)
#define PACKET_EOP_POS          (2UL)


#endif /* SOURCE_INTERFACE_H_ */
