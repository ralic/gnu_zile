/* Exported terminal
   Copyright (c) 1997-2004 Sandro Sigala.
   Copyright (c) 2003-2004 Reuben Thomas.
   All rights reserved.

   This file is part of Zile.

   Zile is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   Zile is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with Zile; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

/*	$Id: term_ncurses.c,v 1.14 2004/05/10 16:39:56 rrt Exp $	*/

/*
 * This module exports only the `ncurses_tp' pointer.
 */

#include "config.h"

#include <stddef.h>
#include <stdarg.h>
#if HAVE_NCURSES_H
#include <ncurses.h>
#else
#include <curses.h>
#endif

#include "zile.h"
#include "extern.h"
#include "term_ncurses.h"

static Terminal thisterm = {
	/* Unitialised screen pointer */
	NULL,

	/* Uninitialized width and height. */
	-1, -1,

	/* Pointers to ncurses terminal functions. */
	term_init,
	term_open,
        term_close,
	ncurses_getkey,
	ncurses_xgetkey,
	ncurses_ungetkey,
	ncurses_refresh_cached_variables,
	term_refresh,
	ncurses_redisplay,
	ncurses_full_redisplay,
	ncurses_show_about,
	term_clear,
	term_beep,
	ncurses_minibuf_write,
	ncurses_minibuf_read,
	ncurses_minibuf_clear,
};

Terminal *ncurses_tp = &thisterm;

Font ZILE_REVERSE = A_REVERSE, ZILE_BOLD = A_BOLD;

Font C_FG_BLACK;
Font C_FG_RED;
Font C_FG_GREEN;
Font C_FG_YELLOW;
Font C_FG_BLUE;
Font C_FG_MAGENTA;
Font C_FG_CYAN;
Font C_FG_WHITE;
Font C_FG_WHITE_BG_BLUE;

void term_getyx(int *y, int *x)
{
        getyx(stdscr, *y, *x);
}

void term_move(int y, int x)
{
        move(y, x);
}

void term_clrtoeol(void)
{
        clrtoeol();
}

void term_refresh(void)
{
        refresh();
}

void term_clear(void)
{
        clear();
}

void term_addch(char c)
{
        addch(c);
}

void term_addnstr(const char *s, int len)
{
        addnstr(s, len);
}

void term_addstr(const char *s)
{
        addstr(s);
}

void term_mvaddch(int y, int x, char c)
{
        mvaddch(y, x, c);
}

void term_mvaddstr(int y, int x, char *s)
{
        mvaddstr(y, x, s);
}

void term_attrset(Font f)
{
        attrset(f);
}

int term_printw(const char *fmt, ...)
{
        int res;
        va_list valist;
        va_start(valist, fmt);
        res = vw_printw(stdscr, fmt, valist);
        va_end(valist);
        return res;
}

void term_beep(void)
{
	beep();
}

void term_init(void)
{
        C_FG_BLACK = COLOR_PAIR(ZILE_COLOR_BLACK);
        C_FG_RED = COLOR_PAIR(ZILE_COLOR_RED);
        C_FG_GREEN = COLOR_PAIR(ZILE_COLOR_GREEN);
        C_FG_YELLOW = COLOR_PAIR(ZILE_COLOR_YELLOW);
        C_FG_BLUE = COLOR_PAIR(ZILE_COLOR_BLUE);
        C_FG_MAGENTA = COLOR_PAIR(ZILE_COLOR_MAGENTA);
        C_FG_CYAN = COLOR_PAIR(ZILE_COLOR_CYAN);
        C_FG_WHITE = COLOR_PAIR(ZILE_COLOR_WHITE);
        C_FG_WHITE_BG_BLUE = COLOR_PAIR(ZILE_COLOR_BLUEBG);

        ncurses_tp->screen = newterm(NULL, stdout, stdin);
	set_term(ncurses_tp->screen);

	ncurses_tp->width = ZILE_COLS;
	ncurses_tp->height = ZILE_LINES;
}

static void init_colors(void)
{
	int fg = COLOR_WHITE;
	int bg = COLOR_BLACK;

#ifdef NCURSES_VERSION
	if (use_default_colors() == OK) {
		fg = -1;
		bg = -1;
	}
#endif

	/* "WHITE" is used as synonym of "DEFAULT". */
	init_pair(ZILE_COLOR_WHITE, fg, bg);

	init_pair(ZILE_COLOR_BLACK,   COLOR_BLACK,   bg);
	init_pair(ZILE_COLOR_RED,     COLOR_RED,     bg);
	init_pair(ZILE_COLOR_GREEN,   COLOR_GREEN,   bg);
	init_pair(ZILE_COLOR_YELLOW,  COLOR_YELLOW,  bg);
	init_pair(ZILE_COLOR_BLUE,    COLOR_BLUE,    bg);
	init_pair(ZILE_COLOR_MAGENTA, COLOR_MAGENTA, bg);
	init_pair(ZILE_COLOR_CYAN,    COLOR_CYAN,    bg);

	init_pair(ZILE_COLOR_BLUEBG,  COLOR_CYAN,    COLOR_BLUE);
}

int term_open(void)
{
	int colors = lookup_bool_variable("colors") && has_colors();

	if (colors)
		start_color();
	noecho();
	nonl();
	raw();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	if (colors)
		init_colors();

	return TRUE;
}

int term_close(void)
{
	/* Clear last line.  */
	term_move(ZILE_LINES - 1, 0);
	term_clrtoeol();
	refresh();

	/* Free memory and finish with ncurses.  */
	ncurses_free_rotation_buffers();
	endwin();
	delscreen(ncurses_tp->screen);
	ncurses_tp->screen = NULL;

	return TRUE;
}
