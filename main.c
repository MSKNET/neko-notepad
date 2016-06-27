/* A simple user interface for this project

Copyright (C) 2016 Arnie97

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include <hpgcc49.h>
#include <hpgraphics.h>
#include "s3c2410.h"
#include "mpllcon.h"
#include "clkslow.h"


void
show_system_info(void)
{
	clear_screen();
	printf(
		"FREQUENCY TUNER\n\n"
		"Build 20160627 by Arnie97\n\n"
	);
	for (int i = 0; i < 6; i++) {
		delay(500000);
		hpg_set_indicator((i + 1) % 6, 0xFF);
		hpg_set_indicator(i, 0x00);
	}

	int status;
	float fclk = clkslow_to_freq(CLKSLOW, FIN);
	if (fclk) {
		// slow mode
		status = 4;
	} else {
		// normal mode
		fclk = mpllcon_to_freq(MPLLCON, FIN);
		status = is_valid_mpllcon(MPLLCON);
	}
	const char *msg[] = {
		"UNKNOWN", "DEFAULT", "CHANGED", "PROFILE",
		"SLOW MODE", "ENABLED"
	};
	printf(
		"MPLLCON %08x at %08x\n"
		"CLKSLOW %08x at %08x\n"
		"  (%.2f MHz, %s %s)\n"
		"\n",
		*MPLLCON, (unsigned)MPLLCON,
		*CLKSLOW, (unsigned)CLKSLOW,
		fclk, msg[status], msg[status == 4? 5: 3]
	);

	delay(750000);
	hpg_set_indicator(0, 0x00);
	printf(
		"CONFIG: [PLOT]   EXIT:    [APLET]"
		"ABOUT:  [SYMB]   REFRESH: [HOME]"
	);
	while (!keyb_isAnyKeyPressed());
}


void
show_freq_config(int page)
{
	while (keyb_isAnyKeyPressed());
	clear_screen();
	printf(
		"SELECT YOUR DESIRED FREQUENCY:\n\n"
	);

	int beg, end;
	char c = 'A';
	if (page == 0) {
		for (int i = 6; i >= 0; i--) {
			unsigned clkslow = 0x10 | i;
			printf(
				"[%c] %-7.2f",
				c++, clkslow_to_freq(&clkslow, FIN)
			);
		}
		beg = 0, end = 8;
	} else {
		beg = 8, end = 22;
	}
	for (int i = beg; i < end; i++) {
		printf(
			"[%c] %-7.2f",
			c++, mpllcon_to_freq(valid_mpllcon_values + i, FIN)
		);
	}
	gotoxy(0, 8);
	printf(
		"BACK:   [HOME]   EXIT:    [APLET]"
		"ABOUT:  [SYMB]   %s:    [%s]",
		page? "PREV": "NEXT", page? "UP": "DOWN"
	);
	while (!keyb_isAnyKeyPressed());
}