#ifndef HSCHED_H_
#define HSCHED_H_

#include "../include/STD_TYPES.h"

typedef void (*schedcbf_t) (void);

typedef enum
{
    HSCHED_ENU_OK = 0,
    HSCHED_ENU_NOK,
    HSCHED_ENU_NULL_PTR,
    HSCHED_ENU_NULL_PTR_CALLBACK,
    HSCHED_ENU_INVALID_PRIORITY
} HSCHED_enuErrorStatus;

typedef struct 
{
    uint32_t Priority;
    uint32_t CyclicTime;
    schedcbf_t CallBack;
    uint32_t FirstDelay;
} HSCHED_RunnableCfg_t;

HSCHED_enuErrorStatus HSCHED_enuInit(void);
HSCHED_enuErrorStatus HSCHED_enuRegisterRunnable(HSCHED_RunnableCfg_t* Add_HSCHED_cfg);
HSCHED_enuErrorStatus HSCHED_enuStart();
HSCHED_enuErrorStatus HSCHED_enuStop();


#endif // HSCHED_H