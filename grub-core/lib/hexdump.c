/* hexdump.c - hexdump function */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2008,2009  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/lib/hexdump.h>

void
hexdump (unsigned long bse, char *buf, int len)
{
  hexdump_ext(bse, buf, len, 80, 0);
}

void
hexdump_ext (unsigned long bse, char *buf, int len, unsigned line_max, char disable_hex)
{
  int pos;
  char line[2048];
  unsigned lines = (line_max > sizeof (line)) ? sizeof (line) : line_max;

  if (0U + len < line_max)
    len = lines;

  while (len > 0)
    {
      int cnt, i;

      pos = grub_snprintf (line, lines, "%08lx  ", bse);
      cnt = lines / 5;
      if (cnt > len)
	cnt = len;

      for (i = 0; i < cnt && !disable_hex; i++)
	{
	  pos += grub_snprintf (&line[pos], lines - pos,
				"%02x ", (unsigned char) buf[i]);
	  if ((i & 7) == 7)
	    line[pos++] = ' ';
	}

      for (; i < cnt && !disable_hex; i++)
	{
	  pos += grub_snprintf (&line[pos], lines - pos, "   ");
	  if ((i & 7) == 7)
	    line[pos++] = ' ';
	}

      line[pos++] = '|';

      for (i = 0; i < cnt; i++) {
	line[pos++] = ((buf[i] >= 32) && (buf[i] < 127)) ? buf[i] : '.';
	if (disable_hex && (buf[i] == '\n')) {
	  break;
	}
      }

      line[pos++] = '|';

      line[pos] = 0;

      grub_printf ("%s\n", line);

      /* Print only first and last line if more than 3 lines are identical.  */
      if (len >= 4 * cnt
	  && ! grub_memcmp (buf, buf + 1 * cnt, cnt)
	  && ! grub_memcmp (buf, buf + 2 * cnt, cnt)
	  && ! grub_memcmp (buf, buf + 3 * cnt, cnt))
	{
	  grub_printf ("*\n");
	  do
	    {
	      bse += cnt;
	      buf += cnt;
	      len -= cnt;
	    }
	  while (len >= 3 * cnt && ! grub_memcmp (buf, buf + 2 * cnt, cnt));
	}

      bse += ((disable_hex && i < cnt) ? i + 1 : cnt);
      buf += ((disable_hex && i < cnt) ? i + 1 : cnt);
      len -= ((disable_hex && i < cnt) ? i + 1 : cnt);
    }
}
