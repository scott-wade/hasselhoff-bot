/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PACKET_H_
#define __PACKET_H_

#include "state_machine_sub.h"
#ifdef __cplusplus
 extern "C" {
#endif

// Define packet.type 
typedef enum {
    RESET_MSG = 0b1,
    DRIVE_FB_MSG = 0b10,
    DRIVE_LR_MSG = 0b11,
    DRIVE_DS_MSG = 0b100,
    LAND_MSG = 0b101,
    STATUS_REQ_MSG = 0b110

} packet_type_t;


// helper methods for SUBMARINE_CURRENT_STATUS_MSG
uint8_t subStateToSubStatusMsg(sub_t);


#ifdef __cplusplus
}
#endif

#endif /*__PACKET_H */

