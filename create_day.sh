#!/bin/bash

# Check if a number is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <day-number>"
    exit 1
fi

DAY_NUM="$1"
OLD_NAME="day1"
NEW_NAME="day${DAY_NUM}"

# Copy directories
cp -r "$OLD_NAME" "$NEW_NAME"
cp -r "inputs/$OLD_NAME" "inputs/$NEW_NAME"

# Rename files containing "day1" in the new directories
find "$NEW_NAME" "inputs/$NEW_NAME" -type f -name "*$OLD_NAME*" | while read -r FILE; do
    NEW_FILE=$(echo "$FILE" | sed "s/$OLD_NAME/$NEW_NAME/g")
    mv "$FILE" "$NEW_FILE"
done

# Replace "day1" inside files
find "$NEW_NAME" "inputs/$NEW_NAME" -type f | while read -r FILE; do
    sed -i "s/$OLD_NAME/$NEW_NAME/g" "$FILE"
done

echo "Copied and updated $OLD_NAME → $NEW_NAME"
