def maybe(val, default):
    return default if val is None else val


class Writer:
    def __init__(self, f):
        self.out = f
        self.level = 0
        self.in_progress = False

    def indent(self):
        self.level += 1
        if self.in_progress:
            self.writeln('')
        self.in_progress = False

    def unindent(self):
        self.level -= 1
        if self.in_progress:
            self.writeln('')
        self.in_progress = False

    def get_alignment(self):
        return '\t' * self.level if not self.in_progress else ''

    def writeln(self, s):
        self.out(self.get_alignment() + s + '\n')
        self.in_progress = False

    def write(self, s):
        self.out(self.get_alignment() + s)
        self.in_progress = True


class List:
    def __init__(self, *data):
        self.data = list(filter(lambda x: x is not None, data))

    def forward_declaration(self, writer: Writer):
        for data in self.data:
            data.forward_declaration(writer)

    def declaration(self, writer: Writer):
        for data in self.data:
            data.declaration(writer)

    def implementation(self, writer: Writer):
        for data in self.data:
            data.implementation(writer)

    def merge(self, other):
        self.data.extend(other.data)


class Enum:
    def __init__(self, name, vals=None):
        self.name: str = name
        self.vals: list[str] = maybe(vals, [])

    def forward_declaration(self, writer: Writer):
        writer.writeln(f'enum {self.name}')
        writer.writeln('{')
        writer.indent()
        for val in self.vals:
            writer.writeln(f'{val},')
        writer.writeln('MAX')
        writer.unindent()
        writer.writeln('};')

    def declaration(self, writer: Writer):
        pass

    def implementation(self, writer: Writer):
        pass


class Struct:
    def __init__(self, name, member_variables=None, member_functions=None, bases=None):
        self.name = name
        self.member_variables: list[VariableDeclaration] = maybe(member_variables, [])
        self.member_functions: list[MemberFunction] = maybe(member_functions, [])
        self.bases: list[str] = maybe(bases, [])

    def forward_declaration(self, writer: Writer):
        writer.writeln(f'struct {self.name};')

    def declaration(self, writer: Writer):
        bases = '' if not self.bases else ' : ' + ', '.join(self.bases)
        writer.writeln(f'struct {self.name}{bases}')
        writer.writeln('{')
        writer.indent()
        for member_variable in self.member_variables:
            member_variable.write(writer)
            writer.writeln(';')
        for member_function in self.member_functions:
            member_function.signature(writer)
            writer.writeln(';')
        writer.unindent()
        writer.writeln('};')

    def implementation(self, writer: Writer):
        for member_function in self.member_functions:
            member_function.signature(writer, namespace=self.name)
            writer.writeln(' {')
            writer.indent()
            if type(member_function.implementation) == str:
                for line in member_function.implementation.split('\n'):
                    writer.writeln(line)
            else:
                exit('Implementation not a string')
            writer.unindent()
            writer.writeln('};')
        pass


class MemberFunction:
    def __init__(self, name, implementation, return_type=None, arguments=None, inline=True, virtual=False,
                 default=False):
        self.name = name
        self.return_type = return_type
        self.arguments: list[VariableDeclaration] = arguments
        self.inline = inline
        self.implementation = implementation
        self.virtual = virtual
        self.default = default

    def signature(self, writer: Writer, namespace=None):
        if self.inline:
            writer.write('inline ')
        namespace = f'{namespace}::' if namespace else ''
        return_ = '' if self.return_type is None else f'{self.return_type} '
        writer.write(f'{return_}{namespace}{self.name}(')
        if self.arguments:
            self.arguments[0].write(writer, initialize=False)
            for argument in self.arguments[1:]:
                writer.write(', ')
                argument.write(writer, initialize=False)
        writer.write(')')

        pass


class VariableDeclaration:
    def __init__(self, name, mtype, val=None):
        self.member_name = name
        self.member_type = mtype
        self.val = val

    def write(self, writer: Writer, initialize=True):
        writer.write(f'{self.member_type} {self.member_name}')
        if self.val is not None and initialize:
            if self.val == '':
                writer.write('{}')
            else:
                writer.write(f'{{ {self.val} }}')


def test():
    pass


if __name__ == '__main__':
    name = 'Everything'

    member_variables = [
        VariableDeclaration(name='count', mtype='size_t', val=0)
    ]

    member_functions = []

    for (ftype, fname) in [('Object', 'object')]:
        member_functions.append(MemberFunction(
            name=name,
            return_type=f'{ftype}&',
            implementation=f'assert(signature(i).test(GAMEOBJECT_COMPONENT::{fname.upper()}));\nreturn data[i].{fname};'))

    struct = Struct(
        name='Everything',
        member_functions=member_functions,
        member_variables=member_variables
    )

    struct.forward_declaration(Writer(lambda x: print(x, end='')))
    struct.declaration(Writer(lambda x: print(x, end='')))
    struct.implementation(Writer(lambda x: print(x, end='')))
