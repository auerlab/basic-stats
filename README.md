# basic-stats

## Description

Basic-stats is a command-line program and a C library
for quickly computing statistics on tabular data.
It eliminates the nuisance of importing data into a spreadsheet
or writing custom scripts in order to gather basic statistics.

Basic-stats
computes common statistics such as mean, median, etc. on rows and/or columns
of tabular data such as a TSV or CSV file.
It can compute multiple functions on multiple rows and/or
columns in a single pass through the input stream.

The default delimiter is either a single TAB or multiple space characters. 
Alternate delimiters, such as a comma, can be specified using, e.g.
--delim ','.

Quantiles are computed using a generalized interpolation formula (method 4
from the Wikipedia page on quartiles).

The output format is designed to be both human-readable and easily parsed
by command-line tools and scripts.

Basic-stats is written entirely in C for maximal performance.  Memory use
is trivial for many functions, but some computations such as quantiles
which involve sorting the data require the use of arrays.

## Example

```
> cat sample.txt    
1    5  21
2   12  22
3   13  23
4   14  24
5   15  25
6   16  26
7   17  27
8   18  28
9   19  29
10  20  30
> basic-stats mean --col 1 median --row 2 quantile 4 --col 3 < sample.txt
Col 1 mean           5.500000
Row 2 low            2.000000
Row 2 quantile(0.50) 12.000000
Row 2 high           22.000000
Col 3 low            21.000000
Col 3 quantile(0.25) 22.750000
Col 3 quantile(0.50) 25.500000
Col 3 quantile(0.75) 28.250000
Col 3 high           30.000000
```

## Design and Implementation

The code is organized following basic object-oriented design principals, but
implemented in C to minimize overhead and keep the source code accessible to
scientists who don't have time to master the complexities of C++.

Structures are treated as classes, with accessor macros and mutator functions
provided, so dependent applications and libraries need not access
structure members directly.  Since the C language cannot enforce this, it's
up to application programmers to exercise self-discipline.

For detailed coding standards, see
https://github.com/outpaddling/Coding-Standards/.

## Building and installing

basic-stats is intended to build cleanly in any POSIX environment on any CPU
architecture.  Please don't hesitate to open an issue if you encounter
problems on any Unix-like system.

Primary development is done on FreeBSD with clang, but the code is frequently
tested on Linux, MacOS, NetBSD, and OpenIndiana as well.  MS Windows is not supported,
unless using a POSIX environment such as Cygwin or Windows Subsystem for Linux.

The Makefile is designed to be friendly to package managers, such as
[Debian packages](https://www.debian.org/distrib/packages),
[FreeBSD ports](https://www.freebsd.org/ports/),
[MacPorts](https://www.macports.org/), [pkgsrc](https://pkgsrc.org/), etc.
End users should install via one of these if at all possible.

I maintain a FreeBSD port and a pkgsrc package, which is sufficient to install
cleanly on virtually any POSIX platform.  If you would like to see a
package in another package manager, please consider creating a package
yourself.  This will be one of the easiest packages in the collection and
hence a good vehicle to learn how to create packages.

For an overview of available package managers, see the
[Repology website](https://repology.org/).

### Installing basic-stats on FreeBSD:

FreeBSD is a highly underrated platform for scientific computing, with over
2,000 scientific libraries and applications in the FreeBSD ports collection
(of more than 30,000 total), modern clang compiler, fully-integrated ZFS
filesystem, and renowned security, performance, and reliability.
FreeBSD has a somewhat well-earned reputation for being difficult to set up
and manage compared to user-friendly systems like [Ubuntu](https://ubuntu.com/).
However, if you're a little bit Unix-savvy, you can very quickly set up a
workstation, laptop, or VM using
[desktop-installer](http://www.acadix.biz/desktop-installer.php).

To install the binary package on FreeBSD:

```
pkg install basic-stats
```
You can just as easily build and install from source.  This is useful for
FreeBSD ports with special build options, for building with non-portable
optimizations such as -march=native, and for 
[work-in-progress ports](https://github.com/outpaddling/freebsd-ports-wip),
for which binary packages are not yet maintained.

```
cd /usr/ports/biology/basic-stats && env CFLAGS='-march=native -O2' make install
cd /usr/ports/wip/basic-stats && make install
```

### Installing via pkgsrc

pkgsrc is a cross-platform package manager that works on any Unix-like
platform. It is native to [NetBSD](https://www.netbsd.org/) and well-supported
on [Illumos](https://illumos.org/), [MacOS](https://www.apple.com/macos/),
[RHEL](https://www.redhat.com)/[CentOS](https://www.centos.org/), and
many other Linux distributions.
Using pkgsrc does not require admin privileges.  You can install a pkgsrc
tree in any directory to which you have write access and easily install any
of the nearly 20,000 packages in the collection.

The
[auto-pkgsrc-setup](https://github.com/outpaddling/auto-admin/blob/master/User-scripts/auto-pkgsrc-setup)
script will help you install pkgsrc in about 10 minutes.  Just download it
and run

```
sh auto-pkgsrc-setup
```

Then, assuming you selected current packages and the default prefix

```
source ~/Pkgsrc/pkg/etc/pkgsrc.sh   # Or pkgsrc.csh for csh or tcsh
cd ~/Pkgsrc/math/basic-stats
sbmake install clean clean-depends
```

See the pkgsrc documentation for more information.

Community support for pkgsrc is available through the
[pkgsrc-users](http://netbsd.org/mailinglists) mailing list.

### Building basic-stats locally

Below are cave man install instructions for development purposes, not
recommended for regular use.

basic-stats depends on [libxtend](https://github.com/outpaddling/libxtend).
Install libxtend before attempting to build basic-stats.

1. Clone the repository
2. Run "make depend" to update Makefile.depend
3. Run "make install"

The default install prefix is ../local.  Clone basic-stats, libxtend and dependent
apps into sibling directories so that ../local represents a common path to all
of them.

To facilitate incorporation into package managers, the Makefile respects
standard make/environment variables such as CC, CFLAGS, PREFIX, etc.  

The library, headers, and man pages are installed under
`${DESTDIR}${PREFIX}`.  DESTDIR is empty by default and is primarily used by
package managers to stage installations.  PREFIX defaults to ../local.

Add-on libraries required for the build, such as biolibc, should be found
under either `${PREFIX}` or `${LOCALBASE}`, which defaults to `${PREFIX}`.
LOCALBASE can be set independently if you want to use libraries installed
by FreeBSD ports (/usr/local), MacPorts (/opt/local), pkgsrc (/usr/pkg), etc.

To install directly to /myprefix, assuming biolibc is installed there as well,
using a make variable:

```
make PREFIX=/myprefix clean depend install
```

Using an environment variable:

```
# C-shell and derivatives
setenv PREFIX /myprefix
make clean depend install

# Bourne shell and derivatives
PREFIX=/myprefix
export PREFIX
make clean depend install
```

View the Makefile for full details.
