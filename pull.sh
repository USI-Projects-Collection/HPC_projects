
#!/bin/bash
# Script per updatare la repo remota

echo "Fetching remoto..."
git fetch origin

echo "Reset forzato alla versione remota..."
git reset --hard origin/main

echo "Pulizia dei file non tracciati..."
git clean -fd