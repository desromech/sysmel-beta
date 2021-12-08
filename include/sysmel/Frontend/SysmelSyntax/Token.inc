TOKEN_NAME(Error)
TOKEN_NAME(ErrorUnexpected)
TOKEN_NAME(ErrorIncompleteComment)
TOKEN_NAME(ErrorIncompleteCharacter)
TOKEN_NAME(ErrorIncompleteString)
TOKEN_NAME(ErrorIncompleteSymbolString)
TOKEN_NAME(ErrorCount)

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

TOKEN_NAME(LogicalNot) // !
TOKEN_NAME(LogicalOr) // ||
TOKEN_NAME(LogicalAnd) // &&
TOKEN_NAME(BitwiseNot) // ~
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

TOKEN_NAME(Colon) // :
TOKEN_NAME(ColonColon) // ::
TOKEN_NAME(Assignment) // :=

TOKEN_NAME(Dot) // .
TOKEN_NAME(Comma) // ,
TOKEN_NAME(Semicolon) // ;
TOKEN_NAME(LeftParent) // ())
TOKEN_NAME(RightParent) // )
TOKEN_NAME(LeftBracket) // [
TOKEN_NAME(RightBracket) // ]
TOKEN_NAME(LeftCurlyBracket) // {
TOKEN_NAME(RightCurlyBracket) // }

TOKEN_NAME(LiteralArrayLeftParent) // #(
TOKEN_NAME(ByteArrayLeftBracket) // #[
TOKEN_NAME(DictionaryLeftBracket) // #{

TOKEN_NAME(Quote) // `'
TOKEN_NAME(QuasiQuote) // ``
TOKEN_NAME(QuasiUnquote) // `,
TOKEN_NAME(Splice) // `@

TOKEN_NAME(EndOfSource)
