// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 7: Some Assembly Required ---

This year, Santa brought little Bobby Tables a set of regs and bitwise
logic gates! Unfortunately, little Bobby is a little under the recommended
age range, and he needs help assembling the circuit.

Each reg has an identifier (some lowercase letters) and can carry a 16-bit
signal (a number from 0 to 65535). A signal is provided to each reg by a
gate, another reg, or some specific value. Each reg can only get a signal
from one source, but can provide its signal to multiple destinations. A
gate provides no signal until all of its inputs have a signal.

The included instructions booklet describes how to connect the parts
together: x AND y -> z means to connect regs x and y to an AND gate, and
then connect its output to reg z.

For example:
    - 123 -> x means that the signal 123 is provided to reg x.
    - x AND y -> z means that the bitwise AND of reg x and reg y is
      provided to reg z.
    - p LSHIFT 2 -> q means that the value from reg p is left-shifted by 2
      and then provided to reg q.
    - NOT e -> f means that the bitwise complement of the value from reg e
      is provided to reg f.

Other possible gates include OR (bitwise OR) and RSHIFT (right-shift). If,
for some reason, you'd like to emulate the circuit instead, almost all
programming languages (for example, C, JavaScript, or Python) provide
operators for these gates.

For example, here is a simple circuit:
    123 -> x
    456 -> y
    x AND y -> d
    x OR y -> e
    x LSHIFT 2 -> f
    y RSHIFT 2 -> g
    NOT x -> h
    NOT y -> i

After it is run, these are the signals on the regs:
    d: 72
    e: 507
    f: 492
    g: 114
    h: 65412
    i: 65079
    x: 123
    y: 456

In little Bobby's kit's instructions booklet (provided as your puzzle
input), what signal is ultimately provided to reg a?


--- Part Two ---

Now, take the signal you got on wire a, override wire b to that signal, and
reset the other wires (including wire a). What new signal is ultimately
provided to wire a?
*/

typedef enum {
      OPR_INVALID
    , OPR_ASSIGN
    , OPR_AND
    , OPR_OR
    , OPR_NOT
    , OPR_LSHIFT
    , OPR_RSHIFT
} Operator;

typedef enum {
      TTP_INVALID
    , TTP_OPERATOR
    , TTP_OPERAND
} TokenType;

typedef enum {
      VLT_INVALID
    , VLT_REGISTER
    , VLT_IMMEDIATE
} ValueType;

typedef char Register[3];

typedef struct Value {
    ValueType type;

    union {
        Register reg;
        u16 immediate;
    };
} Value;

typedef struct Command {
    Value left_operand;
    Value right_operand;
    Operator operator;
    Register destiny;
} Command;

typedef struct CommandList {
    Command command;
    struct CommandList *next;
} CommandList;

typedef struct Token {
    TokenType type;

    union {
        Operator operator;
        Value operand;
    };
} Token;

#define SIGNAL_BUFFER_SIZE 26

#define SIGNAL_INVALID  (1ul << 31)
#define SIGNAL_OVERRIDE (1ul << 30)

#define TOKEN_DELIMITER " "

u32 SingleSignal[SIGNAL_BUFFER_SIZE];
u32 DoubleSignal[SIGNAL_BUFFER_SIZE * SIGNAL_BUFFER_SIZE];

static const Command InvalidCommand = {
    .left_operand = {.type = VLT_INVALID, {{0}}},
    .right_operand = {.type = VLT_INVALID, {{0}}},
    .operator = OPR_INVALID,
    .destiny = {0}
};

static const Slice OperatorAssign = { "->", 2 };
static const Slice OperatorAnd = { "AND", 3 };
static const Slice OperatorOr = { "OR", 2 };
static const Slice OperatorNot = { "NOT", 3 };
static const Slice OperatorLShift = { "LSHIFT", 6 };
static const Slice OperatorRShift = { "RSHIFT", 6 };

static bool
Signal_IsValid(const Register reg)
{
    return
        (reg[1] == '\0' && reg[0] - 'a' < SIGNAL_BUFFER_SIZE) ||
        (reg[0] - 'a' < SIGNAL_BUFFER_SIZE && reg[1] - 'a' < SIGNAL_BUFFER_SIZE);
}

static void
Signal_Reset(void)
{
    for (int row = 0; row < SIGNAL_BUFFER_SIZE; ++row) {
        for (int col = 0; col < SIGNAL_BUFFER_SIZE; ++col) {
            if (row == 0) {
                SingleSignal[col] = SIGNAL_INVALID;
            }

            DoubleSignal[row * SIGNAL_BUFFER_SIZE + col] = SIGNAL_INVALID;
        }
    }
}

static u32 *
Signal_Address(const Register reg)
{
    if (reg == NULL || *reg == '\0') {
        GOTO(address_error);
    } else if (!Signal_IsValid(reg)) {
        GOTO(address_error);
    }

    if (reg[1] == '\0') {
        u8 index = (u8) reg[0] - 'a';

        return &SingleSignal[index];
    } else {
        u8 signal_row = (u8) reg[0] - 'a';
        u8 signal_col = (u8) reg[1] - 'a';

        return &DoubleSignal[signal_row * SIGNAL_BUFFER_SIZE + signal_col];
    }

address_error:
    Quit(5, "%s: invalid address '%s' in %s at %s:%lu.", NAME, reg, DebugFile, DebugFunction, DebugLine);
}

static void
Signal_Override(const Register reg)
{
    if (!Signal_IsValid(reg)) {
        Quit(4, "%s: can't override signal '%s'\n", NAME, reg);
    }

    *Signal_Address(reg) |= SIGNAL_OVERRIDE;
}

static u16
Signal_Load(const Register reg)
{
    return (u16) *Signal_Address(reg);
}

static void
Signal_Store(const Register reg, u16 value)
{
    if (*Signal_Address(reg) & SIGNAL_OVERRIDE) {
        return;
    }

    *Signal_Address(reg) = value;
}

static Token
Token_Parse(Slice token)
{
    Token result = {0};

    if (Slice_Equals(OperatorAssign, token)) {
        result.type = TTP_OPERATOR;
        result.operator = OPR_ASSIGN;
    } else if (Slice_Equals(OperatorAnd, token)) {
        result.type = TTP_OPERATOR;
        result.operator = OPR_AND;
    } else if (Slice_Equals(OperatorOr, token)) {
        result.type = TTP_OPERATOR;
        result.operator = OPR_OR;
    } else if (Slice_Equals(OperatorNot, token)) {
        result.type = TTP_OPERATOR;
        result.operator = OPR_NOT;
    } else if (Slice_Equals(OperatorLShift, token)) {
        result.type = TTP_OPERATOR;
        result.operator = OPR_LSHIFT;
    } else if (Slice_Equals(OperatorRShift, token)) {
        result.type = TTP_OPERATOR;
        result.operator = OPR_RSHIFT;
    } else if (
        (token.data[0] >= 'a' && token.data[1] <= 'z') &&
        (!isdigit(token.data[1]) || (token.data[1] >= 'a' && token.data[1] <= 'z' && !isdigit(token.data[2])))
    ) {
        result.type = TTP_OPERAND;
        result.operand.type = VLT_REGISTER;
        strncpy(result.operand.reg, token.data, token.size);
    } else {
        char *end = NULL;

        u16 immediate = (u16) strtoul(token.data, &end, 10);

        if (errno != ERANGE || end != NULL) {
            result.type = TTP_OPERAND;
            result.operand.type = VLT_IMMEDIATE;
            result.operand.immediate = immediate;
        }
    }

    return result;
}

static bool
Operator_IsUnary(Operator operator)
{
    return
        operator == OPR_ASSIGN ||
        operator == OPR_NOT;
}

static Command
Command_Parse(const Slice slice)
{
    if (
        slice.data == NULL ||
        *slice.data == '\0' ||
        slice.size == 0
    ) {
        GOTO(parse_error);
    }

    Command result = InvalidCommand;

    Slice command = slice;
    Slice token_string = Slice_Token(&command, TOKEN_DELIMITER);

    if (token_string.size == 0) {
        GOTO(parse_error);
    }

    // # 1st token
    Token left_token = Token_Parse(token_string);

    if (
        left_token.type == TTP_OPERATOR &&
        Operator_IsUnary(left_token.operator) &&
        left_token.operator != OPR_ASSIGN)
    {
        result.operator = left_token.operator;
    } else if (left_token.type == TTP_OPERAND) {
        result.left_operand = left_token.operand;
    } else {
        GOTO(parse_error);
    }

    // #2nd token
    token_string = Slice_Token(&command, TOKEN_DELIMITER);

    if (token_string.size == 0) {
        GOTO(parse_error);
    }

    Token middle_token = Token_Parse(token_string);

    if (left_token.type == TTP_OPERAND && middle_token.type == TTP_OPERATOR) {
        result.operator = middle_token.operator;
    } else if (left_token.type == TTP_OPERATOR && middle_token.type == TTP_OPERAND) {
        result.right_operand = middle_token.operand;
    } else {
        GOTO(parse_error);
    }

    // #3rd token
    token_string = Slice_Token(&command, TOKEN_DELIMITER);

    if (token_string.size == 0) {
        GOTO(parse_error);
    }

    Token right_token = Token_Parse(token_string);

    // Test for assign operations, ie.: [src] -> [dst]
    if (
        left_token.type == TTP_OPERAND &&
        middle_token.type == TTP_OPERATOR &&
        middle_token.operator == OPR_ASSIGN &&
        right_token.type == TTP_OPERAND &&
        right_token.operand.type == VLT_REGISTER
    ) {
        strcpy(result.destiny, right_token.operand.reg);

        return result;
    } else if (
        left_token.type == TTP_OPERAND &&
        middle_token.type == TTP_OPERATOR &&
        right_token.type == TTP_OPERAND
    ) {
        result.right_operand = right_token.operand;
    }

    // #4rd token
    token_string = Slice_Token(&command, TOKEN_DELIMITER);

    if (token_string.size == 0) {
        GOTO(parse_error);
    }

    Token assign_token = Token_Parse(token_string);

    // Test for unary operations, ie.: NOT [opr] -> [dst]
    if (
        (left_token.type == TTP_OPERATOR && Operator_IsUnary(left_token.operator)) &&
        (middle_token.type == TTP_OPERAND) &&
        (right_token.type == TTP_OPERATOR && right_token.operator == OPR_ASSIGN) &&
        (assign_token.type == TTP_OPERAND && assign_token.operand.type == VLT_REGISTER)
    ) {
        strcpy(result.destiny, assign_token.operand.reg);

        return result;
    }

    if (assign_token.type != TTP_OPERATOR || assign_token.operator != OPR_ASSIGN) {
        GOTO(parse_error);
    }

    // #5th token
    token_string = Slice_Token(&command, TOKEN_DELIMITER);

    if (token_string.size == 0) {
        GOTO(parse_error);
    }

    Token destiny_token = Token_Parse(token_string);

    if (destiny_token.type != TTP_OPERAND || destiny_token.operand.type != VLT_REGISTER) {
        GOTO(parse_error);
    }

    strcpy(result.destiny, destiny_token.operand.reg);

    return result;

parse_error:
    Quit(3, "%s: invalid input '%s' in %s at %s:%lu.", NAME, slice.data, DebugFile, DebugFunction, DebugLine);
}

static bool
Command_IsExecutable(Command command)
{
    bool left_operand =
        (command.left_operand.type == VLT_IMMEDIATE) ||
        (command.left_operand.type == VLT_REGISTER && ((*Signal_Address(command.left_operand.reg) & SIGNAL_INVALID) == 0)) ||
        (command.left_operand.type == VLT_INVALID && Operator_IsUnary(command.operator));

    bool right_operand =
        (command.right_operand.type == VLT_IMMEDIATE) ||
        (command.right_operand.type == VLT_REGISTER && ((*Signal_Address(command.right_operand.reg) & SIGNAL_INVALID) == 0)) ||
        (command.right_operand.type == VLT_INVALID && command.operator == OPR_ASSIGN);

    bool operator = command.operator != OPR_INVALID;

    bool destiny = Signal_IsValid(command.destiny);

    return left_operand && right_operand && operator && destiny;
}

static void
Command_Execute(Command command)
{
    u16 lhs;

    if (command.left_operand.type == VLT_REGISTER) {
        lhs = Signal_Load(command.left_operand.reg);
    } else if (command.left_operand.type == VLT_IMMEDIATE) {
        lhs = command.left_operand.immediate;
    } else {
        lhs = 0;
    }

    if (command.operator == OPR_ASSIGN) {
        Signal_Store(command.destiny, lhs);

        return;
    }

    u16 rhs;

    if (command.right_operand.type == VLT_REGISTER) {
        rhs = Signal_Load(command.right_operand.reg);
    } else if (command.right_operand.type == VLT_IMMEDIATE) {
        rhs = command.right_operand.immediate;
    } else {
        rhs = 0;
    }

    if (command.operator == OPR_AND) {
        Signal_Store(command.destiny, lhs & rhs);
    } else if (command.operator == OPR_OR) {
        Signal_Store(command.destiny, lhs | rhs);
    } else if (command.operator == OPR_NOT) {
        Signal_Store(command.destiny, ~rhs);
    } else if (command.operator == OPR_LSHIFT) {
        Signal_Store(command.destiny, lhs << rhs);
    } else if (command.operator == OPR_RSHIFT) {
        Signal_Store(command.destiny, lhs >> rhs);
    }
}

static void
CommandList_Insert(CommandList **list, Command command)
{
    CommandList *item = malloc(sizeof(struct CommandList));

    item->command = command;
    item->next = *list;

    *list = item;
}

static u32
CommandList_Execute(CommandList **list)
{
    u32 pending = 0;

    if (list == NULL || *list == NULL) {
        return pending;
    }

    CommandList *item = *list;
    CommandList *parent = NULL;

    while (item != NULL) {
        if (Command_IsExecutable(item->command)) {
            CommandList *current = item;

            item = current->next;

            if (parent) {
                parent->next = item;
            } else {
                *list = item;
            }

            Command_Execute(current->command);

            free(current);
        } else {
            pending++;

            parent = item;
            item = item->next;
        }
    }

    return pending;
}

static void
Program_Run(Slice program)
{
    CommandList *list = NULL;

    while (1) {
        Slice line = Slice_ReadLine(&program);

        if (line.data == NULL) {
            break;
        } else if (line.size == 0) {
            continue;
        }

        CommandList_Execute(&list);

        Command command = Command_Parse(line);

        if (Command_IsExecutable(command)) {
            Command_Execute(command);
        } else {
            CommandList_Insert(&list, command);
        }
    }

    u32 pending_current = CommandList_Execute(&list);
    u32 pending_last = pending_current;

    while (list != NULL) {
        pending_current = CommandList_Execute(&list);

        if (pending_current == pending_last) {
            Quit(2, "%s: not all command could be run (%s).", NAME, __func__);
        }
    }
}

void
Part_One(Slice input)
{
    Program_Run(input);

    printf("Part one: wire a value -> %u\n", Signal_Load("a\0"));
}

void
Part_Two(Slice input)
{
    Program_Run(input);

    printf("Part two: wire a value -> %u\n", Signal_Load("a\0"));
}

int
main(void)
{
    Slice input = Slice_ReadStdIn();

    if (input.data == NULL || *input.data == '\0') {
        Quit(1, "%s: empty input in %s at %s:%lu.", NAME, __FILE__, __func__, __LINE__);
    }

    Signal_Reset();

    Part_One(input);

    u16 signal_a = Signal_Load("a\0");

    Signal_Reset();

    Signal_Store("b\0", signal_a);

    Signal_Override("b\0");

    Part_Two(input);

    return 0;
}

