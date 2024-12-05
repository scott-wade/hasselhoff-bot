/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PACKET_H_
#define __PACKET_H_

#include "state_machine_sub.h"
#ifdef __cplusplus
 extern "C" {
#endif

// Define packet.type 
typedef enum {
    RESET_MSG = 1,
    DRIVE_FB_MSG,
    DRIVE_LR_MSG,
    DRIVE_DS_MSG,
    LAND_MSG,
    STATUS_REQ_MSG

} packet_type_t;


// helper methods for SUBMARINE_CURRENT_STATUS_MSG
uint8_t subStateToSubStatusMsg(sub_t);


#ifdef __cplusplus
}
#endif

#endif /*__PACKET_H */

