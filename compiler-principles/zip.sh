#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: zip <ex name>"
    exit 1
fi

EX_NAME=$1
ZIP_FILE="${EX_NAME}.zip"

# Check if the specified files and directories exist
if [ ! -f "${EX_NAME}/Makefile" ]; then
    echo "Makefile not found in ${EX_NAME} directory"
    exit 1
fi

if [ ! -f "${EX_NAME}/build.sh" ]; then
    echo "build.sh not found in ${EX_NAME} directory"
    exit 1
fi

if [ ! -d "${EX_NAME}/src" ]; then
    echo "src directory not found in ${EX_NAME} directory"
    exit 1
fi

# Create the zip file
rm "./${ZIP_FILE}"
cd "${EX_NAME}"
zip -r "../${ZIP_FILE}" "Makefile" "build.sh" "src"
cd ..

echo "Created ${ZIP_FILE}"