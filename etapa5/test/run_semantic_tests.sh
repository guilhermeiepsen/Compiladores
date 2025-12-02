#!/usr/bin/env bash

set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
EXEC="$ROOT_DIR/etapa4"
TEST_ROOT="$ROOT_DIR/test/Testes"
LOG_DIR="$ROOT_DIR/test/logs"

mkdir -p "$LOG_DIR"

expected_code_for_dir() {
  case "$1" in
    ERR_UNDECLARED) echo 10 ;;
    ERR_DECLARED) echo 11 ;;
    ERR_VARIABLE) echo 20 ;;
    ERR_FUNCTION) echo 21 ;;
    ERR_WRONG_TYPE) echo 30 ;;
    ERR_MISSING_ARGS) echo 40 ;;
    ERR_EXCESS_ARGS) echo 41 ;;
    ERR_WRONG_TYPE_ARGS) echo 42 ;;
    sem_erro) echo 0 ;;
    *) echo -1 ;;
  esac
}

build() {
  ( cd "$ROOT_DIR" && make -s clean && make -s ) || {
    echo "Build failed" >&2
    exit 1
  }
}

run_one_test() {
  local dir_name="$1"; shift
  local test_file="$1"; shift

  local base="${dir_name}_$(basename "$test_file")"
  local dot_out="$LOG_DIR/${base%.txt}.dot"
  local err_out="$LOG_DIR/${base%.txt}.err"

  set +e
  "$EXEC" < "$test_file" > "$dot_out" 2> "$err_out"
  local status=$?
  set -e

  echo "$status"
}

main() {
  build

  local total=0
  local passed=0
  local failed=0

  if [[ ! -x "$EXEC" ]]; then
    echo "Executable not found: $EXEC" >&2
    exit 1
  fi

  if [[ ! -d "$TEST_ROOT" ]]; then
    echo "Tests directory not found: $TEST_ROOT" >&2
    exit 1
  fi

  for dir in "$TEST_ROOT"/*/; do
    [[ -d "$dir" ]] || continue
    dir=${dir%/}
    dir_name=$(basename "$dir")
    expected=$(expected_code_for_dir "$dir_name")
    if [[ "$expected" -lt 0 ]]; then
      echo "[WARN] Skipping unknown suite: $dir_name" >&2
      continue
    fi

    shopt -s nullglob
    tests=("$dir"/*.txt)
    if (( ${#tests[@]} == 0 )); then
      echo "[WARN] No .txt tests in $dir_name" >&2
      continue
    fi

    echo "== Running suite: $dir_name (expected exit $expected) =="
    for test in "${tests[@]}"; do
      ((total++))
      status=$(run_one_test "$dir_name" "$test")
      if [[ "$status" == "$expected" ]]; then
        echo "[PASS] $dir_name/$(basename "$test") -> $status"
        ((passed++))
      else
        echo "[FAIL] $dir_name/$(basename "$test"): got $status, expected $expected"
        ((failed++))
      fi
    done
  done

  printf "\nSummary: %d passed, %d failed, %d total\n" "$passed" "$failed" "$total"
  [[ $failed -eq 0 ]]
}

main "$@"


