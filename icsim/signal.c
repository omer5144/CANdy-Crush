#include "signal.h"

void update_signal_status(struct canfd_frame *cf, int maxdlen, signal_status_t *signal_status)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len < SIGNAL_POS)
    {
        return;
    }
        
    if (cf->data[SIGNAL_POS] & VALUE_LEFT)
    {
        signal_status->left = 1;
    }
    else
    {
        signal_status->left = 0;
    }
    if (cf->data[SIGNAL_POS] & VALUE_RIGHT)
    {
        signal_status->right = 1;
    }
    else
    {
        signal_status->right = 0;
    }
}
