#!/bin/bash

set -ex

IN_DIR=$1

if [ "x$CI_REGISTRY" = x ]; then
  CI_REGISTRY=dev.dingxiang-inc.com:4567
fi

# tag & branch have same name
if [ "A$CI_BUILD_REF_NAME" = A ]; then
  CI_BUILD_REF_NAME=latest
fi

if [ x$CI_REGISTRY != x ]; then
    echo "FROM scratch" > _Dockerfile
    echo "COPY $IN_DIR /out/" >> _Dockerfile
	docker build -t $CI_REGISTRY/stee/dockers/$CI_PROJECT_NAME:$CI_BUILD_REF_NAME -f _Dockerfile .
	rm -r _Dockerfile
	docker push $CI_REGISTRY/stee/dockers/$CI_PROJECT_NAME:$CI_BUILD_REF_NAME
	echo "Upload $IN_DIR to $CI_REGISTRY/stee/$CI_PROJECT_NAME:$CI_BUILD_REF_NAME. Done!"
else
	echo "It\`t not in CI envirenment"
fi