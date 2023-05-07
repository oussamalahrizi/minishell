#!/bin/bash

# Get all environment variable names
env_vars=$(printenv | cut -f1 -d=)

# Randomly shuffle the variable names
shuffled_vars=$(echo "$env_vars" | tr ' ' '\n' | shuf | tr '\n' ' ')

# Loop through the shuffled variable names
for var in $shuffled_vars; do
  # Randomly choose whether to put the variable name in double quotes
  if [ $((RANDOM%2)) -eq 0 ]; then
    # Put the variable name in double quotes
    printf '"$%s" ' "$var"
  else
    # Don't put the variable name in double quotes
    printf '$%s ' "$var"
  fi
done

# Output a newline at the end
echo

