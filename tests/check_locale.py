#!/usr/bin/env python3
"""Check every existing menu label and choice has a reviewed Scots display entry."""
import json
from pathlib import Path
import re
import subprocess
import sys

root = Path(__file__).resolve().parents[1]
terms = json.loads((root / 'assets/locale/scots.json').read_text(encoding='utf-8'))
rows = json.loads((root / 'tests/fixtures/menu-parity.json').read_text())['controls']
titles = re.findall(r'SetTitle\(XOR\("([^"]+)"\)\)', (root / 'menu.h').read_text())
visible = {row['label'] for row in rows if row['label']}
visible.update(choice for row in rows for choice in row['choices'])
visible.update(titles)
visible.update(['save configuration', 'CONTROL ROOM', 'STAY IN BALANCE', 'press key', 'none'])
missing = visible - terms.keys()
assert not missing, f'Missing Scots display entries: {sorted(missing)}'
assert all(terms[key] for key in visible), 'Empty Scots display text'
for key in ['enable', 'save', 'load', 'configuration', 'players', 'visuals', 'misc']:
    assert terms[key] != key, key
assert len(rows) == 366 and len(titles) == 9
subprocess.run([sys.executable, str(root / 'tools/generate_scots.py'), '--check'], check=True)
print(f'Scots display coverage passed: {len(visible)} existing labels/choices, {len(rows)} controls, {len(titles)} categories.')
