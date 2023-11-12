#!/bin/bash

set -xe

cc main.c -o hex -pedantic -std=c99 -lm
