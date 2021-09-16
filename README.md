# basic-stats

## Description

Basic-stats
is a program for quickly computing statistics on tabular data from the
command-line.  It elimintes the nuisance of importing data into a spreadsheet
or writing custom scripts in order to gather basic statistics.

Basic-stats
computes common statistics such as mean, median, etc. on rows and/or columns
of tabular data.  It can compute multiple functions on multiple rows and/or
columns in a single pass through the input stream.

The default delimiter is either a single TAB or multiple space characters. 
Alternate delimiters, such as a comma, can be specified using, e.g.
--delim ','.

Quantiles are computed using a generalized interpolation formula (method 4
from the Wikipedia page on quartiles).

Basic-stats is written entirely in C for maximal performance.  Memory use
is trivial for many functions, but some computations such as quantiles
which involve sorting the data require the use of arrays.

## Design and Implementation

The code is organized following basic object-oriented design principals, but
implemented in C to minimize overhead and keep the source code accessible to
scientists who don't have time to master the complexities of C++.

Structures are treated as classes, with accessor and mutator functions
(or macros) provided, so dependent applications and libraries need not access
structure members directly.  Since the C language cannot enforce this, it's
up to application programmers to exercise self-discipline.

## Building and installing

basic-stats is intended to build cleanly in any POSIX environment on any CPU
architecture.  Please don't hesitate to open an issue if you encounter
problems on any Unix-like system.

Primary development is done on FreeBSD with clang, but the code is frequently
tested on CentOS, MacOS, and NetBSD as well.  MS Windows is not supported,
unless using a POSIX environment such as Cygwin or Windows Subsystem for Linux.

The Makefile is designed to be friendly to package managers, such as
[Debian packages](https://www.debian.org/distrib/packages),
[FreeBSD ports](https://www.freebsd.org/ports/),
[MacPorts](https://www.macports.org/), [pkgsrc](https://pkgsrc.org/), etc.
End users should install via one of these if at all possible.

I maintain a FreeBSD port and a pkgsrc package, which is sufficient to install
cleanly on virtually any POSIX platform.  If you would like to see a
package in another package manager, please consider creating
it yourself.  This will be one of the easiest packages in the collection and
hence a good vehicle to learn how to create packages.

### Installing basic-stats on FreeBSD:

FreeBSD is a highly underrated platform for scientific computing, with over
1,900 scientific libraries and applications in the FreeBSD ports collection
(of more than 30,000 total), modern clang compiler, fully-integrated ZFS
filesystem, and renowned security, performance, and reliability.
FreeBSD has a somewhat well-earned reputation for being difficult to set up
and manage compared to user-friendly systems like [Ubuntu](https://ubuntu.com/).
However, if you're a little bit Unix-savvy, you can very quickly set up a
workstation, laptop, or VM using
[desktop-installer](http://www.acadix.biz/desktop-installer.php).  If
you're new to Unix, you can also reap the benefits of FreeBSD by running
[GhostBSD](https://ghostbsd.org/), a FreeBSD distribution augmented with a
graphical installer and management tools.  GhostBSD does not offer as many
options as desktop-installer, but it may be more comfortable for Unix novices.

```
pkg install basic-stats
```

### Installing via pkgsrc

pkgsrc is a cross-platform package manager that works on any Unix-like
platform. It is native to [NetBSD](https://www.netbsd.org/) and well-supported
on [Illumos](https://illumos.org/), [MacOS](https://www.apple.com/macos/),
[RHEL](https://www.redhat.com)/[CentOS](https://www.centos.org/), and
many other Linux distributions.
Using pkgsrc does not require admin privileges.  You can install a pkgsrc
tree in any directory to which you have write access and easily install any
of the nearly 20,000 packages in the collection.  The
[auto-pkgsrc-setup](http://netbsd.org/~bacon/) script can assist you with
basic setup.

First bootstrap pkgsrc using auto-pkgsrc-setup or any
other method.  Then run the following commands:

```
cd pkgsrc-dir/math/basic-stats
bmake install clean
```

There may also be binary packages available for your platform.  If this is
the case, you can install by running:

```
pkgin install basic-stats
```

See the [Joyent Cloud Services Site](https://pkgsrc.joyent.com/) for
available package sets.

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

Add-on libraries required for the build, such as libxtend, should be found
under ${LOCABASE}, which defaults to ../local.
The library, headers, and man pages are installed under
${DESTDIR}${PREFIX}.  DESTDIR is empty by default and is primarily used by
package managers to stage installations.  PREFIX defaults to ${LOCALBASE}.

To install directly to /myprefix, assuming libxtend is installed there as well,
using a make variable:

```
make LOCALBASE=/myprefix clean depend install
```

Using an environment variable:

```
# C-shell and derivatives
setenv LOCALBASE /myprefix
make clean depend install

# Bourne shell and derivatives
LOCALBASE=/myprefix
export LOCALBASE
make clean depend install
```

View the Makefile for full details.
