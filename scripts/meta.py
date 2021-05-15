import argparse
import re
from collections import namedtuple

from meta_impl import *


def print_structure(writer: Writer, s):
    writer.writeln('/*#')
    for k, v in s.raw.items():
        if type(v) is list:
            for vv in v:
                writer.writeln(f'{k}[]: {vv}')
        else:
            writer.writeln(f'{k}: {v}')
    writer.writeln('#*/')


class Structure:
    def __init__(self, raw, everything_name, array_type, name, structure_type, members):
        self.raw = raw
        self.everything_name = everything_name
        self.array_type = array_type
        self.name = name
        self.members = members
        self.signature_member = Member(
            type=f'Signature<{name.upper()}_COMPONENT>',
            name='signature',
            enum=None,
            simple=None,
        )
        self.structure_type = structure_type
        self.index_type = 'size_t'


class Code:
    def __init__(self, code):
        self.code = code


def make_struct(structure: Structure):
    everything_struct = Struct(name=structure.everything_name)
    object_struct = Struct(name=structure.name)
    object_proxy = Struct(name=f'{structure.name}Proxy') if structure.structure_type == 'sSoA' else None
    enum = Enum(name=f'{structure.name.upper()}_COMPONENT')

    for member in structure.members + [structure.signature_member]:
        enum.vals.append(member[2])

        object_struct.member_variables.append(VariableDeclaration(
            name=member.name,
            mtype=member.type,
            val=''
        ))

    if structure.array_type[0] == 'Vector':
        def data_type(s):
            return f'std::vector<{s}>'

        val = structure.array_type[1]
    else:
        exit(f'Invalid data type: {structure.array_type[0]}')

    if structure.structure_type == 'sSoA':
        everything_struct.member_variables.append(VariableDeclaration(
            name=f'indirectionMap',
            mtype=f'std::vector<{structure.name}Proxy>',
            val=val
        ))

        if structure.raw['indirection_type'] == 'pointers':
            def get_indirection_type(member: Member):
                return f'{member.type}*'

            def get_indirection_access(member: Member):
                return f'return *{member.name}_;'

        elif structure.raw['indirection_type'] == 'integers':
            object_proxy.member_variables.append(VariableDeclaration(
                name='proxy',
                mtype=f'{structure.everything_name}*'
            ))

            def get_indirection_type(member: Member):
                return f'{structure.index_type}'

            def get_indirection_access(member: Member):
                return f'return proxy->{member.name}s[{member.name}_];'

        for member in structure.members:
            object_proxy.member_variables.append(VariableDeclaration(
                name=f'{member.name}_',
                mtype=get_indirection_type(member),
            ))

            object_proxy.member_functions.append(MemberFunction(
                name=member.name,
                return_type=f'{member.type}&',
                implementation=get_indirection_access(member)
            ))

    if structure.structure_type == 'SoA' or structure.structure_type == 'sSoA':
        for member in structure.members + [structure.signature_member]:
            everything_struct.member_variables.append(VariableDeclaration(
                name=f'{member.name}s',
                mtype=data_type(member.type),
                val=val
            ))
    elif structure.structure_type == 'AoS':
        everything_struct.member_variables.append(VariableDeclaration(
            name='data',
            mtype=data_type(structure.name),
            val=val
        ))
    else:
        exit(f'Invalid structure type: {structure.structure_type}')

    if structure.structure_type == 'SoA':
        def get_implementation(member):
            if member.simple is not None:
                sig_assert = f'assert(signature(i).test({enum.name}::{member.name.upper()}));\n'
            else:
                sig_assert = ''
            return f'assert(i < {structure.array_type[1]});\n{sig_assert}return {member.name}s[i];'
    elif structure.structure_type == 'AoS':
        def get_implementation(member):
            if member.simple is not None:
                sig_assert = f'assert(signature(i).test({enum.name}::{member.name.upper()}));\n'
            else:
                sig_assert = ''
            return f'assert(i < {structure.array_type[1]});\n{sig_assert}return data[i].{member};'
    elif structure.structure_type == 'sSoA':
        if structure.raw['indirection_type'] == 'integers':
            def get_implementation(member):
                return f'return {member.name}s[indirectionMap[i].{member.name}_];'
        else:
            def get_implementation(member):
                return f'return indirectionMap[i].{member.name}();'
    else:
        exit(f'Invalid structure type: {structure.structure_type}')

    for member in structure.members:
        everything_struct.member_functions.append(MemberFunction(
            name=member.name,
            return_type=f'{member.type}&',
            implementation=get_implementation(member),
            arguments=[VariableDeclaration(name='i', mtype='SizeAlias')]
        ))

    everything_struct.member_functions.append(MemberFunction(
        name=structure.signature_member.name,
        return_type=f'{structure.signature_member.type}&',
        implementation=f'return {structure.signature_member.name}s[i];',
        arguments=[VariableDeclaration(name='i', mtype='SizeAlias')]
    ))

    return List(everything_struct, object_struct, enum, object_proxy)


class ForwardDeclaration:
    pass


class Declaration:
    pass


class Implementation:
    pass


Member = namedtuple('Member', ['type', 'name', 'enum', 'simple'])


def parse_structure(data):
    processed_data = dict()

    for line in data.splitlines()[1:-1]:
        print(line)
        m = re.match(r'(\w+)(\[\])?:\s*(.+)$', line)
        key = m.group(1)
        value = m.group(3)
        if m.group(2) == '[]':
            if key in processed_data:
                processed_data[key].append(value)
            else:
                processed_data[key] = [value]
        else:
            processed_data[key] = value

    everything_name = processed_data['everything_name']
    structure_type = processed_data['structure']
    array_type = (processed_data['type'], int(processed_data['size']))
    name = processed_data['object_name']

    members = []
    for member in processed_data['component']:
        member_split = member.split()
        if len(member_split) == 1:
            members.append(Member(
                type=member,
                name=member.lower(),
                enum=member.upper(),
                simple=True
            ))
        else:
            members.append(Member(
                type=member_split[0],
                name=member_split[1],
                enum=member_split[1].upper(),
                simple=False
            ))

    return Structure(processed_data, everything_name, array_type, name, structure_type, members)


def main():
    parser = argparse.ArgumentParser(description='meta classes please')

    parser.add_argument('--file', '-f', type=str, default='Test.h', dest='file_name')
    parser.add_argument('--dry-run', type=bool, default=False)

    args = parser.parse_args()
    file_name = args.file_name
    dry_run = args.dry_run

    with open(file_name, 'r') as file:
        blocks = re.split(
            r'(\/\*\#.*?\#\*\/$)|(\/\/\# \w+.*?\/\/\# end)',
            file.read(),
            flags=re.DOTALL + re.MULTILINE
        )

        result = []

        forward = False
        declaration = False
        implementation = False

        structures = List()

        for r in blocks:
            if r is None:
                continue
            elif r.startswith('/*#'):
                structure = parse_structure(r)
                structures.merge(make_struct(structure))
                result.append(structure)
            elif r.startswith('//#'):
                block_type = re.match(r'(?:\/\/\#) (\w+)', r).group(1)
                print(block_type)
                if block_type == 'forward':
                    forward = True
                    result.append(ForwardDeclaration())
                elif block_type == 'declaration':
                    if not forward:
                        forward = True
                        result.append(ForwardDeclaration())
                    declaration = True
                    result.append(Declaration())
                elif block_type == 'implementation':
                    if not declaration:
                        declaration = True
                        result.append(Declaration())
                    implementation = True
                    result.append(Implementation())
            else:
                stripped = r.strip()
                if stripped != '':
                    result.append(Code(stripped))

        if not forward:
            result.append(ForwardDeclaration())
        if not declaration:
            result.append(Declaration())
        if not implementation:
            result.append(Implementation())

    if dry_run:
        writer: Writer = Writer(lambda x: print(x, end=''))
    else:
        file = open(file_name, 'w')
        writer: Writer = Writer(lambda x: file.write(x))

    for r in result:
        if type(r) is Structure:
            print_structure(writer, r)
            writer.writeln('')
        elif type(r) is ForwardDeclaration:
            writer.writeln('//# forward')
            writer.writeln(f'using SizeAlias = {structure.index_type};')
            structures.forward_declaration(writer)
            writer.writeln('//# end')
        elif type(r) is Declaration:
            writer.writeln('//# declaration')
            structures.declaration(writer)
            writer.writeln('//# end')
        elif type(r) is Implementation:
            writer.writeln('//# implementation')
            structures.implementation(writer)
            writer.writeln('//# end')
        elif type(r) is Code:
            writer.writeln(r.code)
        else:
            pass

    if result is None:
        print('Invalid file, no structure found')
        return 0

    if not dry_run:
        file.close()


if __name__ == '__main__':
    main()
