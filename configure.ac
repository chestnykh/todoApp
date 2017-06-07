AC_INIT([todoApp], [0.1])

AC_CONFIG_SRCDIR($(srcdir)/src/todo.c)
AC_LANG([C])

LT_INIT
AC_CONFIG_MACRO_DIRS([m4])


AM_INIT_AUTOMAKE([1.9 foreign])



builddir=$(bindir)/build

AC_SUBST([CFLAGS], ["-ansi -O2 -Wall -D_XOPEN_SOURCE -D_GNU_SOURCE -Wpointer-compare"])
AC_CONFIG_FILES([Makefile src/Makefile])


AC_CHECK_HEADER([ncurses.h], [], [AC_MSG_FAILURE([ncurses.h header file not found!])])
AC_CHECK_HEADER([stdio.h], [], [AC_MSG_FAILURE([stdio.h header file not found!])])
AC_CHECK_HEADER([getopt.h], [], [AC_MSG_FAILURE([getopt.h header file not found!])])
AC_CHECK_HEADER([stdlib.h], [], [AC_MSG_FAILURE([stdlib.h header file not found!])])
AC_CHECK_HEADER([time.h], [], [AC_MSG_FAILURE([time.h header file not found!])])
AC_CHECK_HEADER([signal.h], [], [AC_MSG_FAILURE([signal.h header file not found!])])
AC_CHECK_HEADER([string.h], [], [AC_MSG_FAILURE([string.h header file not found!])])

AC_SEARCH_LIBS([initscr], [ncursesw], [LDFLAGS+=-lncursesw], [
		AC_SEARCH_LIBS([initscr, [ncurses], [LDFLAGS+=-lncurses], [AC_MSG_FAILURE([ncurses library not found])]])

])

AC_CANONICAL_HOST

case "$host_cpu" in
  i[[3456]]86)  CFLAGS="$CFLAGS -march=i386" ;;
  x86_64)       CFLAGS="$CFLAGS -march=x86-64" ;;
esac

AC_OUTPUT
