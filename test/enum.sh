#!/bin/sh

LD_PRELOAD=obj/.libs/libsdate.so perl test/enum
