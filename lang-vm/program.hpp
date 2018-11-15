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

#pragma once

#include <variant>
#include <vector>


namespace variant_talk
{

enum class Register
{
  r0,
  r1,
  r2,
  r3,
};

constexpr auto NUM_REGISTERS = 4;


struct Inc { Register reg; };
struct Dec { Register reg; };

struct Load
{
  Register target;
  int32_t value;
};

struct Print { Register reg; };

struct Compare
{
  Register leftOperand;
  Register rightOperand;
};


struct Jump
{
  enum class Condition
  {
    None,
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
  };

  explicit Jump(int32_t offset_, Condition condition_ = Condition::None)
    : offset(offset_)
    , condition(condition_)
  {
  }

  int32_t offset;
  Condition condition;
};


using OpCode = std::variant<
  Inc,
  Dec,
  Load,
  Compare,
  Jump,
  Print
>;

using Program = std::vector<OpCode>;

} // namespace variant_talk
