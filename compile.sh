#!/bin/bash

if [ $# -ne 1 ]; then
    project='rover'

    echo "Compiling default project $project"
else
    project=$1
    echo "Compiling project $project..."
fi

echo ''

arduino-cli compile -b arduino:avr:uno $project