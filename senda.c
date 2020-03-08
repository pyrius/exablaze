#include <stdio.h>
#include <string.h>
#include <exanic/exanic.h>
#include <exanic/fifo_rx.h>
#include <exanic/fifo_tx.h>
#include <exanic/time.h>
#include <inttypes.h>

int main(void)
{
    char *device = "exanic0";
    int tx_port = 0;
    int rx_port = 1;

    exanic_t *exanic = exanic_acquire_handle(device);
    if (!exanic)
    {
        fprintf(stderr, "exanic_acquire_handle: %s\n", exanic_get_last_error());
        return 1;
    }

    exanic_tx_t *tx = exanic_acquire_tx_buffer(exanic, tx_port, 0);
    if (!tx)
    {
        fprintf(stderr, "exanic_acquire_tx_buffer: %s\n", exanic_get_last_error());
        return 1;
    }

   char frame[] = {0x64, 0x3f, 0x5f, 0x01, 0x2f, 0x43, 0x64, 0x3f, 0x5f, 0x01, 0x2f, 0x42, 0x08, 0x00, 0x45, 0x00, 0x00, 0x52, 0x45, 0x9F, 0x00, 0x00, 0x01, 0x11, 0xD0, 0xE2, 0xC0, 0xA8, 0x02, 0x06, 0xE0, 0x00, 0x00, 0x6B, 0x01, 0x3F, 0x01, 0x3F, 0x00, 0x3E, 0x00, 0x00, 0x12, 0x02, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x63, 0xFF, 0xFF, 0x00, 0x09, 0xBA, 0x00, 0x01, 0x9E, 0x48, 0x05, 0x0F, 0x00, 0x00, 0x45, 0xB1, 0x11, 0x51, 0x04, 0x72, 0xF9, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
    

    exanic_rx_t *rx = exanic_acquire_rx_buffer(exanic, rx_port, 0);
    
    if (!rx)
    {
        fprintf(stderr, "exanic_acquire_rx_buffer: %s\n", exanic_get_last_error());
        return 1;
    }

    char buf[8192];
    exanic_cycles32_t t_timestamp = 0;
    exanic_cycles32_t r_timestamp;
    exanic_cycles_t recv_ts_exp;
    exanic_cycles_t transmit_ts_exp;

    while (1)
    {
      if (exanic_transmit_frame(tx, frame, sizeof(frame)) == 0)
        {
           t_timestamp =  exanic_get_tx_timestamp(tx);
           transmit_ts_exp =  exanic_expand_timestamp(exanic, t_timestamp);
           //printf("Transmitted a frame\n");
        }	
        ssize_t sz = exanic_receive_frame(rx, buf, sizeof(buf), &r_timestamp);
        recv_ts_exp     =  exanic_expand_timestamp(exanic, r_timestamp);
        if (sz > 0)
        {
            //printf("Got a valid frame\n");
            //printf(buf);
            FILE *file = fopen("rec_pkt", "w");
            int results = fputs(buf, file);
            if (results == EOF) {
              printf("could not write to a file");
             }
             fclose(file);
             break;
          }
     }

      //exanic_cycles_t transmit_ts_exp =  exanic_expand_timestamp(exanic, t_timestamp);
      //exanic_cycles_t recv_ts_exp     =  exanic_expand_timestamp(exanic, r_timestamp);
      exanic_cycles_t t_result = recv_ts_exp - transmit_ts_exp;
      uint64_t transmit_ts = exanic_cycles_to_ns(exanic,transmit_ts_exp);
      uint64_t receive_ts = exanic_cycles_to_ns(exanic,recv_ts_exp);
      uint64_t result = receive_ts - transmit_ts;
      uint64_t ts_result = exanic_cycles_to_ns(exanic,t_result);
     // printf("Time taken for transmit %d, %d, %d ns",receive_ts, transmit_ts, result);
     printf("\n");
      //printf("%"PRIu64"\n", receive_ts);
      //printf("%"PRIu64"\n", transmit_ts);
      printf("%"PRIu64"\n", ts_result);
      /*
      printf("%lu\n", receive_ts);
      printf("%lu\n", transmit_ts);
      printf("%lu\n", result);
     */
     exanic_release_tx_buffer(tx);
     exanic_release_rx_buffer(rx);
     exanic_release_handle(exanic);
     return 0;


}


