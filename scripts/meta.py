import argparse
import re


def print_structure(s):
    name = s.name
    everything_name = s.everything_name
    array_type = s.array_type
    members = s.members
    structure_type = s.structure_type

    res = []
    output_line = res.append

    output_line(f'/*#')
    output_line(f'{everything_name} {structure_type}')
    output_line(f'{array_type[0]} {array_type[1]}')
    output_line(f'{name}')
    for member in members:
        if member[2]:
            output_line(f'{member[0]} {member[1]}')
        else:
            output_line(f'{member[0]}')

    output_line(f'#*/')

    return res


def print_generated_structure(s):
    name = s.name
    everything_name = s.everything_name
    array_type = s.array_type
    members = s.members
    structure_type = s.structure_type
    component_enum = f'{name.upper()}_COMPONENT'

    def enum_name(n):
        return n.upper()

    res = []
    output_line = res.append

    output_line(f'//# Start {name}')

    # enum
    output_line(f'enum {component_enum}')
    output_line('{')
    for member in members:
        output_line(f'\t{enum_name(member[1])},')
    output_line('\tMAX')
    output_line('};\n')

    # object
    output_line(f'struct {name}')
    output_line('{')
    output_line(f'\tSignature<{component_enum}> signature;')

    for member in members:
        output_line(f'\t{member[0]} {member[1]};')

    output_line('};\n')

    # proxy
    output_line('struct Everything;')
    output_line(f'struct {name}Proxy')
    output_line('{')
    output_line('\tsize_t i;')
    output_line(f'\t{everything_name}& proxy;')
    for member in members:
        output_line(f'\tinline {member[0]}& {member[1]}();')
    output_line('};')

    # everything
    output_line(f'struct {everything_name}')
    output_line('{')

    output_line('\tsize_t count;')
    if structure_type == 'AoS':
        if array_type[0] == 'Array':
            output_line(f'\tstd::array<{name}, {array_type[1]}> data;\n')
    elif structure_type == 'SoA':
        if array_type[0] == 'Array':
            output_line(f'\tstd::array<Signature<{component_enum}>, {array_type[1]}> signatures;')
            for member in members:
                output_line(f'\tstd::array<{member[0]}, {array_type[1]}> {member[1]}s;')

    output_line(f'\tinline void add({name}&& obj) {{')
    output_line(f'\t\tassert(count < {array_type[1]});')
    if structure_type == 'AoS':
        output_line(f'\t\tif constexpr (std::is_trivially_copyable_v<{name}>) {{')
        output_line(f'\t\t\tdata[count] = obj;')
        output_line(f'\t\t}}')
        output_line(f'\t\telse {{')
        output_line(f'\t\t\tdata[count] = std::move(obj);')
        output_line(f'\t\t}}')
    else:
        for member in members:
            output_line(f'\t\tif constexpr (std::is_trivially_copyable_v<{member[0]}>) {{')
            output_line(f'\t\t\t{member[1]}(count) = obj.{member[1]};')
            output_line(f'\t\t}}')
            output_line(f'\t\telse {{')
            output_line(f'\t\t\t{member[1]}(count) = std::move(obj.{member[1]});')
            output_line(f'\t\t}}')

    output_line(f'\t\tcount++;')
    output_line('\t};')

    output_line(f'\tinline void add({name} const& obj) {{')
    output_line(f'\t\tassert(count < {array_type[1]});')
    if structure_type == 'AoS':
        output_line(f'\t\tdata[count] = obj;')
    else:
        for member in members:
            output_line(f'\t\t{member[1]}(count) = obj.{member[1]};')
    output_line(f'\t\tcount++;')
    output_line('\t};')

    output_line('\tinline void remove(size_t i) {')
    output_line(f'\t\tassert(i < count);')
    if structure_type == 'AoS':
        output_line(f'\t\tdata[i] = data[count - 1];')
    else:
        for member in members:
            output_line(f'\t\t{member[1]}(i) = {member[1]}(count - 1);')
    output_line('\t};')

    output_line(f'\tinline {name}Proxy get(size_t i) {{')
    output_line('\t\treturn { i, *this };')
    output_line('\t};')

    output_line(f'\tinline Signature<{component_enum}>& signature(size_t i) {{')
    if structure_type == 'AoS':
        output_line(f'\t\treturn data[i].signature;')
    else:
        output_line(f'\t\treturn signatures[i];')
    output_line(f'\t}};')

    for member in members:
        output_line(f'\tinline {member[0]}& {member[1]}(size_t i) {{')
        output_line(f'\t\tassert(signature(i).test({component_enum}::{enum_name(member[1])}));')
        if structure_type == 'AoS':
            output_line(f'\t\treturn data[i].{member[1]};')
        else:
            output_line(f'\t\treturn {member[1]}s[i];')
        output_line(f'\t}};')
    output_line(f'}};\n')

    for member in members:
        output_line(f'inline {member[0]}& {name}Proxy::{member[1]}() {{')
        output_line(f'\treturn proxy.{member[1]}(i);')
        output_line('};')

    output_line(f'//# End {name}')

    return res


class Structure:
    def __init__(self, everything_name, array_type, name, structure_type, members):
        self.everything_name = everything_name
        self.array_type = array_type
        self.name = name
        self.members = members
        self.structure_type = structure_type


def parse_structure(data):
    structure = data.splitlines()[1:-1]
    everything_name, structure_type = structure[0].split()
    array_type = structure[1].split()
    name = structure[2]
    array_type[1] = int(array_type[1])

    members = []
    for member in structure[3:]:
        member_split = member.split()
        if len(member_split) == 1:
            members.append((member, member.lower(), False))
        else:
            members.append((member_split[0], member_split[1], True))

    return Structure(everything_name, array_type, name, structure_type, members)


def main():
    parser = argparse.ArgumentParser(description='meta classes please')

    parser.add_argument('--file', '-f', type=str, default='Test.h', dest='file_name')
    parser.add_argument('--dry-run', type=bool, default=False)

    args = parser.parse_args()
    file_name = args.file_name
    dry_run = args.dry_run

    with open(file_name, 'r') as file:
        blocks = re.split(r'(\/\*\#.*?\#\*\/$)|(\/\/\# Start \w+.*?\/\/\# End \w+$)', file.read(),
                          flags=re.DOTALL + re.MULTILINE)

        for block in blocks:
            print(block)
            print('----------------')

        result = []

        for r in blocks:
            if r is None:
                continue
            elif r.startswith('/*#'):
                structure = parse_structure(r)
                result.append(structure)
            elif r.startswith('//#'):
                continue
            else:
                stripped = r.strip()
                if stripped != '':
                    result.append(stripped)

        result_string_blocks = []

        for r in result:
            if type(r) is Structure:
                result_string_blocks.append('\n'.join(print_structure(r)))
                result_string_blocks.append('')
                result_string_blocks.append('\n'.join(print_generated_structure(r)))
                result_string_blocks.append('')
            else:
                result_string_blocks.append(r)
                result_string_blocks.append('')

        if result is None:
            print('Invalid file, no structure found')
            return 0

    if dry_run:
        out = print
    else:
        file = open(file_name, 'w')

        def out(s):
            file.write(s)

    print(len(result_string_blocks))

    out('\n'.join(result_string_blocks))

    if not dry_run:
        file.close()


if __name__ == '__main__':
    main()
