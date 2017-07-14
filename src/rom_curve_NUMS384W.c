#include "arch.h"
#include "ecp_NUMS384W.h"

/*  NUMS 256-bit Curve - Weierstrass */

#if CHUNK==16

#error Not supported

#endif

#if CHUNK==32
const int CURVE_A_NUMS384W= -3;
const int CURVE_B_I_NUMS384W= -34568;
const BIG_384_29 CURVE_B_NUMS384W= {0x1FFF77BB,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x7F};
const BIG_384_29 CURVE_Order_NUMS384W= {0x1B0E61B9,0x26C0FB3,0xDF89E98,0x153A7A98,0x16881BED,0x178F75AE,0x1FFF587A,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x1FFFFFFF,0x7F};
const BIG_384_29 CURVE_Gx_NUMS384W= {0x98152A,0x1CE5D021,0x18711EFA,0x1DDA201E,0xC742522,0x148D9536,0x7D3CEF4,0x19BF703F,0x60225C1,0x12082F8D,0x12203288,0x2DE3038,0x17956F0B,0x3A};
const BIG_384_29 CURVE_Gy_NUMS384W= {0x6180716,0x3A5C763,0x1D2B4997,0xD69B77F,0x837EBCD,0x1BE890D,0xE72E482,0xEFF0FEE,0x1EB00469,0x2C267B,0x15F8CF4C,0x3371C71,0xDEE368E,0x56};
#endif

#if CHUNK==64
const int CURVE_A_NUMS384W= -3;
const int CURVE_B_I_NUMS384W= -34568;
const BIG_384_56 CURVE_B_NUMS384W= {0xFFFFFFFFFF77BBL,0xFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFL};
const BIG_384_56 CURVE_Order_NUMS384W= {0x4D81F67B0E61B9L,0x9D3D4C37E27A60L,0x1EEB5D6881BEDAL,0xFFFFFFFFD61EAFL,0xFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFFFL,0xFFFFFFFFFFFFL};
const BIG_384_56 CURVE_Gx_NUMS384W= {0x9CBA042098152AL,0xED100F61C47BEBL,0x1B2A6CC742522EL,0xFB81F9F4F3BD29L,0x5F1A60225C1CDL,0x181C4880CA2241L,0x757956F0B16FL};
const BIG_384_56 CURVE_Gy_NUMS384W= {0x74B8EC66180716L,0xB4DBBFF4AD265CL,0x7D121A837EBCD6L,0xF87F739CB92083L,0x84CF7EB0046977L,0x8E38D7E33D3005L,0xACDEE368E19BL};
#endif