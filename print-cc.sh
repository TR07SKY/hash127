
cc="`head -1 conf-cc`"
systype="`cat systype`"


gcc -c trycpp.c -malign-double >/dev/null 2>&1 \
&& gccad="-malign-double"

gcc -c trycpp.c -mcpu=ultrasparc >/dev/null 2>&1 \
&& gccus="-mcpu=ultrasparc"

gcc -c trycpp.c -mcpu=21164 >/dev/null 2>&1 \
&& gcc21="-mcpu=21164"

rm -f trycpp.o


case "$cc:$systype" in
  auto:*:i386-*:*:genuineintel-000006*)
    cc="gcc -O1 -fomit-frame-pointer $gccad"
    ;;
  auto:*:i386-*:*:genuineintel-000005*)
    cc="gcc -O6 -fomit-frame-pointer $gccad"
    ;;
  auto:*:i386-*:*)
    cc="gcc -O6 -fomit-frame-pointer $gccad"
    ;;
  auto:*:sparc-*:*:*:*)
    cc="gcc -O6 -fomit-frame-pointer $gccus"
    ;;
  auto:*:alpha-*:*:*:*)
    cc="gcc -O6 -fomit-frame-pointer $gcc21"
    ;;
  auto:*)
    cc="gcc -O6 -fomit-frame-pointer"
    ;;
esac


cat warn-auto.sh
echo exec "$cc" '-c ${1+"$@"}'
