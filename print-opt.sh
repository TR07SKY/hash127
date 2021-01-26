
opt="`head -1 conf-opt`"
systype="`cat systype`"


case "$opt:$systype" in
  auto:*:i386-*:*:genuineintel-000006*)
    opt=ppro
    ;;
  auto:*:i386-*:*:genuineintel-000005*)
    opt=pentium
    ;;
  auto:*:i386-*:*)
    opt=pentium
    ;;
  auto:*:sparc-*:*:*:*)
    opt=sparc
    ;;
esac


case "$opt" in
  idea)
    cat opt/idea."$1"
    ;;
  x86-idea)
    cat opt/x86-idea."$1"
    ;;
  pentium-idea)
    cat opt/pentium-idea."$1"
    ;;
  pentium)
    cat opt/pentium."$1"
    ;;
  ppro)
    cat opt/ppro."$1"
    ;;
  sparc)
    cat opt/sparc."$1"
    ;;
  *)
    cat opt/sparc."$1"
    ;;
esac
