#include <sys/mman.h>
#include <sys/types.h>

#include <errno.h>
#include <float.h>
#include <langinfo.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include "dx_stdio_utils.h"
#include "../dx_libc_prefix.h"

__BEGIN_DECLS

int dx_sprintf(char* s, const char* fmt, ...);

int dx_vsprintf(char* s, const char* fmt, va_list ap);

int __dxvfprintf(dxFILE* fp, const char* fmt0, va_list ap);

int dx_vsnprintf(char* s, size_t n, const char* fmt, va_list ap);

__END_DECLS
// extern char *__ldtoa(long double *ld, int mode, int ndigits, int *decpt, int *sign,
//     char **rve);
    
// extern char *__hdtoa(double d, const char *xdigs, int ndigits, int *decpt, int *sign,
//     char **rve);

// extern char *__hldtoa(long double e, const char *xdigs, int ndigits, int *decpt, int *sign,
//     char **rve);

#define	RADIXCHAR	50	/* radix char */

union arg {
  int intarg;
  unsigned int uintarg;
  long longarg;
  unsigned long ulongarg;
  long long longlongarg;
  unsigned long long ulonglongarg;
  ptrdiff_t ptrdiffarg;
  size_t sizearg;
  ssize_t ssizearg;
  intmax_t intmaxarg;
  uintmax_t uintmaxarg;
  void* pvoidarg;
  char* pchararg;
  signed char* pschararg;
  short* pshortarg;
  int* pintarg;
  long* plongarg;
  long long* plonglongarg;
  ptrdiff_t* pptrdiffarg;
  ssize_t* pssizearg;
  intmax_t* pintmaxarg;
  double doublearg;
  long double longdoublearg;
  wint_t wintarg;
  wchar_t* pwchararg;
};

// Helper function for `fprintf to unbuffered unix file': creates a
// temporary buffer.  We only work on write-only files; this avoids
// worries about ungetc buffers and so forth.
static int __dxsbprintf(dxFILE* fp, const char* fmt, va_list ap) {
  dxFILE fake;
  struct __dxsfileext fakeext;
  unsigned char buf[BUFSIZ];

  _FILEEXT_SETUP(&fake, &fakeext);
  /* copy the important variables */
  fake._flags = fp->_flags & ~__SNBF;
  fake._file = fp->_file;
  fake._cookie = fp->_cookie;
  fake._write = fp->_write;

  /* set up the buffer */
  fake._bf._base = fake._p = buf;
  fake._bf._size = fake._w = sizeof(buf);
  fake._lbfsize = 0; /* not actually used, but Just In Case */

  /* do the work, then copy any error status */
  int ret = __dxvfprintf(&fake, fmt, ap);
  if (ret >= 0 && __dxsflush(&fake)) ret = EOF;
  if (fake._flags & __SERR) fp->_flags |= __SERR;
  return ret;
}

static int __find_arguments(const char* fmt0, va_list ap, union arg** argtable, size_t* argtablesiz);
static int __grow_type_table(unsigned char** typetable, int* tablesize);

#define DEFPREC 6

#define to_digit(c) ((c) - '0')
#define is_digit(c) ((unsigned)to_digit(c) <= 9)
#define to_char(n) ((char)((n) + '0'))

template <typename CharT>
static int exponent(CharT* p0, int exp, int fmtch) {
  CharT* p = p0;
  *p++ = fmtch;
  if (exp < 0) {
    exp = -exp;
    *p++ = '-';
  } else {
    *p++ = '+';
  }

  CharT expbuf[MAXEXPDIG];
  CharT* t = expbuf + MAXEXPDIG;
  if (exp > 9) {
    do {
      *--t = to_char(exp % 10);
    } while ((exp /= 10) > 9);
    *--t = to_char(exp);
    for (; t < expbuf + MAXEXPDIG; *p++ = *t++) /* nothing */;
  } else {
    /*
     * Exponents for decimal floating point conversions
     * (%[eEgG]) must be at least two characters long,
     * whereas exponents for hexadecimal conversions can
     * be only one character long.
     */
    if (fmtch == 'e' || fmtch == 'E') *p++ = '0';
    *p++ = to_char(exp);
  }
  return (p - p0);
}

#define PAD(howmany, with)     \
  do {                         \
    if ((n = (howmany)) > 0) { \
      while (n > PADSIZE) {    \
        PRINT(with, PADSIZE);  \
        n -= PADSIZE;          \
      }                        \
      PRINT(with, n);          \
    }                          \
  } while (0)

#define PRINTANDPAD(p, ep, len, with)       \
  do {                                      \
    n2 = (ep) - (p);                        \
    if (n2 > (len)) n2 = (len);             \
    if (n2 > 0) PRINT((p), n2);             \
    PAD((len) - (n2 > 0 ? n2 : 0), (with)); \
  } while (0)

/*
 * The size of the buffer we use as scratch space for integer
 * conversions, among other things.  Technically, we would need the
 * most space for base 10 conversions with thousands' grouping
 * characters between each pair of digits.  100 bytes is a
 * conservative overestimate even for a 128-bit uintmax_t.
 */
#define BUF 100

#define STATIC_ARG_TBL_SIZE 8 /* Size of static argument table. */

/*
 * Flags used during conversion.
 */
#define ALT 0x0001      /* alternate form */
#define LADJUST 0x0004  /* left adjustment */
#define LONGDBL 0x0008  /* long double */
#define LONGINT 0x0010  /* long integer */
#define LLONGINT 0x0020 /* long long integer */
#define SHORTINT 0x0040 /* short integer */
#define ZEROPAD 0x0080  /* zero (as opposed to blank) pad */
#define FPT 0x0100      /* Floating point number */
#define PTRINT 0x0200   /* (unsigned) ptrdiff_t */
#define SIZEINT 0x0400  /* (signed) size_t */
#define CHARINT 0x0800  /* 8 bit integer */
#define MAXINT 0x1000   /* largest integer size (intmax_t) */

/*
 * Type ids for argument type table.
 */
#define T_UNUSED 0
#define T_SHORT 1
#define T_U_SHORT 2
#define TP_SHORT 3
#define T_INT 4
#define T_U_INT 5
#define TP_INT 6
#define T_LONG 7
#define T_U_LONG 8
#define TP_LONG 9
#define T_LLONG 10
#define T_U_LLONG 11
#define TP_LLONG 12
#define T_DOUBLE 13
#define T_LONG_DOUBLE 14
#define TP_CHAR 15
#define TP_VOID 16
#define T_PTRINT 17
#define TP_PTRINT 18
#define T_SIZEINT 19
#define T_SSIZEINT 20
#define TP_SSIZEINT 21
#define T_MAXINT 22
#define T_MAXUINT 23
#define TP_MAXINT 24
#define T_CHAR 25
#define T_U_CHAR 26
#define T_WINT 27
#define TP_WCHAR 28

// To extend shorts properly, we need both signed and unsigned
// argument extraction methods.
#define SARG()                                                                               \
  ((intmax_t)(flags & MAXINT                                                                 \
                  ? GETARG(intmax_t)                                                         \
                  : flags & LLONGINT                                                         \
                        ? GETARG(long long)                                                  \
                        : flags & LONGINT                                                    \
                              ? GETARG(long)                                                 \
                              : flags & PTRINT                                               \
                                    ? GETARG(ptrdiff_t)                                      \
                                    : flags & SIZEINT                                        \
                                          ? GETARG(ssize_t)                                  \
                                          : flags & SHORTINT                                 \
                                                ? (short)GETARG(int)                         \
                                                : flags & CHARINT ? (signed char)GETARG(int) \
                                                                  : GETARG(int)))
#define UARG()                                                                                \
  ((uintmax_t)(flags & MAXINT                                                                 \
                   ? GETARG(uintmax_t)                                                        \
                   : flags & LLONGINT                                                         \
                         ? GETARG(unsigned long long)                                         \
                         : flags & LONGINT                                                    \
                               ? GETARG(unsigned long)                                        \
                               : flags & PTRINT ? (uintptr_t)GETARG(ptrdiff_t) : /* XXX */    \
                                     flags & SIZEINT                                          \
                                         ? GETARG(size_t)                                     \
                                         : flags & SHORTINT                                   \
                                               ? (unsigned short)GETARG(int)                  \
                                               : flags & CHARINT ? (unsigned char)GETARG(int) \
                                                                 : GETARG(unsigned int)))

// Append a digit to a value and check for overflow.
#define APPEND_DIGIT(val, dig)                            \
  do {                                                    \
    if ((val) > INT_MAX / 10) goto overflow;              \
    (val) *= 10;                                          \
    if ((val) > INT_MAX - to_digit((dig))) goto overflow; \
    (val) += to_digit((dig));                             \
  } while (0)

// Get * arguments, including the form *nn$.  Preserve the nextarg
// that the argument can be gotten once the type is determined.
#define GETASTER(val)                                                     \
  n2 = 0;                                                                 \
  cp = fmt;                                                               \
  while (is_digit(*cp)) {                                                 \
    APPEND_DIGIT(n2, *cp);                                                \
    cp++;                                                                 \
  }                                                                       \
  if (*cp == '$') {                                                       \
    int hold = nextarg;                                                   \
    if (argtable == NULL) {                                               \
      argtable = statargtable;                                            \
      if (__find_arguments(fmt0, orgap, &argtable, &argtablesiz) == -1) { \
        ret = -1;                                                         \
        goto error;                                                       \
      }                                                                   \
    }                                                                     \
    nextarg = n2;                                                         \
    val = GETARG(int);                                                    \
    nextarg = hold;                                                       \
    fmt = ++cp;                                                           \
  } else {                                                                \
    val = GETARG(int);                                                    \
  }

// Get the argument indexed by nextarg.   If the argument table is
// built, use it to get the argument.  If its not, get the next
// argument (and arguments must be gotten sequentially).
#define GETARG(type) \
  ((argtable != NULL) ? *((type*)(&argtable[nextarg++])) : (nextarg++, va_arg(ap, type)))

/*
 * Find all arguments when a positional parameter is encountered.  Returns a
 * table, indexed by argument number, of pointers to each arguments.  The
 * initial argument table should be an array of STATIC_ARG_TBL_SIZE entries.
 * It will be replaced with a mmap-ed one if it overflows (malloc cannot be
 * used since we are attempting to make snprintf thread safe, and alloca is
 * problematic since we have nested functions..)
 */
static int __find_arguments(const char* fmt0, va_list ap, union arg** argtable,
                            size_t* argtablesiz) {
  int ch;                   /* character from fmt */
  int n, n2;                /* handy integer (short term usage) */
  int flags;                /* flags as above */
  unsigned char* typetable; /* table of types */
  unsigned char stattypetable[STATIC_ARG_TBL_SIZE];
  int tablesize; /* current size of type table */
  int tablemax;  /* largest used index in table */
  int nextarg;   /* 1-based argument index */
  int ret = 0;   /* return value */

  /*
   * Add an argument type to the table, expanding if necessary.
   */
#define ADDTYPE(type)                                                      \
  ((nextarg >= tablesize) ? __grow_type_table(&typetable, &tablesize) : 0, \
   (nextarg > tablemax) ? tablemax = nextarg : 0, typetable[nextarg++] = type)

#define ADDSARG()                                                                             \
  ((flags & MAXINT)                                                                           \
       ? ADDTYPE(T_MAXINT)                                                                    \
       : ((flags & PTRINT) ? ADDTYPE(T_PTRINT)                                                \
                           : ((flags & SIZEINT)                                               \
                                  ? ADDTYPE(T_SSIZEINT)                                       \
                                  : ((flags & LLONGINT)                                       \
                                         ? ADDTYPE(T_LLONG)                                   \
                                         : ((flags & LONGINT)                                 \
                                                ? ADDTYPE(T_LONG)                             \
                                                : ((flags & SHORTINT)                         \
                                                       ? ADDTYPE(T_SHORT)                     \
                                                       : ((flags & CHARINT) ? ADDTYPE(T_CHAR) \
                                                                            : ADDTYPE(T_INT))))))))

#define ADDUARG()                                                                  \
  ((flags & MAXINT)                                                                \
       ? ADDTYPE(T_MAXUINT)                                                        \
       : ((flags & PTRINT)                                                         \
              ? ADDTYPE(T_PTRINT)                                                  \
              : ((flags & SIZEINT)                                                 \
                     ? ADDTYPE(T_SIZEINT)                                          \
                     : ((flags & LLONGINT)                                         \
                            ? ADDTYPE(T_U_LLONG)                                   \
                            : ((flags & LONGINT)                                   \
                                   ? ADDTYPE(T_U_LONG)                             \
                                   : ((flags & SHORTINT)                           \
                                          ? ADDTYPE(T_U_SHORT)                     \
                                          : ((flags & CHARINT) ? ADDTYPE(T_U_CHAR) \
                                                               : ADDTYPE(T_U_INT))))))))

  /*
   * Add * arguments to the type array.
   */
#define ADDASTER()         \
  n2 = 0;                  \
  cp = fmt;                \
  while (is_digit(*cp)) {  \
    APPEND_DIGIT(n2, *cp); \
    cp++;                  \
  }                        \
  if (*cp == '$') {        \
    int hold = nextarg;    \
    nextarg = n2;          \
    ADDTYPE(T_INT);        \
    nextarg = hold;        \
    fmt = ++cp;            \
  } else {                 \
    ADDTYPE(T_INT);        \
  }
  char* fmt = const_cast<char*>(fmt0);
  char* cp;
  typetable = stattypetable;
  tablesize = STATIC_ARG_TBL_SIZE;
  tablemax = 0;
  nextarg = 1;
  memset(typetable, T_UNUSED, STATIC_ARG_TBL_SIZE);

  /*
   * Scan the format for conversions (`%' character).
   */
  for (;;) {
    for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++) continue;
    if (ch == '\0') goto done;
    fmt++; /* skip over '%' */

    flags = 0;

  rflag:
    ch = *fmt++;
  reswitch:
    switch (ch) {
      case ' ':
      case '#':
      case '\'':
        goto rflag;
      case '*':
        ADDASTER();
        goto rflag;
      case '-':
      case '+':
        goto rflag;
      case '.':
        if ((ch = *fmt++) == '*') {
          ADDASTER();
          goto rflag;
        }
        while (is_digit(ch)) {
          ch = *fmt++;
        }
        goto reswitch;
      case '0':
        goto rflag;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        n = 0;
        do {
          APPEND_DIGIT(n, ch);
          ch = *fmt++;
        } while (is_digit(ch));
        if (ch == '$') {
          nextarg = n;
          goto rflag;
        }
        goto reswitch;
      case 'L':
        flags |= LONGDBL;
        goto rflag;
      case 'h':
        if (*fmt == 'h') {
          fmt++;
          flags |= CHARINT;
        } else {
          flags |= SHORTINT;
        }
        goto rflag;
      case 'j':
        flags |= MAXINT;
        goto rflag;
      case 'l':
        if (*fmt == 'l') {
          fmt++;
          flags |= LLONGINT;
        } else {
          flags |= LONGINT;
        }
        goto rflag;
      case 'q':
        flags |= LLONGINT;
        goto rflag;
      case 't':
        flags |= PTRINT;
        goto rflag;
      case 'z':
        flags |= SIZEINT;
        goto rflag;
      case 'C':
        flags |= LONGINT;
        /*FALLTHROUGH*/
      case 'c':
        if (flags & LONGINT)
          ADDTYPE(T_WINT);
        else
          ADDTYPE(T_INT);
        break;
      case 'D':
        flags |= LONGINT;
        /*FALLTHROUGH*/
      case 'd':
      case 'i':
        ADDSARG();
        break;
      case 'a':
      case 'A':
      case 'e':
      case 'E':
      case 'f':
      case 'F':
      case 'g':
      case 'G':
        if (flags & LONGDBL)
          ADDTYPE(T_LONG_DOUBLE);
        else
          ADDTYPE(T_DOUBLE);
        break;
#ifndef NO_PRINTF_PERCENT_N
      case 'n':
        if (flags & LLONGINT)
          ADDTYPE(TP_LLONG);
        else if (flags & LONGINT)
          ADDTYPE(TP_LONG);
        else if (flags & SHORTINT)
          ADDTYPE(TP_SHORT);
        else if (flags & PTRINT)
          ADDTYPE(TP_PTRINT);
        else if (flags & SIZEINT)
          ADDTYPE(TP_SSIZEINT);
        else if (flags & MAXINT)
          ADDTYPE(TP_MAXINT);
        else
          ADDTYPE(TP_INT);
        continue; /* no output */
#endif            /* NO_PRINTF_PERCENT_N */
      case 'O':
        flags |= LONGINT;
        /*FALLTHROUGH*/
      case 'o':
        ADDUARG();
        break;
      case 'p':
        ADDTYPE(TP_VOID);
        break;
      case 'S':
        flags |= LONGINT;
        /*FALLTHROUGH*/
      case 's':
        ADDTYPE((flags & LONGINT) ? TP_WCHAR : TP_CHAR);
        break;
      case 'U':
        flags |= LONGINT;
        /*FALLTHROUGH*/
      case 'u':
      case 'X':
      case 'x':
        ADDUARG();
        break;
      default: /* "%?" prints ?, unless ? is NUL */
        if (ch == '\0') goto done;
        break;
    }
  }
done:
  /*
   * Build the argument table.
   */
  if (tablemax >= STATIC_ARG_TBL_SIZE) {
    *argtablesiz = sizeof(union arg) * (tablemax + 1);
    *argtable = static_cast<arg*>(mmap(NULL, *argtablesiz,
                                       PROT_WRITE | PROT_READ,
                                       MAP_ANON | MAP_PRIVATE, -1, 0));
    if (*argtable == MAP_FAILED) return -1;
  }

  for (n = 1; n <= tablemax; n++) {
    switch (typetable[n]) {
      case T_UNUSED:
      case T_CHAR:
      case T_U_CHAR:
      case T_SHORT:
      case T_U_SHORT:
      case T_INT:
        (*argtable)[n].intarg = va_arg(ap, int);
        break;
      case TP_SHORT:
        (*argtable)[n].pshortarg = va_arg(ap, short*);
        break;
      case T_U_INT:
        (*argtable)[n].uintarg = va_arg(ap, unsigned int);
        break;
      case TP_INT:
        (*argtable)[n].pintarg = va_arg(ap, int*);
        break;
      case T_LONG:
        (*argtable)[n].longarg = va_arg(ap, long);
        break;
      case T_U_LONG:
        (*argtable)[n].ulongarg = va_arg(ap, unsigned long);
        break;
      case TP_LONG:
        (*argtable)[n].plongarg = va_arg(ap, long*);
        break;
      case T_LLONG:
        (*argtable)[n].longlongarg = va_arg(ap, long long);
        break;
      case T_U_LLONG:
        (*argtable)[n].ulonglongarg = va_arg(ap, unsigned long long);
        break;
      case TP_LLONG:
        (*argtable)[n].plonglongarg = va_arg(ap, long long*);
        break;
      case T_DOUBLE:
        (*argtable)[n].doublearg = va_arg(ap, double);
        break;
      case T_LONG_DOUBLE:
        (*argtable)[n].longdoublearg = va_arg(ap, long double);
        break;
      case TP_CHAR:
        (*argtable)[n].pchararg = va_arg(ap, char*);
        break;
      case TP_VOID:
        (*argtable)[n].pvoidarg = va_arg(ap, void*);
        break;
      case T_PTRINT:
        (*argtable)[n].ptrdiffarg = va_arg(ap, ptrdiff_t);
        break;
      case TP_PTRINT:
        (*argtable)[n].pptrdiffarg = va_arg(ap, ptrdiff_t*);
        break;
      case T_SIZEINT:
        (*argtable)[n].sizearg = va_arg(ap, size_t);
        break;
      case T_SSIZEINT:
        (*argtable)[n].ssizearg = va_arg(ap, ssize_t);
        break;
      case TP_SSIZEINT:
        (*argtable)[n].pssizearg = va_arg(ap, ssize_t*);
        break;
      case T_MAXINT:
        (*argtable)[n].intmaxarg = va_arg(ap, intmax_t);
        break;
      case T_MAXUINT:
        (*argtable)[n].uintmaxarg = va_arg(ap, uintmax_t);
        break;
      case TP_MAXINT:
        (*argtable)[n].pintmaxarg = va_arg(ap, intmax_t*);
        break;
      case T_WINT:
        (*argtable)[n].wintarg = va_arg(ap, wint_t);
        break;
      case TP_WCHAR:
        (*argtable)[n].pwchararg = va_arg(ap, wchar_t*);
        break;
    }
  }
  goto finish;

overflow:
  errno = ENOMEM;
  ret = -1;

finish:
  if (typetable != NULL && typetable != stattypetable) {
    munmap(typetable, *argtablesiz);
    typetable = NULL;
  }
  return (ret);
}

void dx_bcopy(const void* src, void* dst, size_t n) {
  memmove(dst, src, n);
}
/*
 * Increase the size of the type table.
 */
static int __grow_type_table(unsigned char** typetable, int* tablesize) {
  unsigned char* old_table = *typetable;
  int new_size = *tablesize * 2;

  if (new_size < getpagesize()) new_size = getpagesize();

  if (*tablesize == STATIC_ARG_TBL_SIZE) {
    *typetable = static_cast<unsigned char*>(mmap(NULL, new_size,
                                                  PROT_WRITE | PROT_READ,
                                                  MAP_ANON | MAP_PRIVATE, -1, 0));
    if (*typetable == MAP_FAILED) return -1;
    dx_bcopy(old_table, *typetable, *tablesize);
  } else {
    unsigned char* new_table = static_cast<unsigned char*>(mmap(NULL, new_size,
                                                                PROT_WRITE | PROT_READ,
                                                                MAP_ANON | MAP_PRIVATE, -1, 0));
    if (new_table == MAP_FAILED) return -1;
    memmove(new_table, *typetable, *tablesize);
    munmap(*typetable, *tablesize);
    *typetable = new_table;
  }
  memset(*typetable + *tablesize, T_UNUSED, (new_size - *tablesize));

  *tablesize = new_size;
  return 0;
}

void dx_bzero(void* dst, size_t n) {
  memset(dst, 0, n);
}

struct helpers {
  // Flush out all the vectors defined by the given uio,
  // then reset it so that it can be reused.
  static int sprint(dxFILE* fp, struct __suio* uio) {
    if (uio->uio_resid == 0) {
      uio->uio_iovcnt = 0;
      return 0;
    }
    int result = __dxsfvwrite(fp, uio);
    uio->uio_resid = 0;
    uio->uio_iovcnt = 0;
    return result;
  }

  // Convert a wide character string argument for the %ls format to a multibyte
  // string representation. If not -1, prec specifies the maximum number of
  // bytes to output, and also means that we can't assume that the wide char
  // string is null-terminated.
  static char* wcsconv(wchar_t* wcsarg, int prec) {
    mbstate_t mbs;
    char buf[MB_LEN_MAX];
    wchar_t* p;
    char* convbuf;
    size_t clen, nbytes;

    // Allocate space for the maximum number of bytes we could output.
    if (prec < 0) {
      memset(&mbs, 0, sizeof(mbs));
      p = wcsarg;
      nbytes = wcsrtombs(NULL, (const wchar_t**)&p, 0, &mbs);
      if (nbytes == (size_t)-1) return NULL;
    } else {
      // Optimisation: if the output precision is small enough,
      // just allocate enough memory for the maximum instead of
      // scanning the string.
      if (prec < 128) {
        nbytes = prec;
      } else {
        nbytes = 0;
        p = wcsarg;
        memset(&mbs, 0, sizeof(mbs));
        for (;;) {
          clen = wcrtomb(buf, *p++, &mbs);
          if (clen == 0 || clen == (size_t)-1 || nbytes + clen > (size_t)prec) break;
          nbytes += clen;
        }
        if (clen == (size_t)-1) return NULL;
      }
    }
    if ((convbuf = static_cast<char*>(malloc(nbytes + 1))) == NULL) return NULL;

    // Fill the output buffer.
    p = wcsarg;
    memset(&mbs, 0, sizeof(mbs));
    if ((nbytes = wcsrtombs(convbuf, (const wchar_t**)&p, nbytes, &mbs)) == (size_t)-1) {
      free(convbuf);
      return NULL;
    }
    convbuf[nbytes] = '\0';
    return convbuf;
  }

  // Like __fputwc_unlock, but handles fake string (__SSTR) files properly.
  // File must already be locked.
  static wint_t xfputwc(wchar_t wc, dxFILE* fp) {
    if ((fp->_flags & __SSTR) == 0) return __dxfputwc_unlock(wc, fp);

    char buf[MB_LEN_MAX];
    mbstate_t mbs = {};
    size_t len = wcrtomb(buf, wc, &mbs);
    if (len == (size_t)-1) {
      fp->_flags |= __SERR;
      errno = EILSEQ;
      return WEOF;
    }

    struct __siov iov;
    iov.iov_base = buf;
    iov.iov_len = len;
    struct __suio uio;
    uio.uio_iov = &iov;
    uio.uio_resid = len;
    uio.uio_iovcnt = 1;
    return (__dxsfvwrite(fp, &uio) != EOF ? (wint_t)wc : WEOF);
  }

  // Convert a multibyte character string argument for the %s format to a wide
  // string representation. ``prec'' specifies the maximum number of bytes
  // to output. If ``prec'' is greater than or equal to zero, we can't assume
  // that the multibyte character string ends in a null character.
  //
  // Returns NULL on failure.
  // To find out what happened check errno for ENOMEM, EILSEQ and EINVAL.
  static wchar_t* mbsconv(char* mbsarg, int prec) {
    mbstate_t mbs;
    const char* p;
    size_t insize, nchars, nconv;

    if (mbsarg == NULL) return NULL;

    // Supplied argument is a multibyte string; convert it to wide characters first.
    if (prec >= 0) {
      // String is not guaranteed to be NUL-terminated. Find the number of characters to print.
      p = mbsarg;
      insize = nchars = nconv = 0;
      dx_bzero(&mbs, sizeof(mbs));
      while (nchars != (size_t)prec) {
        nconv = mbrlen(p, MB_CUR_MAX, &mbs);
        if (nconv == (size_t)0 || nconv == (size_t)-1 || nconv == (size_t)-2) break;
        p += nconv;
        nchars++;
        insize += nconv;
      }
      if (nconv == (size_t)-1 || nconv == (size_t)-2) return (NULL);
    } else {
      insize = dx_strlen(mbsarg);
    }

    // Allocate buffer for the result and perform the conversion,
    // converting at most `size' bytes of the input multibyte string to
    // wide characters for printing.
    wchar_t* convbuf = static_cast<wchar_t*>(calloc(insize + 1, sizeof(*convbuf)));
    if (convbuf == NULL) return NULL;
    wchar_t* wcp = convbuf;
    p = mbsarg;
    dx_bzero(&mbs, sizeof(mbs));
    nconv = 0;
    while (insize != 0) {
      nconv = mbrtowc(wcp, p, insize, &mbs);
      if (nconv == 0 || nconv == (size_t)-1 || nconv == (size_t)-2) break;
      wcp++;
      p += nconv;
      insize -= nconv;
    }
    if (nconv == (size_t)-1 || nconv == (size_t)-2) {
      free(convbuf);
      return NULL;
    }
    *wcp = '\0';

    return convbuf;
  }

};
