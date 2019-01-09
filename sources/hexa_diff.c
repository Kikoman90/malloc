#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int     upper_to_lowercase(char c)
{
    if(c >= 65 && c <= 90)
        return(c + 32);
    return(c);
}        

int convert_more_ten(char c)
{
    int val;

    val = 0;
    if (c == 'A')
        val = 10;
    else if (c == 'B')
        val = 11;
    else if (c == 'C')
        val = 12;
    else if (c == 'D')
        val = 13;
    else if (c == 'E')
        val = 14;
    else if (c == 'F')
        val = 15;
    else
        val = -1;
    return (val);
}

int     ft_atoi_base_seize(const char *str)
{
        int i;
        int neg;
        int nb;

        i = 0;
        neg = 0;
        nb = 0;
        while (str[i] == '\f' || str[i] == '\r' || str[i] == '\t' || str[i] == '\v'
        || str[i] == '\n' || str[i] == ' ')
                i++;
        if (str[i] == '-')
                neg = 1;
        if (str[i] == '-' || str[i] == '+')
                i++;
        while ((str[i] > 47 && str[i] < 58) || (str[i] >= 65 && str[i] <= 90) 
            || (str[i] >= 97 && str[i] <= 122)) // 65 a 90 et 97 a 122
        {
            if(str[i] >= 65 && str[i] <= 90)
            {
                upper_to_lowercase(str[i]);
                printf("test %d\n", str[i]);
                nb = nb * 16 + convert_more_ten(str[i]);
            }
            else
                nb = nb * 16 + (str[i] - 48);
            printf("test2 %d\n", nb);
            i++;
        }
        if (neg)
                return (-nb);
        return (nb);
}

int hexadiff(char *addr1, char *addr2)
{
    int i = 0;
    int j = 0;

    if (addr1[0] == '0' && addr1[1] == 'x')
        i = 2;
    if (addr2[0] == '0' && addr2[1] == 'x')
        j = 2;
    return (ft_atoi_base_seize(&addr1[i]) - ft_atoi_base_seize(&addr2[j]));
}

int main(void)
{
   int octet;

    octet = hexadiff("FFF", "AAA");
    printf("diff 0x50 & 0x23 = %d\n", octet);
    //printf("start = %p\n end = %p\n",tab,&tab[3]);
    return (0);
}