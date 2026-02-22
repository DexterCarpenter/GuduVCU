/*
#include "bms.h"


static const uint8_t CRCKEY = 0x185;

// should this be better understood as handle bms can msg?
void decode_bms_can(struct can_frame *frame){
    
}

// crc check
uint8_t bms_can_crc(int byte_count, uint8_t *msg)
{
   uint16_t tmp = 0;
   uint8_t crc = 0;
   uint8_t i,j;

   for(i = 0; i < byte_count; i++)
   {
      tmp ^= *(msg + i);
      for(j = 0; j < 8; j++ )
      {
         tmp <<= 1;
         if( tmp & 0x0100 )
         {
            tmp ^= CRCKEY;
         }
      }
   }
   crc = (uint8_t)(tmp & 0xff);
   return crc;
}
*/