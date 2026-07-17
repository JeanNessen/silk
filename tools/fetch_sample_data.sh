#!/usr/bin/env bash
#
# fetch_sample_data.sh — download + verify + decompress one NASDAQ ITCH 5.0 day.
#
# Source: Nasdaq's public archive at https://emi.nasdaq.com/ITCH/Nasdaq ITCH/
# Files are named MMDDYYYY.NASDAQ_ITCH50.gz with a matching .gz.md5sum.
#
# Usage:   tools/fetch_sample_data.sh [DATE] [OUTDIR]
#   DATE    MMDDYYYY, default 07302019 (~3.7 GB gz — the smallest full day available)
#   OUTDIR  destination, default ./data
#
# If a date 404s, some older .gz files have been purged (only the .md5sum remains).
# Open the base URL in a browser and pick a date that still lists a .gz.
#
set -euo pipefail

BASE_URL="https://emi.nasdaq.com/ITCH/Nasdaq ITCH"
DATE="${1:-07302019}"
OUTDIR="${2:-data}"

FILE="${DATE}.NASDAQ_ITCH50"     # decompressed name
GZ="${FILE}.gz"

mkdir -p "$OUTDIR"
cd "$OUTDIR"

if [[ -f "$FILE" ]]; then
  echo "✓ $FILE already present — nothing to do."
  exit 0
fi

# URL-encode the space in the "Nasdaq ITCH" path segment.
url_gz="${BASE_URL// /%20}/${GZ}"
url_md5="${url_gz}.md5sum"

echo "→ Downloading ${GZ} (multi-GB; -C - resumes if interrupted)…"
curl -fL --retry 3 -C - -o "$GZ" "$url_gz"

echo "→ Fetching checksum…"
if curl -fL --retry 3 -o "${GZ}.md5sum" "$url_md5"; then
  expected="$(grep -oE '[0-9a-fA-F]{32}' "${GZ}.md5sum" | head -n1)"
  actual="$(md5sum "$GZ" | awk '{print $1}')"
  if [[ "$expected" == "$actual" ]]; then
    echo "✓ checksum OK ($actual)"
  else
    echo "✗ checksum MISMATCH: expected $expected, got $actual" >&2
    exit 1
  fi
else
  echo "⚠ no .md5sum published for this date — skipping verification." >&2
fi

echo "→ Decompressing (keeps the .gz so re-runs are free; delete it to reclaim disk)…"
gunzip -kf "$GZ"

echo "✓ Ready: ${OUTDIR}/${FILE}"
echo "  Next: build tools/truncate_itch and carve a small dev slice from this file."
