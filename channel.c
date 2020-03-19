#include "channel.h"

static inline unsigned char is_full(channel_t *ch)
{
    return (ch->free_slots == 0) ? 1 : 0;
}

static inline unsigned char is_empty(channel_t *ch)
{
    return (ch->free_slots == ch->channel_size) ? 1 : 0;
}

void channel_close(channel_t *ch)
{
    ch->read_complete = 1;
}

int is_close(channel_t *ch)
{
    if ((ch->free_slots < ch->channel_size) || (ch->read_complete == 0))
    {
        return 0;
    }

    return 1;
}
extern int channel_create(channel_t *ch, size_t size)
{
    pthread_mutex_init(&ch->mtx, NULL);
    pthread_cond_init(&ch->cond, NULL);
    ch->channel_size = size;
    ch->buffer = malloc(ch->channel_size);
    ch->read = 0;
    ch->write = 0;
    ch->free_slots = ch->channel_size;
    return ch->buffer ? 0 : -1;
}

extern int channel_write(channel_t *ch, const void *data, size_t len)
{
    pthread_mutex_lock(&ch->mtx);
    while (is_full(ch))
    {
        pthread_cond_wait(&ch->cond, &ch->mtx);
    }

    if (ch->write >= ch->channel_size)
    {
        ch->write = 0;
    }
    if (len > ch->free_slots)
        len = ch->free_slots;

    if (len > ch->channel_size - ch->write)
        len = (ch->channel_size - ch->write);

    memcpy((ch->buffer + ch->write), data, len);
    ch->free_slots = ch->free_slots - len;
    ch->write = ch->write + len;
    pthread_cond_signal(&ch->cond);
    pthread_mutex_unlock(&ch->mtx);
    return len;
}

extern int channel_read(channel_t *ch, void *buff, size_t len)
{
    pthread_mutex_lock(&ch->mtx);
    while (is_empty(ch))
    {
        pthread_cond_wait(&ch->cond, &ch->mtx);
    }

    if (ch->read >= ch->channel_size)
        ch->read = 0;

    if (len > (ch->channel_size - ch->free_slots))
    {
        len = ch->channel_size - ch->free_slots;
    }

    if (len > (ch->channel_size - ch->read))
    {
        len = ch->channel_size - ch->read;
    }

    memcpy(buff, (ch->buffer + ch->read), len);
    ch->free_slots = ch->free_slots + len;
    ch->read = ch->read + len;
    pthread_cond_signal(&ch->cond);
    pthread_mutex_unlock(&ch->mtx);
    return len;
}

extern int channel_destroy(channel_t *ch)
{
    int ret = is_close(ch);
    if (ret)
    {
        free(ch->buffer);
    }
    return 0;
}