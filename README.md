sdate - never ending September date
-----------------------------------

```
$ sdate
Thu Sep 9718 09:13:41 CEST 1993
```

This program wraps the libc localtime() and gmtime() functions to output the
eternal September 1993 date. The epoch month is configurable (new in version
0.3).

sdate works like fakeroot (on which its code is based) by setting LD_PRELOAD to
a wrapper library.

http://www.catb.org/~esr/jargon/html/S/September-that-never-ended.html

[![Build Status](https://travis-ci.org/df7cb/sdate.svg?branch=master)](https://travis-ci.org/df7cb/sdate)

# Programs compatible with sdate

Mutt: works. It will send out septemberfied Date: headers. Depends if you like
that.

![Mutt](https://www.df7cb.de/projects/sdate/mutt.png)

xclock: works.

![xclock](https://www.df7cb.de/projects/sdate/xclock.png)

GNU date: works somewhat. date does not fully rely on strftime but does some of
the work on its own (for whatever reason) and chokes sometimes.

Please tell me your experiences with sdate so I can add the programs to this
list.

# Links

* https://www.df7cb.de/projects/sdate/
* https://github.com/df7cb/sdate
* https://travis-ci.org/github/df7cb/sdate
