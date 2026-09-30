#!/usr/bin/env python3
"""Build the checked-in, display-only Scots lookup from its reviewed term list."""
import argparse
import json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
terms = json.loads((root / 'assets/locale/scots.json').read_text(encoding='utf-8'))
lines = ['#pragma once', '', '#include <string>', '#include <unordered_map>', '',
         '// Human-facing menu text only. Saved keys, item indices and callbacks stay in English.',
         'namespace scots {',
         'inline const std::string& display(const std::string& original) {',
         '    static const std::unordered_map<std::string, std::string> terms = {']
for source, translated in sorted(terms.items()):
    lines.append('        {' + json.dumps(source, ensure_ascii=True) + ', ' + json.dumps(translated, ensure_ascii=True) + '},')
lines += ['    };', '    const auto found = terms.find(original);',
          '    return found == terms.end() ? original : found->second;', '}', '}']
output = '\n'.join(lines) + '\n'
parser = argparse.ArgumentParser()
parser.add_argument('--check', action='store_true')
args = parser.parse_args()
if args.check:
    assert (root / 'scots_locale.h').read_text() == output, 'Regenerate scots_locale.h'
else:
    (root / 'scots_locale.h').write_text(output)
