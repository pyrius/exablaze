#include <stdio.h>
#include <exanic/exanic.h>
#include <exanic/fifo_rx.h>

//checking comment 

int main(void)
{
    char *device = "exanic0";
    int port = 1;

    exanic_t *exanic = exanic_acquire_handle(device);
    if (!exanic)
    {
        fprintf(stderr, "exanic_acquire_handle: %s\n", exanic_get_last_error());
        return 1;
    }

    exanic_rx_t *rx = exanic_acquire_rx_buffer(exanic, port, 0);
    if (!rx)
    {
        fprintf(stderr, "exanic_acquire_rx_buffer: %s\n", exanic_get_last_error());
        return 1;
    }

    char buf[8192];
    exanic_cycles32_t timestamp;

    while (1)
    {
        ssize_t sz = exanic_receive_frame(rx, buf, sizeof(buf), &timestamp);
        if (sz > 0)
        {
            printf("Got a valid frame\n");
            printf(buf);
            FILE *file = fopen("rec_pkt", "w");
            int results = fputs(buf, file);
            if (results == EOF) {
             // Failed to write do error code here 
             printf("could not write to a file");
             }
            fclose(file);
            break;
        }
    }

    exanic_release_rx_buffer(rx);
    exanic_release_handle(exanic);
    return 0;
}

//final comment