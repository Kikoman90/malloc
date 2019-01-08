/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 18:15:11 by fsidler           #+#    #+#             */
/*   Updated: 2019/01/08 12:17:15 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

typedef struct		s_elf {
	int		a4;
	//short	b7;
	//char	c1;
	//char	c1o;
}			t_elf;

void	display_elf(t_elf *tam) {
	(void)tam;
	//printf("{ %d, %d, %c, %c}\n", tam->a4, tam->b7, tam->c1, tam->c1o);
}

int main() {

	t_elf	**lop;
	t_elf	*momy;
	t_elf	*pop;
	t_elf	*mamene;
	t_elf	ok;

	momy = &ok;
	printf("momy %p\n", momy);
	printf("momy+1 %p\n", momy+1);

	/*
	momy = (t_elf*)malloc(sizeof(t_elf));
	momy->a4 = 0;
	momy->b7 = 1;
	momy->c1 = 'M';
	momy->c1o = 'O';*/
	/*printf("momy %p\n", momy);
	printf("momy->a4 %p\n", &momy->a4);
	printf("momy->b7 %p\n", &momy->b7);
	printf("momy->c1 %p\n", &momy->c1);
	printf("momy->c1o %p\n", &momy->c1o);*/

	pop = (t_elf*)malloc(sizeof(t_elf));
	pop->a4 = 2;
	//pop->b7 = 3;
	//pop->c1 = 'P';
	//pop->c1o = 'A';
	printf("pop %p\n", pop);

	mamene = (t_elf*)malloc(sizeof(t_elf));
	mamene->a4 = 42;
	//mamene->b7 = 43;
	//mamene->c1 = 'M';
	//mamene->c1o = 'E';
	printf("mamene %p\n", mamene);

	(void)lop;

	/*
	if (!(lop = (t_elf**)malloc(sizeof(t_elf*) * 3)))
		return (0);
	else
	{
		lop[0] = momy;
		lop[1] = pop;
		lop[2] = mamene;
		display_elf(lop[0]);
		display_elf(lop[1]);
		display_elf(lop[0] + 2);
		//free(lop + 1); // abort (pointer being freed was not allocated)
		free(lop); // ok
	}*/
	return (0);
}
