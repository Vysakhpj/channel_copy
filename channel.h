#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h>
#include <sys/mman.h>

typedef struct
{
    int write;
    int read; // used for queue to find the first and last pointer
    size_t channel_size;
    int read_complete;
    int free_slots;
    void *buffer;
    int free;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
} channel_t;

// size is the depth of channel
extern int channel_create(channel_t *ch, size_t size);

// Returns bytes wrote. Blocks if channel is full
extern int channel_write(channel_t *ch, const void *data, size_t len);

// Returns actual number of bytes read. Blocks if channel is empty
extern int channel_read(channel_t *ch, void *buff, size_t len);

extern int channel_destroy(channel_t *ch);

void channel_close(channel_t *ch);

int is_close(channel_t *ch);
