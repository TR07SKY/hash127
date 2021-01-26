# Don't edit Makefile! Use conf-* for configuration.

SHELL=/bin/sh

default: it

accuracy: \
load accuracy.o hash127.a
	./load accuracy hash127.a 

accuracy.o: \
compile accuracy.c hash127.h int32.h
	./compile accuracy.c

accuracy2: \
load accuracy2.o hash127.a
	./load accuracy2 hash127.a 

accuracy2.o: \
compile accuracy2.c hash127.h int32.h
	./compile accuracy2.c

auto-str: \
load auto-str.o unix.a byte.a
	./load auto-str unix.a byte.a 

auto-str.o: \
compile auto-str.c buffer.h readwrite.h exit.h
	./compile auto-str.c

auto_home.c: \
auto-str conf-home
	./auto-str auto_home `head -1 conf-home` > auto_home.c

auto_home.o: \
compile auto_home.c
	./compile auto_home.c

buffer.o: \
compile buffer.c buffer.h
	./compile buffer.c

buffer_2.o: \
compile buffer_2.c readwrite.h buffer.h
	./compile buffer_2.c

buffer_copy.o: \
compile buffer_copy.c buffer.h
	./compile buffer_copy.c

buffer_get.o: \
compile buffer_get.c buffer.h byte.h error.h
	./compile buffer_get.c

buffer_put.o: \
compile buffer_put.c buffer.h str.h byte.h error.h
	./compile buffer_put.c

byte.a: \
makelib byte_copy.o byte_cr.o str_len.o
	./makelib byte.a byte_copy.o byte_cr.o str_len.o

byte_copy.o: \
compile byte_copy.c byte.h
	./compile byte_copy.c

byte_cr.o: \
compile byte_cr.c byte.h
	./compile byte_cr.c

check: \
it instcheck
	./instcheck

choose: \
warn-auto.sh choose.sh conf-home
	cat warn-auto.sh choose.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> choose
	chmod 755 choose

compile: \
warn-auto.sh conf-cc systype print-cc.sh trycpp.c
	sh print-cc.sh > compile
	chmod 755 compile

error.o: \
compile error.c error.h
	./compile error.c

error_str.o: \
compile error_str.c error.h
	./compile error_str.c

hasgethr.h: \
choose compile load trygethr.c hasgethr.h1 hasgethr.h2
	./choose cl trygethr hasgethr.h1 hasgethr.h2 > hasgethr.h

hash127.a: \
makelib hash127.o
	./makelib hash127.a hash127.o

hash127.c: \
systype conf-opt print-opt.sh opt/idea.c opt/x86-idea.c opt/pentium.c \
opt/ppro.c opt/sparc.c
	sh print-opt.sh c > hash127.c

hash127.h: \
systype conf-opt print-opt.sh opt/idea.h opt/x86-idea.h opt/pentium.h \
opt/ppro.h opt/sparc.h
	sh print-opt.sh h > hash127.h

hash127.o: \
compile hash127.c hash127.h int32.h
	./compile hash127.c

hasrdtsc.h: \
choose compile load tryrdtsc.c hasrdtsc.h1 hasrdtsc.h2
	./choose clr tryrdtsc hasrdtsc.h1 hasrdtsc.h2 > hasrdtsc.h

hasrpcc.h: \
choose compile load tryrpcc.c hasrpcc.h1 hasrpcc.h2
	./choose cl tryrpcc hasrpcc.h1 hasrpcc.h2 > hasrpcc.h

hier.o: \
compile hier.c auto_home.h
	./compile hier.c

install: \
load install.o hier.o auto_home.o unix.a byte.a
	./load install hier.o auto_home.o unix.a byte.a 

install.o: \
compile install.c buffer.h strerr.h error.h open.h readwrite.h exit.h
	./compile install.c

instcheck: \
load instcheck.o hier.o auto_home.o unix.a byte.a
	./load instcheck hier.o auto_home.o unix.a byte.a 

instcheck.o: \
compile instcheck.c strerr.h error.h readwrite.h exit.h
	./compile instcheck.c

int32.h: \
choose compile load trylong32.c int32.h1 int32.h2
	./choose clr trylong32 int32.h1 int32.h2 > int32.h

it: \
prog install instcheck

load: \
warn-auto.sh conf-ld
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo exec "`head -1 conf-ld`" \
	'-o "$$main" "$$main".o $${1+"$$@"}' \
	) > load
	chmod 755 load

makelib: \
warn-auto.sh systype
	( cat warn-auto.sh; \
	echo 'main="$$1"; shift'; \
	echo 'rm -f "$$main"'; \
	echo 'ar cr "$$main" $${1+"$$@"}'; \
	case "`cat systype`" in \
	sunos-5.*) ;; \
	unix_sv*) ;; \
	irix64-*) ;; \
	irix-*) ;; \
	dgux-*) ;; \
	hp-ux-*) ;; \
	sco*) ;; \
	*) echo 'ranlib "$$main"' ;; \
	esac \
	) > makelib
	chmod 755 makelib

open_read.o: \
compile open_read.c open.h
	./compile open_read.c

open_trunc.o: \
compile open_trunc.c open.h
	./compile open_trunc.c

prog: \
accuracy accuracy2 speed

setup: \
it install
	./install

speed: \
load speed.o hash127.a
	./load speed hash127.a 

speed.o: \
compile speed.c hash127.h int32.h timing.h hasrdtsc.h hasgethr.h \
hasrpcc.h
	./compile speed.c

str_len.o: \
compile str_len.c str.h
	./compile str_len.c

strerr_die.o: \
compile strerr_die.c buffer.h exit.h strerr.h
	./compile strerr_die.c

strerr_sys.o: \
compile strerr_sys.c error.h strerr.h
	./compile strerr_sys.c

systype: \
find-systype.sh trycpp.c x86cpuid.c
	sh find-systype.sh > systype

unix.a: \
makelib buffer.o buffer_2.o buffer_copy.o buffer_get.o buffer_put.o \
error.o error_str.o open_read.o open_trunc.o strerr_die.o \
strerr_sys.o
	./makelib unix.a buffer.o buffer_2.o buffer_copy.o \
	buffer_get.o buffer_put.o error.o error_str.o open_read.o \
	open_trunc.o strerr_die.o strerr_sys.o
