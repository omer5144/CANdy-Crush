#include "signals.h"

void update_signals(struct canfd_frame *cf, int maxdlen, signals_status_t *signals_status)
{
    int len = (cf->len > maxdlen) ? maxdlen : cf->len;
    if (len < SIGNAL_POS)
    {
        return;
    }
        
    if (cf->data[SIGNAL_POS] & 1)
    {
        signals_status->left = 1;
    }
    else
    {
        signals_status->left = 0;
    }
    if (cf->data[SIGNAL_POS] & 2)
    {
        signals_status->right = 1;
    }
    else
    {
        signals_status->right = 0;
    }
}
