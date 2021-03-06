function usage {
  cat <<-EOF
USAGE

  $0  [-dghv] [-f focus-string]

OPTIONS

  -h  print this message

  -d  run tests in a debugger (either lldb or gdb)

  -g  run tests with valgrind

  -v  run tests with verbose output

  -f  run only tests whose description contain the given string

EOF
}

function run_tests {
  local profile=
  local mode=normal
  local args=()
  local target=$1
  local cmd="out/Debug/${target}"
  shift

  while getopts "df:ghpv" option; do
    case ${option} in
      h)
        usage
        exit
        ;;
      d)
        mode=debug
        ;;
      g)
        mode=valgrind
        ;;
      p)
        profile=true
        ;;
      f)
        args+=("--only=${OPTARG}")
        ;;
      v)
        args+=("--reporter=spec")
        ;;
    esac
  done

  BUILDTYPE=Debug make $target
  args=${args:-""}

  if [[ -n $profile ]]; then
    export CPUPROFILE=/tmp/${target}-$(date '+%s').prof
  fi

  case ${mode} in
    valgrind)
      valgrind \
        --suppressions=./script/util/valgrind.supp \
        --dsymutil=yes \
        $cmd "${args[@]}" 2>&1 | \
        grep --color -E '\w+_specs?.cc:\d+|$'
      ;;

    debug)
      if which -s gdb; then
        gdb $cmd -- "${args[@]}"
      elif which -s lldb; then
        lldb $cmd -- "${args[@]}"
      else
        echo "No debugger found"
        exit 1
      fi
      ;;

    normal)
      time $cmd "${args[@]}"
      ;;
  esac

  if [[ -n $profile ]]; then
    pprof $cmd $CPUPROFILE
  fi
}
