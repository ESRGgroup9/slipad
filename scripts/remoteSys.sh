#!/bin/sh

# run website PHP server
php -S localhost:8000 -t ~/slipad/remoteSystem/website/ &

# run remote system
~/slipad/remoteSystem/bin/remoteSystem.elf 5000