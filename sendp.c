#include <stdio.h>
#include <string.h>
#include <exanic/exanic.h>
#include <exanic/fifo_tx.h>

int main(void)
{
    char *device = "exanic0";
    int port = 0;

    exanic_t *exanic = exanic_acquire_handle(device);
    if (!exanic)
    {
        fprintf(stderr, "exanic_acquire_handle: %s\n", exanic_get_last_error());
        return 1;
    }

    exanic_tx_t *tx = exanic_acquire_tx_buffer(exanic, port, 0);
    if (!tx)
    {
        fprintf(stderr, "exanic_acquire_tx_buffer: %s\n", exanic_get_last_error());
        return 1;
    }

    //char frame[1000];
    //char frame[] = "0x64 0x3F 0x5F 0x01 0x2F 0x42 0x64 0x3F 0x5F 0x01 0x2F 0x42 0x88 0x47 0x00 0x01 0x20 0xFE 0x00 0x01 0x01 0xFF 0x00 0x00 0x00 0x00 0xCC 0x07 0x0D 0x08 0x00 0x00 0xCC 0x00 0x0A 0x64 0x00 0x00 0x81 0x00 0x00 0x01 0x08 0x00 0x45 0x00 0x00 0x64 0x00 0x09 0x00 0x00 0xFF 0x01 0x38 0x3C 0xC0 0xA8 0x01 0x02 0xC0 0xA1 0x01 0x01 0x00 0x00 0xB9 0xC8 0x00 0x01 0x00 0x04 0x00 0x00 0x00 0x00 0x00 0x04 0xCC 0x78 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD 0xAB 0xCD";
   //char frame[] = {0x64, 0x3F, 0x5F, 0x01, 0x2F, 0x42, 0x64, 0x3F, 0x5F, 0x01, 0x2F, 0x42, 0x88, 0x47, 0x00, 0x01, 0x20, 0xFE, 0x00, 0x01, 0x01, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x07, 0x0D, 0x08, 0x00, 0x00, 0xCC, 0x00, 0x0A, 0x64, 0x00, 0x00, 0x81, 0x00, 0x00, 0x01, 0x08, 0x00, 0x45, 0x00, 0x00, 0x64, 0x00, 0x09, 0x00, 0x00, 0xFF, 0x01, 0x38, 0x3C, 0xC0, 0xA8, 0x01, 0x02, 0xC0, 0xA1, 0x01, 0x01, 0x00, 0x00, 0xB9, 0xC8, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xCC, 0x78, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD, 0xAB, 0xCD}; 
   char frame[] = {0x64, 0x3f, 0x5f, 0x01, 0x2f, 0x43, 0x64, 0x3f, 0x5f, 0x01, 0x2f, 0x42, 0x08, 0x00, 0x45, 0x00, 0x00, 0x52, 0x45, 0x9F, 0x00, 0x00, 0x01, 0x11, 0xD0, 0xE2, 0xC0, 0xA8, 0x02, 0x06, 0xE0, 0x00, 0x00, 0x6B, 0x01, 0x3F, 0x01, 0x3F, 0x00, 0x3E, 0x00, 0x00, 0x12, 0x02, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x63, 0xFF, 0xFF, 0x00, 0x09, 0xBA, 0x00, 0x01, 0x9E, 0x48, 0x05, 0x0F, 0x00, 0x00, 0x45, 0xB1, 0x11, 0x51, 0x04, 0x72, 0xF9, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
   //memset(frame, 0x77, 1000); 
   printf("%lu",sizeof(frame));
    if (exanic_transmit_frame(tx, frame, sizeof(frame)) == 0)
        printf("Transmitted a frame\n");

    exanic_release_tx_buffer(tx);
    exanic_release_handle(exanic);
    return 0;
}


