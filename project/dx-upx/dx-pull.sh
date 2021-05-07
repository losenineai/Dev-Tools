#!/bin/bash

set -e

PNAME="$1"
OUT_DIR="$2"
PVER="$(cat dependent.txt | grep $PNAME | cut -f2 -d:)"

if [ "x$CI_REGISTRY" = x ]; then
  CI_REGISTRY=dev.dingxiang-inc.com:4567
fi

fetch_files_from_docker() {
  url=$1
  dest=$2
  docker pull $url
  docker rm -f fetch-tmp || true
  docker create --name fetch-tmp $url ignore
  docker cp fetch-tmp:/out/ $dest
}

[ -e $OUT_DIR ] && mkdir -p $OUT_DIR || true
fetch_files_from_docker $CI_REGISTRY/stee/dockers/$PNAME:$PVER $OUT_DIR/

echo "Download $CI_REGISTRY/stee/dockers/$PNAME:$PVER to $OUT_DIR. Done!"