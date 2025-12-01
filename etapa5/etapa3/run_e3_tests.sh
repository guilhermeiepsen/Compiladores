#!/usr/bin/env bash
set -u

# cd to script dir
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

EXEC="${EXEC:-./ast_parser}"
TEST_DIR="${TEST_DIR:-E3}"
OUT_DIR="$TEST_DIR"

# Build if needed
if [[ ! -x "$EXEC" ]]; then
  echo "[i] Building parser..."
  make -s
fi

# Ensure test dir exists
if [[ ! -d "$TEST_DIR" ]]; then
  echo "[!] Test directory '$TEST_DIR' not found" >&2
  exit 1
fi

has_dot() { command -v dot >/dev/null 2>&1; }

canonicalize() {
  # canonicalize <in.dot> <out.canon>
  local in="$1" out="$2"
  if has_dot; then
    # Use graphviz canonical output (order-insensitive)
    dot -Tcanon "$in" > "$out"
  else
    # Fallback: normalize node ids and sort lines
    awk '
    function mapid(x){ if(!(x in m)){ m[x] = sprintf("N%05d", ++n) } return m[x] }
    {
      line = $0
      if (match(line, /^[[:space:]]*([0-9]+)[[:space:]]+\[/, a)) {
        id = a[1]
        sub(/^[[:space:]]*[0-9]+/, mapid(id), line)
        print line; next
      }
      if (match(line, /^[[:space:]]*([0-9]+)[[:space:]]*->[[:space:]]*([0-9]+)/, a)) {
        u = a[1]; v = a[2]
        sub(/^[[:space:]]*[0-9]+/, mapid(u), line)
        sub(/->[[:space:]]*[0-9]+/, "-> " mapid(v), line)
        print line; next
      }
      print line
    }' "$in" | LC_ALL=C sort > "$out"
  fi
}

# Enable nullglob to avoid literal patterns when nothing matches
shopt -s nullglob
inputs=(
  "$TEST_DIR"/z[0-9][0-9]
  "$TEST_DIR"/zasl[0-9][0-9][0-9]
)
shopt -u nullglob

if (( ${#inputs[@]} == 0 )); then
  echo "[!] No inputs matching zNN or zaslNNN in $TEST_DIR" >&2
  exit 1
fi

pass=0
fail=0
skip=0

declare -a results

echo "[i] Running ${#inputs[@]} test(s) from $TEST_DIR"

for inpath in "${inputs[@]}"; do
  base="$(basename "$inpath")"
  input="$inpath"
  ref_dot="$OUT_DIR/$base.ref.dot"
  out_dot="$OUT_DIR/$base.dot"
  diff_file="$OUT_DIR/$base.diff"
  ref_canon="$OUT_DIR/$base.ref.canon"
  out_canon="$OUT_DIR/$base.canon"

  # Check reference exists
  if [[ ! -f "$ref_dot" ]]; then
    echo "[skip] $base: missing reference '$ref_dot'"
    results+=("$base SKIPPED ref-missing")
    ((skip++))
    continue
  fi

  # Run parser
  if ! "$EXEC" < "$input" > "$out_dot"; then
    echo "[fail] $base: program execution failed"
    results+=("$base FAIL exec-error")
    ((fail++))
    continue
  fi

  # Canonicalize both graphs (order-insensitive)
  canonicalize "$ref_dot" "$ref_canon"
  canonicalize "$out_dot" "$out_canon"

  if diff -u "$ref_canon" "$out_canon" > "$diff_file" 2>/dev/null; then
    echo "[ok]   $base"
    results+=("$base OK")
    ((pass++))
    rm -f "$diff_file"
  else
    echo "[fail] $base (see $diff_file)"
    results+=("$base FAIL diff")
    ((fail++))
  fi

done

echo
echo "Summary: $pass passed, $fail failed, $skip skipped"
if (( fail > 0 )); then
  echo "Failures:"
  for r in "${results[@]}"; do
    case "$r" in *"FAIL"*) echo "  - $r" ;; esac
  done
  exit 1
fi
exit 0
