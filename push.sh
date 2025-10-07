#!/bin/bash
# Script per fare add, commit e push veloce con messaggio "update"

echo "Aggiungo modifiche..."
git add .

echo "Commit con messaggio 'update'..."
git commit -m "update"

echo "Pusho su remoto..."
git push
