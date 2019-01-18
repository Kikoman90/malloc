// 42 header

#include "malloc.h"

void    display_meta_mem(void *addr, size_t size, size_t nb_bytes)
{
    size_t  i;

    i = 0;
    size /= nb_bytes;
    printf("\033[0mMEMORY [\n");
    while (i < size)
    {
        if (nb_bytes == 4)
            printf("mem[%zu] = %d\n", i, ((int*)(addr))[i]);
        else if (nb_bytes == 1)
            printf("mem[%zu] = %c\n", i, ((char*)(addr))[i]);
        else if (nb_bytes == 8)
            printf("mem[%zu] = %ld\n", i, ((long*)(addr))[i]);
        //addr += nb_bytes;
        i++;
    }
    printf("]\n");
}

void    display_meta(t_meta *meta, size_t *nb_bytes, int display_mem)
{
    t_meta  *list;
    int i  = 0;

    printf("**********************_DISPLAY_META_*************************\n");
    list = meta;
    while (list)
    {
        (list->used) ? printf("\033[31mallocated memory\n") : printf("\033[32mfree memory\n");
        printf("start addr : %p\n", list->addr);
        printf("end addr : %p\n", (void *)((char *)list->addr + list->size));
        hexadiff(list->addr, (void *)((char *)list->addr + list->size));
        ft_putchar('\n');
        if (display_mem && list->used)
            display_meta_mem(list->addr, list->size, nb_bytes[i]);
        printf("-------------------------------------------------\n");
        i++;
        list = list->next;
    }
    printf("\n");
}


// 0x1f170 (allocated - color:red);
// 0x1F175 (free - color:green);
WINDOW      *init_debug_display()
{
    setlocale(LC_ALL, "");
    if (!(g_memory.d_scr = newterm(NULL, stdout, stdin)))
        return (log_error_null("error [newterm]: ", strerror(errno)));
    if (!(g_memory.d_scr = set_term(g_memory.d_scr)))
        return (log_error_null("error [set_term]: ", strerror(errno)));        
    if (!(g_memory.d_win = initscr()))
        return (log_error_null("error [initscr]: ", strerror(errno)));
    //if (!(g_memory.d_win = newwin(50, 50, 12, 12)))
    //    return (log_error_null("error [newwin]: ", strerror(errno)));
    raw();
    keypad(g_memory.d_win, TRUE);
    noecho();
    start_color();
    //init_color(COLOR_GREEN, 20, 20, 1000);
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    mvwaddch(g_memory.d_win, 2, 2, ACS_ULCORNER);
    wmove(g_memory.d_win, 2, 2);
    mvwaddch(g_memory.d_win, 2, 3, ACS_HLINE);
    wmove(g_memory.d_win, 2, 3);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(3));
    // 0x1f170 (allocated);
    // 0x1F175 (free);
    mvwprintw(g_memory.d_win, 5, 4, "%lc %lc %lc %lc %lc %lc %lc\n", 0x1F170, 0x1F170, 0x26A0, 0x1F51E, 0x1F6AB, 0x1F6A8, 0x1F4A3);
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(2));
    mvwprintw(g_memory.d_win, 4, 6, "%lc %lc %lc%lc %lc%lc %lc %lc", 0x1F175, 0x1F175, 0x25FC, 0x25FC, 0x1F22F, 0x1F536, 0x1F532, 0x1F195);
    //mvprintw(4, 5, "%lc", 0x1F526);
    mvwprintw(g_memory.d_win, 2, 4, "%lc", 0x23F9);
    mvwprintw(g_memory.d_win, 2, 6, "%lc", 0x2F1E);
    mvwprintw(g_memory.d_win, 2, 8, "%lc", 0x2F1E);
    mvwprintw(g_memory.d_win, 2, 10, "%lc", 0x3249);
    mvwprintw(g_memory.d_win, 2, 11, "\033[31m%lc", 0xAB09);
    mvwprintw(g_memory.d_win, 2, 12, "%lc", 0x2588);
    //mvaddch(2, 6, 127);
    //mvaddch(2, 7, ACS_S9);
    attroff(COLOR_PAIR(2));
    refresh();
    return (g_memory.d_win);
	//getch();			/* Wait for user input */
	//endwin();
    /*WINDOW *initscr(void);
    int endwin(void);
    bool isendwin(void);
    SCREEN *newterm(char *type, FILE *outfd, FILE *infd);
    SCREEN *set_term(SCREEN *new);
    void delscreen(SCREEN* sp);*/
}