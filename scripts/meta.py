import argparse
import re
from collections import namedtuple

from meta_impl import *


def print_structure(writer: Writer, s):
    writer.writeln('/*#')
    writer.writeln(f'{s.everything_name} {s.structure_type}')
    writer.writeln(f'{s.array_type[0]} {s.array_type[1]}')
    writer.writeln(f'{s.name}')
    for member in s.members:
        if member.simple is None:
            continue
        elif member.simple:
            writer.writeln(f'{member[0]}')
        else:
            writer.writeln(f'{member[0]} {member[1]}')
    writer.writeln('#*/')


class Structure:
    def __init__(self, everything_name, array_type, name, structure_type, members):
        self.everything_name = everything_name
        self.array_type = array_type
        self.name = name
        self.members = members
        self.structure_type = structure_type


class Code:
    def __init__(self, code):
        self.code = code


def make_struct(structure: Structure):
    everything_struct = Struct(name=structure.everything_name)
    object_struct = Struct(name=structure.name)
    enum = Enum(name=f'{structure.name.upper()}_COMPONENT')

    for member in structure.members:
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

    if structure.structure_type == 'SoA':
        for member in structure.members:
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
        def get_implementation(s):
            if member.simple is not None:
                sig_assert = f'assert(signature(i).test({enum.name}::{member.name.upper()}));\n'
            else:
                sig_assert = ''
            return f'assert(i < {structure.array_type[1]});\n{sig_assert}return data[i].{s};'

    for member in structure.members:
        everything_struct.member_functions.append(MemberFunction(
            name=member.name,
            return_type=f'{member.type}&',
            implementation=get_implementation(member),
            arguments=[VariableDeclaration(name='i', mtype='size_t')]
        ))

    return List(everything_struct, object_struct, enum)


class ForwardDeclaration:
    pass


class Declaration:
    pass


class Implementation:
    pass


Member = namedtuple('Member', ['type', 'name', 'enum', 'simple'])


def parse_structure(data):
    structure = data.splitlines()[1:-1]
    everything_name, structure_type = structure[0].split()
    array_type = structure[1].split()
    name = structure[2]
    array_type[1] = int(array_type[1])

    members = []
    members.append(Member(
        type=f'Signature<{name.upper()}_COMPONENT>',
        name='signature',
        enum=None,
        simple=None,
    ))
    for member in structure[3:]:
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

    return Structure(everything_name, array_type, name, structure_type, members)


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
