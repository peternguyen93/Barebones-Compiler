# Example Bare Bones program - Fibonacci sequence
# $Id: fibonacci.bb 12 2008-02-19 00:17:07Z eric $
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

# Bare Bones example program: compute the Nth value in the Fibonacci sequence
#   F(n) = 0 if n = 0
#   F(n) = 1 if n = 1
#   F(n) = F(n-1) + F(n-2) if n > 1

# start with argument in variable N
# upon completion, result will be in variable F

# G and H used as temporaries

# NOTE: computing the factorial of arguments greater than 42 will take
# a long time.  Fib(43) took approximately 59 seconds to compute on an
# Opteron 246.
init N = 10;
clear F;
clear G;

while N not 0 do;
  copy G to H;
  while F not 0 do;
    incr H;
    decr F;
  end;

  copy G to F;
  copy H to G;

  decr N;
end;
