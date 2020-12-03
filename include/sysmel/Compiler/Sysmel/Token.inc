TOKEN_NAME(Error)
TOKEN_NAME(Float) // [+\-]?[0-9]+ \. [0-9]+ ([eE] [\+\-]? [0-9]+)? ;
TOKEN_NAME(Integer) // [+\-]?[0-9]+ (r [0-9A-Za-z]+)?;
TOKEN_NAME(Keyword) // ([_A-Za-z][_A-Za-z0-9]*\:)+;
TOKEN_NAME(Identifier) // [_A-Za-z][_A-Za-z0-9]* ;
TOKEN_NAME(String) // \"" ([^\\\""] | (\\.) )*\"";
TOKEN_NAME(Character) // \' ([^\\\'] | (\\.) )*\';
TOKEN_NAME(SymbolKeyword) // \#([_A-Za-z][_A-Za-z0-9]*\:)+ ;
TOKEN_NAME(SymbolIdentifier) // \#([_A-Za-z][_A-Za-z0-9]*)+ ;
TOKEN_NAME(SymbolOperator) // \#[\+\-/\\\*~<>=@,%|&?!^]+ ;
TOKEN_NAME(SymbolString) // \# \"" ([^\\\""] | (\\.) )*\"";
TOKEN_NAME(LowPrecedenceBinaryOperator) // \:\: [\+\-/\\\*~<>=@,%|&?!^]+;
TOKEN_NAME(GenericBinaryOperator) // [\+\-/\\\*~<>=@,%|&?!^]+;

TOKEN_NAME(LogicalOr) // ||
TOKEN_NAME(LogicalAnd) // &&
TOKEN_NAME(BitwiseOr) // |
TOKEN_NAME(BitwiseXor) // ^
TOKEN_NAME(BitwiseAnd) // &
TOKEN_NAME(Equality) // =
TOKEN_NAME(IdentityEquality) // ==
TOKEN_NAME(NotEquality) // ~=
TOKEN_NAME(IdentityNotEquality) // ~=
TOKEN_NAME(LessOrEqualThan) // <=
TOKEN_NAME(LessThan) // <
TOKEN_NAME(GreaterOrEqualThan) // >=
TOKEN_NAME(GreaterThan) // >
TOKEN_NAME(LogicalShiftLeft) // <<
TOKEN_NAME(LogicalShiftRight) // >>
TOKEN_NAME(Plus) // +
TOKEN_NAME(Minus) // -
TOKEN_NAME(Times) // *
TOKEN_NAME(Divide) // /
TOKEN_NAME(IntegerDivide) // //
TOKEN_NAME(Modulus) // %
TOKEN_NAME(EndOfSource)
