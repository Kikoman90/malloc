#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int     ft_strlen(char const *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

void	ft_putstr(char const *s)
{
	write(1, s, ft_strlen(s));
}

void    ft_print_unsigned_long_long(unsigned long long n)
{
        if (n > 9 && n <= ULLONG_MAX)
            ft_print_unsigned_long_long(n / 10);
        ft_putchar(n % 10 + '0');
}

char	*ft_itoa_base_seize(unsigned long long value, char *buff)
{
	unsigned long long res;
	unsigned long long j = 0;
	unsigned long long symb;
	unsigned long long stock = 0;

	j = 12;
	if (value > ULLONG_MAX)	
		return (NULL);
	if (value == ULLONG_MAX)
	{
		stock = value % 10;
		value /= 10;
		j++;
	}
	res = value;
	if (stock != 0)
	{
		buff[j] = stock + '0';
		j--;
	}
	while (j > 1)
	{
		if ((res % 16) >= 10)
			buff[j] = ((res % 16 - 9) + '@');	
		else
			buff[j] = ((res % 16) + '0');
		res /= 16;
		j--;
	}
	buff[0] = '0';
	buff[1] = 'x';
	buff[13] = '\0';
	return (buff);
}

void print_addr(void *addr)
{
    char buff[14];

    ft_putstr(ft_itoa_base_seize((unsigned long long)addr, buff));
    return ;
}

void hexadiff2(void *addr1, void *addr2)
{
    ft_putstr("Start : ");
    print_addr(addr1);
    ft_putstr("\nEnd : ");
    print_addr(addr2);
    ft_putstr("\nIl y a ");
    ft_print_unsigned_long_long(addr1 - addr2);
    ft_putstr(" octet de difference !");
    ft_putchar('\n');
}


int main(void)
{
    int octet;
    int str1[] = {1,2,3,4,5,6,0};
    
    void *test = &str1;
    void *test2 = &str1[5];

    printf("%p\n", str1);
    printf("%x\n", &str1);
    //printf("%d\n", test);
    //printf("%d\n", test2);
   // printf("%d\n",((unsigned long long )test2 - (unsigned long long)test));
    /*printf("%d\n",(test2 - test));*/
    hexadiff2(test2,test);
   /* printf("0 = %p\n4 = %p\n",&str[0], &str[4]);
    octet = hexadiff(&str[0], &str[4]);
    printf("diff = %d && octet diff = %d\n", octet, (octet/ 16));*/
    //printf("start = %p\n end = %p\n",octet);
    return (0);
}