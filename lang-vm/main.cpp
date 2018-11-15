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
#include "program.hpp"


using namespace variant_talk;

namespace
{

void run(const Program& program)
{
  Interpreter interpreter;
  interpreter.run(program);
}

} // namespace

int main()
{
  using R = Register;

  Program myProgram{
    // i = 1;
    // while (i <= 10) {
    //   print(i);
    //   ++i;
    // }

    Load{R::r0, 1},                   //   LOAD r0, 1
    Load{R::r1, 10},                  //   LOAD r1, 10
                                      // loop:
    Compare{R::r0, R::r1},            //   CMP  r0, r1
    Jump{4, Jump::Condition::Greater},//   JGT  :done
    Print{R::r0},                     //   PRN  r0
    Inc{R::r0},                       //   INC  r0
    Jump{-4},                         //   JUMP :loop
                                      // done:
  };

  run(myProgram);
}

