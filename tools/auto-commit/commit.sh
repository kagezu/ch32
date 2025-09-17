#!/bin/bash

cd $1
echo "Проект: $1"
git log --oneline --graph -3
echo -n "git commit -a -m: "
read com

git add ./
git commit -a -m "$com"
git push -u old-origin master
git push -u origin master
git log --oneline --graph -16
sleep 100
