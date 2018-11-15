/* Copyright (C) 2018, Nikolai Wuttke. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "interpreter.hpp"

#include "match.hpp"

#include <cassert>
#include <iostream>


namespace variant_talk
{

Interpreter::Interpreter()
  : mRegisters{}
{
}


void Interpreter::run(const Program& program)
{
  mInstructionPointer = 0;

  const auto numInstructions = static_cast<int>(program.size());

  while (mInstructionPointer < numInstructions)
  {
    const auto savedInstructionPointer = mInstructionPointer;

    interpretOpCode(program[static_cast<std::size_t>(mInstructionPointer)]);

    // If the operation didn't touch the instruction pointer, it was a "normal"
    // operation, so we go to the next instruction.
    // Otherwise, it was a jump, which means we are already at the right index
    // for the next instruction, and mustn't change the instruction pointer.
    if (savedInstructionPointer == mInstructionPointer)
    {
      ++mInstructionPointer;
    }
  }
}


void Interpreter::interpretOpCode(const OpCode& opCode)
{
  match(opCode,
    [this](const Inc& op)
    {
      ++getReg(op.reg);
    },

    [this](const Dec& op)
    {
      --getReg(op.reg);
    },

    [this](const Load& op)
    {
      getReg(op.target) = op.value;
    },

    [this](const Print& op)
    {
      std::cout << getReg(op.reg) << '\n';
    },

    [this](const Compare& op)
    {
      mLastComparisonResult = getReg(op.leftOperand) - getReg(op.rightOperand);
    },

    [this](const Jump& op)
    {
      if (conditionFulfilled(op.condition)) {
        mInstructionPointer += op.offset;
      }
    }
  );
}


int32_t& Interpreter::getReg(const Register r)
{
  return mRegisters[static_cast<size_t>(r)];
}


bool Interpreter::conditionFulfilled(const Jump::Condition condition)
{
  using C = Jump::Condition;

  switch (condition)
  {
    case C::None: return true;
    case C::Less: return mLastComparisonResult < 0;
    case C::LessOrEqual: return mLastComparisonResult <= 0;
    case C::Greater: return mLastComparisonResult > 0;
    case C::GreaterOrEqual: return mLastComparisonResult >= 0;
    case C::Equal: return mLastComparisonResult == 0;
    case C::NotEqual: return mLastComparisonResult != 0;
  }

  assert(false);
  return false;
}

} // namespace variant_talk
