#!/bin/bash

if [ $# -ne 1 ]; then
    project='rover'

    echo "Uploading default project $project"
else
    project=$1
    echo "Uploading project $project..."
fi

echo ''

arduino-cli upload -p /dev/ttyUSB0 -b arduino:avr:uno rover