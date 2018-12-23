#!/usr/bin/env bash

function exit_error {
	echo "$@"
	exit 1
}

locate stubs-64.h >/dev/null 2>&1 || exit_error "failed to locate required gun header files"
locate features.h > /dev/null 2>&1 || exit_error "no glibc development is installed"
exit 0
