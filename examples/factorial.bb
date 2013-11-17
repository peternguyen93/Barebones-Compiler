# Example Bare Bones program - Factorial
# $Id: factorial.bb 12 2008-02-19 00:17:07Z eric $
# Copyright 2008 Eric Smith <eric@brouhaha.com>

# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3 as
# published by the Free Software Foundation.  Note that I am not
# granting permission to redistribute or modify this program under the
# terms of any other version of the General Public License.

# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program (in the file "COPYING"); if not, see
# <http://www.gnu.org/licenses/>.

##############################################################################

# Bare Bones example program: compute the factorial of N

# start with argument in variable N
# upon completion, result will be in variable F

# G and H used as temporaries

# NOTE: computing the factorial of arguments greater than 11 will take
# a long time.  F(12) took approximately 82 seconds to compute on an
# Opteron 246.
init N = 10;
clear F;
incr F;
decr N;

while N not 0 do;
  copy F to G;
  while G not 0 do;
    copy N to H;
    while H not 0 do;
      incr F;
      decr H;
    end;
    decr G;
  end;
  decr N;
end;
