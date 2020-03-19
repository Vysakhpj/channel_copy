#include "channel.h"
void *data_generator(channel_t *ch)
{
    FILE *fptr;
    fptr = fopen("/home/span69/workspace/git/channel/ubuntu-18.04-live-server-amd64.iso", "rb");
    fseek(fptr, 0L, SEEK_END);
    long int file_length = ftell(fptr);
    size_t len = 19000;
    void *data;
    data = mmap(0, file_length, PROT_READ, MAP_PRIVATE, fileno(fptr), 0);

    for (long int i = 0; i < file_length; i++)
    {
        int byte_size;
        if ((i + len) > file_length)
        {
            len = (file_length - i);
        }
        if (fptr == NULL)
        {

            exit(1);
        }
        byte_size = channel_write(ch, (data + i), len);
        i = i + byte_size - 1;
        if (i == file_length - 1)
            channel_close(ch);
    }
    return NULL;
}

static void *print(channel_t *ch)
{
    const int len = 18000;
    int file_close;
    char data[len];
    FILE *fptr;
    fptr = fopen("read.txt", "wb");
    while (1)
    {
        memset(data, 0, len);
        file_close = is_close(ch);
        if (file_close == 1)
        {
            fclose(fptr);
            exit(0);
        }
        int ret = (channel_read(ch, data, len));
        if (ret > 0)
        {
            fwrite(data, ret, 1, fptr);

            // fflush(fptr);
        }
    }

    return NULL;
}
int main()
{
    pthread_t tid1;
    pthread_t tid2;
    channel_t ch = {0};
    channel_create(&ch, 20000);
    pthread_create(&tid1, NULL, (void *)data_generator, &ch);
    pthread_create(&tid2, NULL, (void *)print, &ch);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}