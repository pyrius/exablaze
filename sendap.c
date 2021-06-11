#include <stdio.h>
#include <string.h>
#include <exanic/fifo_tx.h>
#include <exanic/fifo_rx.h>
#include <exanic/exanic.h>
#include <exanic/time.h>
#include <unistd.h>
#include <inttypes.h>

/* Not used here - chunks must be QWORD aligned though. */
#define QWORD_ALIGN(x) (((x) + 7) & ~7)

/*
 * Arbitrary maximum chunk size. Size this is a multiple of QWORD size, we don't
 * need to explicity QWORD align.
 */
#define TX_SLOT_SIZE 256

/* Configure the header for a given slot. */
void set_slot_header(exanic_tx_t *tx, int slot, int length)
{
    struct tx_chunk *chunk =
        (struct tx_chunk *) (tx->buffer + TX_SLOT_SIZE * slot);
    size_t padding = exanic_payload_padding_bytes(EXANIC_TX_TYPE_RAW);

    chunk->feedback_id = 0x0000;            /* Not applicable. */
    chunk->feedback_slot_index = 0x8000;    /* No feedback. */
    chunk->length = padding + length ;      /* Frame size + padding. */
    chunk->type = EXANIC_TX_TYPE_RAW;       /* Only supported transmit type. */
    chunk->flags = 0;
}

/* Get a pointer to the payload for a given slot. */
char * get_slot_payload(exanic_tx_t *tx, int slot)
{
    struct tx_chunk *chunk =
        (struct tx_chunk *) (tx->buffer + TX_SLOT_SIZE * slot);
    size_t padding = exanic_payload_padding_bytes(EXANIC_TX_TYPE_RAW);

    return chunk->payload + padding;
}

/*
 * Force the write combining buffers to be flushed after pushing all of the
 * frames to the card.
 */
void flush_wc_buffers(exanic_tx_t *tx)
{
    /*
     * This should trigger a write combining flush. It is a dummy write to a
     * read only register.
     */
    tx->exanic->registers[REG_EXANIC_INDEX(REG_EXANIC_PCIE_IF_VER)]
        = 0xDEADBEEF;

    /* --> this may work too: asm volatile ("sfence" ::: "memory"); */
}

/* Trigger a send on a given slot. */
void trigger_slot_send(exanic_tx_t *tx, int slot)
{
    int offset = slot * TX_SLOT_SIZE;
    tx->exanic->registers[REG_PORT_INDEX(tx->port_number, REG_PORT_TX_COMMAND)]
        = offset + tx->buffer_offset;
}

const char fake_smac_addr[6] = {0x64, 0x3f, 0x5f, 0x01, 0xc3, 0x57};
const char fake_dmac_addr[6] = {0x64, 0x3f, 0x5f, 0x01, 0xc3, 0x56};
void generate_packet(char *packet, size_t size, uint8_t content)
{
    /* Set source and destination MAC address */
    //memset(packet, 0xFF, 6);
    //memcpy(&packet[0], fake_smac_addr, 6);
    //memcpy(&packet[6], fake_dmac_addr, 6);

    /* IP packet */
    //packet[12] = 0x08;
    //packet[13] = 0x00;

    /* Zero IP header area */
    //memset(&packet[14], 0, 20);

    /* Fill packet with content */
    //memset(&packet[34], content, size-34 );

    char packet1[] = {0x64, 0x3f, 0x5f, 0x01, 0xc3, 0x57, 0x64, 0x3f, 0x5f, 0x01, 0xc3, 0x56, 0x08, 0x00, 0x45, 0x00, 0x00, 0x52, 0x45, 0x9F, 0x00, 0x00, 0x01, 0x11, 0xD0, 0xE2, 0xC0, 0xA8, 0x02, 0x06, 0xE0, 0x00, 0x00, 0x6B, 0x01, 0x3F, 0x01, 0x3F, 0x00, 0x3E, 0x00, 0x00, 0x12, 0x02, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x63, 0xFF, 0xFF, 0x00, 0x09, 0xBA, 0x00, 0x01, 0x9E, 0x48, 0x05, 0x0F, 0x00, 0x00, 0x45, 0xB1, 0x11, 0x51, 0x04, 0x72, 0xF9, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
}


int main(void)
{
    int port_number = 0;
    char device[10] = "exanic1";
    unsigned buffer_size = 0x1000;
    exanic_t *exanic;
    exanic_tx_t *tx;
    int i;
    unsigned packet_size;
    char * payload;

    if ((exanic = exanic_acquire_handle(device)) == NULL)
    {
        fprintf(stderr, "%s: %s\n", device, exanic_get_last_error());
        return -1;
    }

  int rx_port = 1; 
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



    /* Reserve some space in the TX buffer. */
    tx = exanic_acquire_tx_buffer(exanic, port_number, buffer_size);

    /* Generate preloaded dummy packet */    
    payload = get_slot_payload(tx, 0);
    packet_size = 60;
    set_slot_header(tx, 0, packet_size);

    //generate_packet(payload, packet_size, 0);
    char frame[] = {0x64, 0x3f, 0x5f, 0x01, 0xc3, 0x57, 0x64, 0x3f, 0x5f, 0x01, 0xc3, 0x56, 0x08, 0x00, 0x45, 0x00, 0x00, 0x52, 0x45, 0x9F, 0x00, 0x00, 0x01, 0x11, 0xD0, 0xE2, 0xC0, 0xA8, 0x02, 0x06, 0xE0, 0x00, 0x00, 0x6B, 0x01, 0x3F, 0x01, 0x3F, 0x00, 0x3E, 0x00, 0x00, 0x12, 0x02, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x63, 0xFF, 0xFF, 0x00, 0x09, 0xBA, 0x00, 0x01, 0x9E, 0x48, 0x05, 0x0F, 0x00, 0x00, 0x45, 0xB1, 0x11, 0x51, 0x04, 0x72, 0xF9, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    payload = *frame;
    
    /*
     * Force a flush of the write combining buffers to sync everything to the
     * card. We do this because the ExaNIC transmit buffers are mapped in write
     * combining mode, which means that multiple writes can get buffered and
     * sent together. Normally this isn't an issue because we aren't pre-loading
     * but in this example we want all the data to be in the tx buffer well
     * ahead of time, so the transmit latency is just the time to send a command
     * to BAR0 (ie just hit the transmit register)
     */
    flush_wc_buffers(tx);

    /*
     * Trigger a send from each slot spaced 1 sec apart.
     * Note: To avoid overflowing the TX command queue, the software should make
     * sure that no more than buffer_size/EXANIC_TX_CMD_FIFO_SIZE_DIVISOR
     * transmit commands are in flight at any one time. The TX feedback
     * mechanism (not used in this example) can be used to notify the software
     * when a transmit command is finished.
     */
   exanic_cycles32_t tx_ts[10];
   exanic_cycles_t tx_ts_extended;
   uint64_t tx_ts_ns;

   for (i=0 ; i<10 ; ++i)
   {
     trigger_slot_send(tx, 0);
     tx_ts[i] = exanic_get_tx_timestamp(tx);
     transmit_ts_exp =  exanic_expand_timestamp(exanic, tx_ts[i]);
     ssize_t sz = exanic_receive_frame(rx, buf, sizeof(buf), &r_timestamp);
     recv_ts_exp     =  exanic_expand_timestamp(exanic, r_timestamp);

     usleep(1000);
   }
/*

  FILE *fp;
  fp = fopen("tx-ts.txt", "w");   
  
  for (i=0; i<10 ; ++i) 
  {
    tx_ts_extended = exanic_expand_timestamp(exanic, tx_ts[i]);
    tx_ts_ns = exanic_cycles_to_ns(exanic, tx_ts_extended);
    fprintf(fp, "%" PRIu64 "\n", tx_ts_ns);
  }

  fclose(fp);
  printf("\nTX timestamp file writen\n");  
*/


    exanic_cycles_t t_result = recv_ts_exp - transmit_ts_exp;
    uint64_t transmit_ts = exanic_cycles_to_ns(exanic,transmit_ts_exp);
    uint64_t receive_ts = exanic_cycles_to_ns(exanic,recv_ts_exp);
    uint64_t result = receive_ts - transmit_ts;
    uint64_t ts_result = exanic_cycles_to_ns(exanic,t_result);
    printf("\n");
    
    printf("%"PRIu64"\n", ts_result);

  exanic_release_tx_buffer(tx);
  exanic_release_handle(exanic);  
  return 0;
}
