# Toy CPU — ISA Specification

**Version:** Draft 0.2  
**Project:** From the Transistor to the Web Browser  
**Module:** 5 — Instruction Set Architecture Design  
**Status:** Audited against available project notes; not yet frozen

## 1. Purpose and scope

This specification defines the software-visible behavior of our toy CPU. It is the contract for two later implementations:

1. **Module 6 — Assembler:** Converts assembly source into 32-bit machine instructions.
2. **Module 7 — CPU:** Fetches, decodes, and executes those instructions.

The ISA must define instruction encodings, operands, registers, flags, memory effects, control flow, and faults precisely enough that the assembler and CPU can be implemented independently.

### Decision-status labels

- **DECIDED:** Explicitly established in the project discussions.
- **PROVISIONAL:** A design choice has been recorded but is not fully frozen.
- **PENDING:** More detail or a decision is required before implementation.

The roadmap defines the completion criterion as a sufficiently precise ISA specification, not simply an agreed list of instructions.

---

## 2. Architectural state

### 2.1 General-purpose registers

**Status: DECIDED**

The CPU has 16 general-purpose registers:

`R0–R15`

Each register is 32 bits wide. Register identifiers require four bits.

Registers hold operands, intermediate values, results, and memory addresses.

The individual roles of these registers remain to be defined.

### 2.2 Program Counter (PC)

**Status: DECIDED conceptually; details PENDING**

The PC identifies the address of the instruction being fetched.

For fixed-width 32-bit instructions, normal sequential execution advances by four bytes:

`PC_next = PC_current + 4`

A taken branch changes the next instruction address to the calculated branch target. An untaken conditional branch continues sequentially.

Reset is separate from branching.

Pending:
- Reset/boot address.
- Exact relationship between the PC value used during execution and the value used in branch calculations.
- Whether PC is accessible through the general-purpose register interface.
- PC behavior following a fault.

### 2.3 Stack Pointer (SP)

**Status: DECIDED conceptually; details PENDING**

The stack is a region of ordinary memory. The SP tracks its position according to the chosen stack convention.

Nested function calls require return information to be preserved so each function can return to its caller.

Pending:
- Which register represents SP.
- Whether SP is a dedicated register or one of `R0–R15`.
- Stack growth direction.
- Stack alignment.
- Return-address storage.
- Whether dedicated CALL/RET or push/pop instructions are included.

### 2.4 Condition flags

**Status: DECIDED conceptually; exact formulas PENDING**

The CPU maintains four flags:

| Flag | Meaning |
|---|---|
| Z | Zero |
| N | Negative/sign bit |
| C | Carry or the defined unsigned-arithmetic carry/borrow indicator |
| V | Signed overflow |

For a 32-bit result:

- `Z = 1` when the result is zero.
- `N = result[31]`.

C and V must be defined separately for each relevant operation. In particular, the subtraction convention for C must be explicit.

The flags support conditional branches.

---

## 3. Instruction representation

### 3.1 Instruction width

**Status: DECIDED**

Every instruction is exactly 32 bits (four bytes).

Normal sequential execution advances the PC by four bytes.

### 3.2 Opcode width

**Status: DECIDED**

The opcode occupies the most-significant eight bits:

```text
31       24 23                    0
+----------+-----------------------+
| opcode 8 |     operand fields    |
+----------+-----------------------+
```

This provides 256 possible opcode values.

### 3.3 Instruction formats

**Status: PROVISIONAL**

The remaining 24 bits have different meanings depending on the instruction format.

#### R-format — register operands

```text
31       24 23   20 19   16 15   12 11          0
+----------+-------+-------+-------+--------------+
| opcode 8 | Rd 4  | Rn 4  | Rm 4  | reserved 12 |
+----------+-------+-------+-------+--------------+
```

Operand convention:

- `Rd`: destination register.
- `Rn`: first source register.
- `Rm`: second source register.

Example:

```asm
ADD R0, R1, R2
```

Conceptually:

`R0 = R1 + R2`

The 12 reserved bits must have a defined decoding rule, preferably requiring zero in the initial ISA.

#### I-format — immediate operands

```text
31       24 23   20 19   16 15                 0
+----------+-------+-------+--------------------+
| opcode 8 | Rd 4  | Rn 4  | immediate 16      |
+----------+-------+-------+--------------------+
```

Example:

```asm
ADDI R0, R1, #100
```

Conceptually:

`R0 = R1 + 100`

The immediate field is 16 bits wide. Its signedness and valid range must be specified for each instruction that uses it.

**Encoding issue to resolve:** The opcode must unambiguously identify the instruction format. The current opcode assignments do not distinguish register and immediate forms of ADD, SUB, or the logical instructions. A recommended solution is to allocate separate opcodes to register and immediate variants.

#### B-format — branches

```text
31       24 23                              0
+----------+---------------------------------+
| opcode 8 | branch displacement 24         |
+----------+---------------------------------+
```

Branches use a PC-relative displacement rather than storing a complete 32-bit target address.

Pending:
- Displacement base.
- Signedness and scaling.
- Valid range.
- Alignment requirements.
- Behavior when a target is invalid or outside the address space.

#### M-format — memory operations

```text
31       24 23   20 19   16 15 14 13        0
+----------+-------+-------+-----+------------+
| opcode 8 | Rd 4  | Rn 4  | W 2 | offset 14  |
+----------+-------+-------+-----+------------+
```

- `Rd`: data register.
- `Rn`: base-address register.
- `W`: access-width field.
- `offset14`: signed byte displacement.

Effective address:

`effective_address = Rn_value + sign_extend(offset14)`

The offset range is −8192 through +8191 bytes.

---

## 4. Opcode allocation

### 4.1 Opcode-space organization

**Status: PROVISIONAL**

| Range | Instruction class |
|---|---|
| `0x00–0x1F` | Arithmetic |
| `0x20–0x3F` | Logical operations |
| `0x40–0x5F` | Load/store |
| `0x60–0x7F` | Compare/branch |
| `0x80–0x9F` | Shifts |
| `0xA0–0xFF` | Reserved/future |

These ranges are project design choices, not requirements imposed by the roadmap.

### 4.2 Recorded opcode assignments

**Status: PROVISIONAL**

#### Arithmetic

| Opcode | Instruction | Behavior |
|---|---|---|
| `0x00` | ADD | Add; preserve flags |
| `0x01` | ADDS | Add; update flags |
| `0x02` | SUB | Subtract; preserve flags |
| `0x03` | SUBS | Subtract; update flags |

Register and immediate forms are intended to be supported. Their encoding mechanism is not finalized.

#### Logical operations

| Opcode | Instruction | Behavior |
|---|---|---|
| `0x20` | AND | Bitwise AND; preserve flags |
| `0x21` | ANDS | Bitwise AND; update flags |
| `0x22` | ORR | Bitwise OR; preserve flags |
| `0x23` | ORRS | Bitwise OR; update flags |
| `0x24` | EOR | Bitwise XOR; preserve flags |
| `0x25` | EORS | Bitwise XOR; update flags |
| `0x26` | BIC | Bit clear; preserve flags |
| `0x27` | BICS | Bit clear; update flags |

BIC computes:

`Rd = Rn AND NOT(operand2)`

Register and immediate forms are intended to be supported.

#### Load/store

| Opcode | Instruction |
|---|---|
| `0x40` | LOAD |
| `0x41` | STORE |

The access width is encoded within the instruction.

#### Compare, branches, and shifts

The following instructions have been discussed, but their exact opcode assignments have not been frozen:

```text
CMP

B
BEQ
BNE
BLT
BGE
BGT
BLE
BCC
BCS
BHI
BLS

LSL
LSR
ASR
```

`MOV` has appeared in examples, but its opcode and complete semantics remain unresolved.

---

## 5. Arithmetic and logical semantics

### 5.1 Arithmetic

**Status: DECIDED conceptually**

```text
ADD:
    Rd = Rn + operand2
    flags unchanged

ADDS:
    Rd = Rn + operand2
    update Z, N, C, V

SUB:
    Rd = Rn - operand2
    flags unchanged

SUBS:
    Rd = Rn - operand2
    update Z, N, C, V
```

Results are 32 bits wide. The final specification must define carry-out, borrow convention, and signed overflow formulas.

### 5.2 Logical operations

**Status: DECIDED conceptually**

```text
AND: Rd = Rn AND operand2
ORR: Rd = Rn OR operand2
EOR: Rd = Rn XOR operand2
BIC: Rd = Rn AND NOT(operand2)
```

Non-S variants preserve flags. S variants update flags.

The exact behavior of C and V for flag-setting logical instructions remains pending. Z and N should be defined from the result.

---

## 6. Compare and condition flags

### 6.1 CMP

**Status: DECIDED conceptually**

CMP subtracts its operands to update flags but does not store the subtraction result.

```text
temporary = Rn - operand2
update flags from subtraction
do not write a general-purpose destination register
```

Example:

```text
R0 = 7
R1 = 10

CMP R0, R1
```

The 32-bit result is `0xFFFFFFFD`, representing −3 in two's complement.

Therefore:

```text
Z = 0
N = 1
```

C and V depend on the final subtraction flag definitions.

Pending:
- Opcode assignment.
- Register and immediate forms.
- Operand-order confirmation.
- Exact flag formulas.

---

## 7. Branch semantics

### 7.1 Conditional branch conditions

**Status: DECIDED conceptually**

#### Unconditional and equality

| Instruction | Condition |
|---|---|
| B | Always |
| BEQ | `Z == 1` |
| BNE | `Z == 0` |

#### Signed comparisons

Define:

`S = N XOR V`

| Instruction | Condition |
|---|---|
| BLT | `S == 1` |
| BGE | `S == 0` |
| BGT | `Z == 0 AND S == 0` |
| BLE | `Z == 1 OR S == 1` |

#### Unsigned comparisons

| Instruction | Condition |
|---|---|
| BCC | `C == 0` |
| BCS | `C == 1` |
| BHI | `C == 1 AND Z == 0` |
| BLS | `C == 0 OR Z == 1` |

These conditions rely on the flag definitions established by arithmetic or CMP.

### 7.2 Branch target calculation

**Status: PROVISIONAL**

Branches use a signed PC-relative displacement.

The exact target calculation must specify:
- Whether the displacement is relative to the current instruction or the next instruction.
- Whether the displacement is in bytes or instruction units.
- The displacement range.
- Required target alignment.
- Behavior for invalid targets and address overflow.

---

## 8. Shift instructions

**Status: DECIDED conceptually; encoding PENDING**

### LSL — Logical Shift Left

Zeros enter from the least-significant side.

```text
00000011 LSL #2
= 00001100
```

### LSR — Logical Shift Right

Zeros enter from the most-significant side.

```text
00001100 LSR #2
= 00000011
```

### ASR — Arithmetic Shift Right

The sign bit is replicated during a right shift.

```text
11111000 ASR #2
= 11111110
```

The intended flag-setting behavior is:

- Z: updated from the result.
- N: updated from the result.
- C: last bit shifted out.
- V: unchanged.

Pending:
- Exact opcode assignments.
- Whether flag-setting variants use separate opcodes.
- Whether shift amounts are immediate, register-specified, or both.
- Valid shift amounts.
- Behavior for zero or oversized shifts, especially C.

---

## 9. Memory model

### 9.1 Addressing and endianness

**Status: DECIDED**

Memory is byte-addressable. Each address identifies one byte.

The CPU uses little-endian representation for multi-byte data.

Example:

```text
Value: 0x12345678

Address     Byte
0x1000      0x78
0x1001      0x56
0x1002      0x34
0x1003      0x12
```

Endianness affects the order of bytes in memory, not the numerical value stored in a register.

### 9.2 Access widths

**Status: DECIDED in the later Module 5 design**

Supported access widths:

| W | Width |
|---|---|
| `00` | 8-bit |
| `01` | 16-bit |
| `10` | 32-bit |
| `11` | Reserved |

Earlier discussions mentioned only 8-bit and 32-bit accesses. The later Module 5 design explicitly added 16-bit accesses, so this specification uses the later decision.

### 9.3 Effective address

**Status: DECIDED conceptually**

For LOAD and STORE:

`effective_address = base_register_value + sign_extend(offset14)`

The signed byte displacement range is −8192 through +8191.

Example syntax:

```asm
LOAD  R0, [R1 + 20]
STORE R0, [R1 - 8]
```

The exact assembly grammar remains provisional.

### 9.4 Alignment

**Status: DECIDED**

| Width | Alignment |
|---|---|
| 8-bit | Any address |
| 16-bit | Address divisible by 2 |
| 32-bit | Address divisible by 4 |

Misaligned accesses produce an alignment fault/exception.

Examples:

```text
8-bit access at 0x1001   → valid
16-bit access at 0x1002  → valid
16-bit access at 0x1001  → alignment fault
32-bit access at 0x1004  → valid
32-bit access at 0x1002  → alignment fault
```

The general exception mechanism remains pending.

### 9.5 LOAD

**Status: PROVISIONAL**

Conceptually:

```text
address = Rn_value + sign_extend(offset14)
value = memory[address, width]
Rd = value extended to 32 bits
```

For an 8- or 16-bit load, two plausible policies remain:

- Zero extension: upper bits become zero.
- Sign extension: upper bits replicate the loaded value's sign bit.

The project notes explicitly leave this unresolved. The final ISA may choose one policy or provide separate signed and unsigned load variants.

### 9.6 STORE

**Status: DECIDED conceptually**

Conceptually:

```text
address = Rn_value + sign_extend(offset14)
memory[address, width] = low width bits of Rd
```

The implementation must write only the selected number of bytes in little-endian order.

Fault behavior for invalid memory addresses remains pending.

---

## 10. Invalid instructions and faults

**Status: PENDING**

The ISA must define deterministic behavior for:

- Undefined opcodes.
- Reserved width encodings.
- Nonzero reserved bits.
- Invalid branch targets.
- Misaligned memory accesses.
- Invalid or inaccessible memory addresses.

Misaligned accesses are already specified to cause an alignment fault. The broader fault mechanism is not finalized.

The final specification must define:
- Whether the CPU halts, raises an exception, or uses another mechanism.
- Whether the faulting instruction modifies registers or flags.
- Whether memory writes can partially occur before a fault.
- How execution resumes, if resumption is supported.

---

## 11. Calling convention

**Status: PENDING**

The stack occupies ordinary memory and is tracked by SP. Nested calls must preserve return information.

The calling convention must define:

- Argument registers.
- Return-value registers.
- Caller-saved registers.
- Callee-saved registers.
- Return-address handling.
- Function-call and return instructions, if supported.
- Stack growth direction.
- Stack alignment.
- Reserved architectural registers.

This is a software convention that allows separately written functions to interoperate. It is also a prerequisite for later compiler-related work.

---

## 12. Assembly syntax

**Status: PROVISIONAL**

Examples used in the project discussions include:

```asm
ADD  R0, R1, R2
ADDS R0, R1, #5

CMP  R0, R1
BEQ  equal

LOAD  R0, [R1 + 20]
STORE R0, [R1 - 8]
```

General conventions discussed:

- Register names are `R0` through `R15`.
- Immediate operands begin with `#`.
- Memory operands identify a base register and signed displacement.
- Labels identify branch destinations.

The lexer and parser still need a formal grammar for punctuation, signed numbers, numeric bases, labels, comments, and case sensitivity.

The opcode and operand-format ambiguity must be resolved before the assembler's encoder is finalized.

---

## 13. Audit findings and remaining work

The available project notes were reviewed for the ISA design, Module 5 status, assembler direction, and roadmap. This review identified the following important points.

### Corrections and clarifications

1. **Register/immediate encoding:** The intended support for both forms is recorded, but the encoding must distinguish them unambiguously. This is a major implementation blocker.
2. **Memory widths:** Use the later decision of 8-, 16-, and 32-bit accesses, not the earlier 8-/32-bit-only design.
3. **Narrow loads:** Zero versus sign extension is still unresolved.
4. **Opcode assignments:** CMP, branches, shifts, and MOV do not yet have a complete final opcode table.
5. **PC and branch behavior:** Sequential `PC + 4` is established, but branch target calculation is not.
6. **Flags:** The conceptual branch conditions are established, but the exact C/V rules need to be made precise.
7. **Calling convention and faults:** These remain unfinished rather than implicitly decided.

### Remaining checklist

#### A. Freeze instruction encodings

- [ ] Finalize all instruction formats.
- [ ] Separate or otherwise distinguish register and immediate forms.
- [ ] Finalize opcode assignments for every supported instruction.
- [ ] Define reserved bits and encodings.
- [ ] Ensure every supported instruction has unambiguous decoding.

#### B. Finalize semantics

- [ ] Define immediate signedness and ranges.
- [ ] Define arithmetic carry, borrow, and overflow formulas.
- [ ] Define C/V behavior for logical instructions.
- [ ] Finalize CMP semantics.
- [ ] Finalize shift amount and flag behavior.
- [ ] Finalize branch target calculation and range.
- [ ] Decide whether MOV is included and define its semantics.

#### C. Finalize memory and faults

- [ ] Decide narrow-load extension behavior.
- [ ] Define exact STORE byte-write behavior.
- [ ] Define invalid-address behavior.
- [ ] Define alignment-fault handling.
- [ ] Define invalid-instruction behavior.

#### D. Finalize architectural behavior

- [ ] Define reset/boot PC.
- [ ] Define PC behavior around branch execution.
- [ ] Decide PC's relationship to the general-purpose registers.
- [ ] Define fault effects on architectural state.

#### E. Finalize calling convention

- [ ] Assign register roles.
- [ ] Define argument and result registers.
- [ ] Define caller/callee preservation rules.
- [ ] Define return-address handling.
- [ ] Define stack growth and alignment.

#### F. Validate

- [ ] Write small assembly programs for arithmetic, flags, branches, shifts, and memory.
- [ ] Manually verify instruction fields and expected execution results.
- [ ] Confirm every format totals exactly 32 bits.
- [ ] Publish a final opcode and semantics table.

---

## 14. Completion criterion

Module 5 is complete when the specification is precise enough for the assembler and CPU to be implemented independently without inventing missing rules.

```text
Module 5 — ISA specification
          ↓
Module 6 — Assembler
          ↓
Module 7 — CPU implementation
          ↓
Machine-code execution and integration tests
```

The core conceptual foundation is established: fixed-width instructions, a register file, arithmetic and logical operations, flags, conditional branches, shifts, and byte-addressable little-endian memory.

**This remains a draft, not a frozen machine-code contract.** The unresolved encoding and semantic rules must be settled before treating it as the definitive specification.
