#!/bin/sh

echo "File:" $1
if grub-file --is-x86-multiboot $1; then
    echo "Multiboot confirmed!"
else
    echo "The file is not multiboot!"
fi
