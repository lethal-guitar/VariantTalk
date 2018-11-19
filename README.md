# Example code for my talk "std::variant and the Power of Pattern Matching"

This repository contains the example code shown in my presentation "std::variant and the power of pattern matching". To build, a C++ 17 compatible compiler is required. I have tested it on the following compilers:

- Microsoft Visual Studio 2017 (version 15.8.9)
- clang 7.0.0 (with libc++)

In addition, the library SFML is required. I tested versions 2.4 and 2.5.
On Mac OS, you can install it via homebrew:

```
brew install sfml
```

You can also find the slides for my presentation in the `slides` directory.

All the code in this repository is shared under the MIT license (see `LICENSE`).

## Licenses for graphics/fonts

There are some non-code assets in the `state-machine/resources` directory. Except
for the following exceptions, these files are made by me and covered by a [Creative Commons "CC by" license](https://creativecommons.org/licenses/by/4.0/).

The following files are Copyright Ascender (see `Apache-License.txt`):

* `DroidSans.ttf`
* `DroidSans-Bold.ttf`

The following file is Copyright ESA, NASA, and L. Calcada (ESO for STScI) (see `NASA-License.txt`):

* `space-bg.jpg` [source](http://hubblesite.org/image/2432/news_release/2008-39)
