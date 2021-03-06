# Channel Copy
This is a project which uses a circular channel buffer to read and write data into a file.It works on inter thread communcation and locking mechanism. There are two thread, one used for reading the data from the source file and one to write the data to destination file. The user has the freedom to chose the length of the buffer, length of the string that has to be written into the channel buffer from the source file and the length of the string to read from the channel buffer and write to the destination file.
A [Sample Program](https://github.com/Vysakhpj/channel_copy/blob/master/sample.c) which drives the channel copy program.In this sample program, data generator API reads raw data from source file in binary format and passes the raw data to channel buffer. And a Print API which collects the raw data of user defined length from the channel buffer and writes it to the destination file.
A [Channel Copy program](https://github.com/Vysakhpj/channel_copy/blob/master/channel.c) which does the main functionality. 

## API GUIDE

#### Creating Channel

```c
int channel_create(channel_t *ch, size_t size);
```
Channel create API initialize the circular buffer and creates a pointer to circular buffer. returns a 0 if the buffer is created and returns a -1 on failure.

#### Channel Write

````c
int channel_write(channel_t *ch, const void *data, size_t len)
````
Channel write API takes the data and copy it to the circular buffer.and it returns length of the data that has been successfully written into the channel buffer.

#### Channel Read

```c
int channel_read(channel_t *ch, void *buff, size_t len)
````
Channel read API reads data from the channel buffer and copies it to the address location pointed by buff pointer.and it returns length of the data that has been successfully read from the channel buffer. 

#### Channel Destroy
````c
int channel_destroy(channel_t *ch)
````
Channel destroy API deletes the channel buffer when needed by the user.

#### Channel Close

````c
void channel_close(channel_t *ch)
````
Channel Close API raises a flag once every data from the source file has been read.

#### Is Close

````c
int is_close(channel_t *ch)
````
Is_Close API continuosly checks the status of the flag which is being raised the channel close API.Once the status is true it waits for the channel to be empty and once the channel is empty, it returns a 1.


## Build
A [CMAKE File](https://github.com/Vysakhpj/channel_copy/blob/master/CMakeLists.txt) which is used to compile the program. I have created a static library for channel copy program and it is linked to the sample program.
