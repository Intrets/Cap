class Writer:
    def __init__(self, f):
        self.out = f
        self.level = 0
        self.in_progress = False

    def indent(self):
        self.level += 1
        self.in_progress = False

    def unindent(self):
        self.level -= 1
        self.in_progress = False

    def get_alignment(self):
        return '\t' * self.level if not self.in_progress else ''

    def writeln(self, s):
        self.out(self.get_alignment() + s + '\n')
        self.in_progress = False

    def write(self, s):
        self.out(self.get_alignment() + s)
        self.in_progress = True


class Struct:
    def __init__(self, name, member_variables, member_functions):
        self.name = name
        self.member_variables: VariableDeclaration = member_variables
        self.member_functions = member_functions

    def write(self, writer: Writer):
        writer.writeln(f'struct {self.name}')
        writer.writeln('{')
        writer.indent()
        for member_variable in self.member_variables:
            member_variable.write(writer)
            writer.writeln(';')
        writer.writeln(f'stuff')
        writer.unindent()
        writer.writeln('};')


class MemberFunction:
    def __init__(self, name, return_type, arguments=None, inline=True):
        self.name = name
        self.return_type = return_type
        self.arguments = arguments
        self.inline = inline


class VariableDeclaration:
    def __init__(self, name, mtype, val=None):
        self.member_name = name
        self.member_type = mtype
        self.val = val

    def write(self, writer: Writer):
        writer.write(f'{self.member_type} {self.member_name}')
        if self.val:
            writer.write(f'{{ {self.val} }}')

def test():
    pass


if __name__ == '__main__':
    struct = Struct(
        name='StructName',
        member_functions=None,
        member_variables=[
            VariableDeclaration(name='test', mtype='std::vector<Object>', val=1),
            VariableDeclaration(name='test', mtype='std::vector<Object>', val=1),
        ]
    )

    struct.write(Writer(lambda x: print(x, end='')))
