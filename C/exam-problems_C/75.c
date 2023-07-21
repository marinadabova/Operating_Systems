// Зад. 73 2021-SE-02 Вашите колеги от съседната лаборатория са написали програма на C, която може да обработва подаден входен двоичен файл и на негова база генерира изходен двоичен файл.
// Програмата работи като encoder, който имплементира вариант на Manchester code, т.е.:
// • за всеки входен бит 1 извежда битовете 10, и
// • за всеки входен бит 0 извежда битовете 01
// Например следните 8 бита вход 1011 0110 == 0xB6 по описания алгоритъм дават следните 16 бита изход 1001 1010 0110 1001 == 0x9A69
// Напишете програма на C, която извършва обратния процес, т.е., декодира файлове, създадени от горната програма. Примерно извикване: $ ./main input.bin output.bin

#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <err.h>

bool get_bit(uint8_t byte, int idx)
{
    return ((byte & (1 << idx)) != 0);
}

uint16_t set_bit(uint16_t x, int idx, bool bit)
{
    if(bit)
    {
        return (1 << idx) | x ;
    }
    else
    {
        return (~(1 << idx)) & x ;
    }
}

uint8_t manchester_decode_byte(uint16_t x)
{
    uint8_t result = 0;
    for (int i = 0; i < 16; i += 2)
    {
        bool first = get_bit(x, i);
        bool second = get_bit(x, i + 1);

        if (!first && second)
        {
            result = set_bit(result, i / 2, 0);
        }
        else if (first && !second)
        {
            result = set_bit(result, i / 2, 1);
        }
        else
        {
            errx(1, "invalid sequence");
        }
    }

    return result;
}

void manchester_decode_bytes(uint16_t* in_buf, uint8_t* out_buf, int n)
{
    for(int i = 0; i < n; ++i)
    {
        out_buf[i] = manchester_decode_byte(in_buf[i]);
    }
}
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        warnx("%s encodes a file to manchester encoding", argv[0]);
        errx(1, "usage: %s <input file> <output file>", argv[0]);
    }
    
    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in < 0)
    {
        err(1, "Could not open %s", argv[1]);
    }

    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_out < 0)
    {
        err(1, "Could not open %s", argv[1]);
    }

    uint16_t in_buf[4096];
    uint8_t out_buf[4096];

    int bytes_read;
    while((bytes_read = read(fd_in, in_buf, sizeof(in_buf))) > 0)
    {
        manchester_decode_bytes(in_buf, out_buf, bytes_read / 2);
        int bytes_written = write(fd_out, out_buf, bytes_read / 2);
        if (bytes_written < 0)
        {
            err(1, "Could not write data to %s", argv[2]);
        }
        if (bytes_read != bytes_written)
        {
            errx(1, "Could not write all data at once");
        }
    }

    if (bytes_read < 0)
    {
        err(1, "Could not read data from %s", argv[1]);
    }
    close(fd_in);
    close(fd_out);
    return 0;
}
