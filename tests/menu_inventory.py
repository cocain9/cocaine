#!/usr/bin/env python3
"""Record/check the existing menu contract independently of its presentation.

The baseline includes every registration, setup argument (choices/defaults/ranges),
callback and visibility predicate. Never refresh it to accommodate a UI regression.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
BASELINE = ROOT / 'tests' / 'fixtures' / 'menu-parity.json'


def without_comments(source):
    token = r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*[\s\S]*?\*/'
    return re.sub(token, lambda m: '' if m[0].startswith(('//', '/*')) else m[0], source)


def arguments(source, opening):
    depth, quote, escaped = 1, None, False
    for i in range(opening + 1, len(source)):
        char = source[i]
        if quote:
            if escaped:
                escaped = False
            elif char == '\\':
                escaped = True
            elif char == quote:
                quote = None
        elif char in ('"', "'"):
            quote = char
        elif char == '(':
            depth += 1
        elif char == ')':
            depth -= 1
            if depth == 0:
                return source[opening + 1:i]
    raise AssertionError('Unbalanced call in menu source')


def compact(source):
    # Preserve strings exactly; normalize only whitespace between tokens.
    return ''.join(re.findall(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|\S', source))


def inventory():
    source = without_comments((ROOT / 'menu.h').read_text(encoding='utf-8'))
    records = []
    for match in re.finditer(r'class (\w+) : public Tab \{([\s\S]*?)(?=\nclass |\Z)', source):
        cls, body = match.groups()
        category = re.search(r'SetTitle\(XOR\("([^"]+)"\)', body)[1]
        types = dict((m[2], m[1]) for m in re.finditer(
            r'\b(Checkbox|Slider|Dropdown|MultiDropdown|Keybind|Colorpicker|Edit|Button)\s+(\w+)\s*;', body))
        for registration in re.finditer(r'RegisterElement\(\s*&([\w]+)(?:\s*,\s*(\d+))?\s*\)', body):
            member, column = registration.groups()
            setup = re.search(r'\b' + re.escape(member) + r'\.setup\s*\(', body)
            assert setup, (cls, member)
            args = arguments(body, setup.end() - 1)
            strings = re.findall(r'"((?:\\.|[^"\\])*)"', args)
            calls = []
            for call in re.finditer(r'\b' + re.escape(member) + r'\.(\w+)\s*\(', body):
                if call[1] != 'setup':
                    calls.append(call[1] + '(' + compact(arguments(body, call.end() - 1)) + ')')
            records.append({
                'key': cls + '.' + member, 'category': category, 'member': member,
                'type': types[member], 'label': strings[0],
                'config_id': strings[1] if types[member] != 'Button' else None,
                'column': int(column or 0), 'setup': compact(args), 'calls': calls,
                'choices': strings[2:] if types[member] in ('Dropdown', 'MultiDropdown') else [],
            })
    assert len({r['key'] for r in records}) == len(records)
    return records


def checklist(records):
    terms = json.loads((ROOT / 'assets/locale/scots.json').read_text(encoding='utf-8'))
    lines = ['# Ticklist o ilka feature', '',
             'Baseline: aa 366 controls registert in `menu.h` afore the spatial-layout restyle. '
             'Ilka tick maks siccar the source control an its settin contract are still there; '
             'it disna claim post-migration runtime checks.', '',
             'Conditional controls bide conditional. Their original visibility predicates are listit '
             'sae ilka path can be checkit. Unlabelt controls are kenned by their settin key. '
             'Dropdown an multi-pick choices are includit.', '']
    for category in dict.fromkeys(r['category'] for r in records):
        rows = [r for r in records if r['category'] == category]
        lines += [f'## {terms.get(category, category)} ({len(rows)})', '', '| Here | Option / member | Kind | Current place | Existing conditions / picks |', '|---|---|---|---|---|']
        for row in rows:
            label = terms.get(row['label'], row['label']) if row['label'] else '(nae label: ' + row['config_id'] + ')'
            conditions = [c for c in row['calls'] if c.startswith('AddShowCallback')]
            detail = '; '.join(conditions) or 'Ayeweys ready'
            if row['choices']:
                detail += '; picks: ' + ', '.join(terms.get(choice, choice) for choice in row['choices'])
            lines.append(f"| ✓ | {label} · `{row['member']}` | {row['type']} | {terms.get(category, category)} / col {row['column'] + 1} | {detail.replace('|', '&#124;')} |")
        lines.append('')
    return '\n'.join(lines) + '\n'


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--capture', action='store_true', help='One-time baseline capture; refuses to overwrite')
    args = parser.parse_args()
    current = inventory()
    if args.capture:
        BASELINE.parent.mkdir(parents=True, exist_ok=True)
        with BASELINE.open('x') as stream:
            json.dump({'source_sha256': hashlib.sha256((ROOT / 'menu.h').read_bytes()).hexdigest(), 'controls': current}, stream, indent=2)
            stream.write('\n')
        (ROOT / 'docs' / 'FEATURE_PARITY.md').write_text(checklist(current), encoding='utf-8')
    else:
        baseline = json.loads(BASELINE.read_text(encoding='utf-8'))['controls']
        assert current == baseline, 'Menu contract changed: review controls, setup values, callbacks and visibility conditions'
        assert (ROOT / 'docs' / 'FEATURE_PARITY.md').read_text(encoding='utf-8') == checklist(current), 'Regenerate the Scots parity ticklist'
    print(f'Menu parity: {len(current)} controls across {len(set(r["category"] for r in current))} categories; all registrations and contracts preserved.')
