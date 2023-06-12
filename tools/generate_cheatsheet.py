import os
from collections import defaultdict
import glob
import json


def load_json(filename):
    with open(filename, 'r') as f:
        out = json.load(f)
    return out


class CheatSheetGenerator(object):
    def __init__(self, output_dir):
        self.out_dir = output_dir

        self.cmd2label = {
            'gnnoremap': 'GUI Normal Mode',
            'gvnoremap': 'GUI Visual Mode',
            'ennoremap': 'Editor Normal Mode',
            'evnoremap': 'Editor Visual Mode',
            'inoremap': 'Insert Mode',
            'rnoremap': 'Resident Mode',
            'command': 'Command Mode'
        }

        self.mode_tag = '{mode}'
        self.cheat_url = '{{ site.url }}/cheat_sheet/functions/'

    def make_tiersheet(self, rc_filename):
        tier, rc_info = self.load_rc(rc_filename)

        tier_dir = os.path.join(self.out_dir, tier)
        os.makedirs(tier_dir, exist_ok=True)

        out_filename = os.path.join(tier_dir, 'index.md')
        with open(out_filename, 'w') as f:
            cap_tier = tier.capitalize()

            # Set up page headers and an title
            f.write(
                '---\n' +
                'layout: page\n' +
                'show_in_menu: false\n' +
                'disable_anchors: true\n' +
                'title: {} Mappings - win-vind\n'.format(cap_tier) +
                'nav: {} Mappings\n'.format(cap_tier) +
                '---\n\n')

            for mode_label, mode_info in rc_info.items():
                f.write('## ' + mode_label + '\n\n')

                for ft_label, maps in mode_info.items():
                    if not maps:
                        continue

                    if ft_label:
                        f.write('### ' + ft_label + '\n\n')

                    # Add table headers
                    f.write('|**Trigger Commands**|**Called Commands**|\n')
                    f.write('|:---:|:---:|\n')

                    # Build reversed dicrionary to integrate lines where
                    # the called commands are the same
                    back_maps = defaultdict(set)
                    for m in maps:
                        i_cmd = self.integrate_modes(m[0])

                        # To make the dictionary key, divides them.
                        o_cmds = [
                            self.integrate_modes(x[1:])
                            for x in m[1].split('>')
                            if x
                        ]

                        back_maps[tuple(o_cmds)].add(i_cmd)

                    for o_cmds, i_cmds in back_maps.items():
                        # Join the trigger commands with an inner-code tag.
                        i_text = ', '.join([
                            '`' + x.lower() + '`' for x in i_cmds])

                        # Add each url link
                        o_text = ''.join([
                            '[\\<' + x + '\\>](' + self.cheat_url + '#' +
                            x.replace(self.mode_tag, '').lower() + ')'
                            for x in o_cmds])

                        f.write('|{}|{}|\n'.format(i_text, o_text))

                    f.write('\n')

            f.write(2 * '<br>\n')
            f.write('## See also\n')
            f.write('- [tiny](../tiny)\n')
            f.write('- [small](../small)\n')
            f.write('- [normal](../normal)\n')
            f.write('- [big](../big)\n')
            f.write('- [huge](../huge)\n')
            f.write(1 * '<br>\n')

    def integrate_modes(self, text):
        if text.endswith('noremap'):
            return self.mode_tag + 'noremap'

        if text.endswith('unmap'):
            return self.mode_tag + 'unmap'

        if text.endswith('mapclear'):
            return self.mode_tag + 'mapclear'

        if text.endswith('map'):
            return self.mode_tag + 'map'

        return text

    def load_rc(self, rc_filename):
        tag = os.path.basename(rc_filename).split('.')[0]

        with open(rc_filename, 'r') as f:
            lines = f.read().splitlines()

        output = defaultdict(lambda: defaultdict(list))
        ft_tag = ''   # This text will be <h3>
        for line in lines:
            line = line.lstrip()
            if not line:
                continue

            if line.startswith('"'):  # Parse comment lines
                # To set each <h3> text, employs @ tag in .vindrc.
                if '@' in line:
                    ft_tag = '@'.join(line.split('@')[1:]).strip()
                continue

            line_splits = line.split(' ')
            cmd = line_splits[0].strip()
            arg1 = line_splits[1].strip()
            arg2 = (' '.join(line_splits[2:])).strip()

            if cmd in self.cmd2label:
                label = self.cmd2label[cmd]
                output[label][ft_tag].append((arg1, arg2))

        return tag, output


def main():
    generator = CheatSheetGenerator('docs/cheat_sheet/defaults')

    rc_files = glob.glob('res/resources/defaults/*.vindrc')
    for rc_file in rc_files:
        generator.make_tiersheet(rc_file)


if __name__ == '__main__':
    main()
