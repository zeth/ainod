/***
  This file is part of ainod.

  Copyright 2017 Zeth

  ainod is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  ainod is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with ainod; If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef AINOD_SYS_H   /* Include guard */
#define AINOD_SYS_H

#pragma weak do_exit
void do_exit(void);

#pragma weak do_printf
void do_printf(char *formatted_message);


#endif // AINOD_SYS_H
