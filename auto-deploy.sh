#!/usr/bin/env sh

while true; do
    inotifywait panotify.c
    pkill pidgin
    make deploy
    pidgin &
done

