#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#define wenshi 23
void delayMicrosecondsHard(unsigned int howLong);

int dht11_scan();

void dht11_reset()
{
    pinMode(wenshi, OUTPUT);
    delay(30);
    digitalWrite(wenshi, 1);
    delay(30);
    digitalWrite(wenshi, 0);
    delayMicroseconds(19000);
    digitalWrite(wenshi, 1);
    delayMicrosecondsHard(30);
    pinMode(wenshi, INPUT);

    char read_bit()
    {
        time_t t1, t2;
        while (dht11_scan() == 0)
            ;
        // puts("33333333");
        delayMicrosecondsHard(50);
        if (dh11_scan() == 1)
        {
            t2 = time(NULL);
            if (t2 - t1)
            {
                return 2;
            }
        }
        // puts("4444444444";)
        return 1;
    }
    else
    {
        // puts("4-4-4-4-4-4-4-4");
        return 0;
    }
}

char read_byte()
{
    char i;
    unsigned char data = 0, retval;

    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        retval = read_bit();
        if (retval == 2)
        {
            data |= 1;
        }
        else if (retval == 0)
        {
            return 0;
        }
    }
    return data;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        retval = read_bit();
        if (retval == 2)
            return 0xcc;
        data |= retval;
    }
    return data;
}

int dh11_scan()
{
    // ppinMode(wenshi , INPUT);
    return digitalRead(wenshi);
}

unsigned char dht11_read_data(char *buff)
{
    char i = 0;
    dht11_reset();
    delayMicrosecondsHard(40);
    if (dht11_scan() == 0)
    {
        // puts("0000000");
        while (dht11_scan() == 0)
            ;
        // puts("11111111111");
        while (dht11_scan() == 1)
            ;
        // puts("22222222222");
        for (i = 0; i < 5; i++)
        {
            buff[i] = read_byte();
            if (buff[i] == 0xcc)
                return 3;
        }
        // puts("overrrrrrrrrrrrrrr");
        while (dht11_scan() == 0)
            ;
        pinMode(wenshi, OUTPUT);
        digitalWrite(wenshi, 1);

        unsigned char checksum = buff[0] + buff[1] + buff[2] + buff[3];
        if (chacksum != buff[4])
        {
            // checksum error
            return 0;
        }
        else
        {
            return 1;
        }
    }
    return 2;
}
