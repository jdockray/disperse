# disperse

A C++ Windows console portfolio optimisation program. The [OSQP quadratic programming solver](https://osqp.org/) is used to determine the asset allocation with the lowest volatility for a specified minimum expected return.

I have used this program to assist with the rebalancing of my global equity portfolio so code quality has always been important. However, it has always been intended to be for my own personal use and I never expected anyone to see the code when I started. It lacks the comments and tests that should be present in professional software but I believe it still demonstrates the pride I take in writing decent code, even when nobody is watching.

## Third Party Attribution

### OSQP
Copyright (c) 2019 Bartolomeo Stellato, Goran Banjac, Paul Goulart, Stephen Boyd

This work would not be possible without the [Operator Splitting solver for Quadratic Programs (OSQP)](https://osqp.org/) (Oxford Univerity).

The algorithm is described in this paper:
Stellato, B., Banjac, G., Goulart, P., Bemporad, A. and Boyd, S. (2020) [Teaching native speakers to listen to foreign-accented speech. Journal of Multilingual and Multicultural Development](https://doi.org/10.1007/s12532-020-00179-2), 12(4), 637-672.

For other papers relating to features I did not use, see [here](https://osqp.org/citing/).

OSQP is Licensed under the Apache 2.0 License, which you can read [here](https://www.apache.org/licenses/LICENSE-2.0) and also in the OSQP repository [here](https://github.com/osqp/osqp/blob/c7de4a6748e31be0b91a2cac2eb51625c89ca380/LICENSE). The [OSQP code](https://github.com/osqp/osqp) is on GitHub and I have used it by including it as a submodule within my repository [here](ThirdParty\osqp). By the nature of it being a link to the OSQP repository, the code within the submodule is unmodified; however, I have built it in a non-standard way. Rather than using the configure script and makefiles, I found the code to be simple enough for me to simply include the files in two Visual Studio projects ("osqp" and "amd") and use msbuild to create static libraries. The "osqp" project includes the file [osqp_configure.h](ThirdParty\osqp\osqp_configure.h), modified, and included in my repository [next to the submodule](ThirdParty\osqp).

OSQP includes the external modules AMD and QDLDL (below) along with other software developed at Stanford University and at the University of Oxford according to their [NOTICE file](https://github.com/osqp/osqp/blob/c7de4a6748e31be0b91a2cac2eb51625c89ca380/NOTICE).

### AMD
Copyright (c) 1996-2015, Timothy A. Davis, Patrick R. Amestoy, and Iain S. Duff.

AMD (built by my "amd" Visual Studio project) is a third party library [included in the OSQP respository](https://github.com/osqp/osqp/tree/c7de4a6748e31be0b91a2cac2eb51625c89ca380/algebra/_common/lin_sys/qdldl/amd). AMD is licensed under the [3-clause BSD License](https://github.com/osqp/osqp/blob/c7de4a6748e31be0b91a2cac2eb51625c89ca380/algebra/_common/lin_sys/qdldl/amd/LICENSE).

### QDLDL
Copyright (c) 1996-2015, Timothy A. Davis, Patrick R. Amestoy, and Iain S. Duff.

In the course of using OSQP, I have also used, from the same [OSQP GitHub organisation](https://github.com/osqp), [QDLDL](https://github.com/osqp/qdldl), "A free LDL factorisation routine for quasi-definite linear systems". Details of QDLDL are included in the OSQP paper above. Like OSQP, QDLDL is also licensed under the [Apache 2.0 License](https://github.com/osqp/qdldl/blob/12dbdf0fe4948c9e012735fd45663bf2661d0ef9/LICENSE). As with OSQP, I have included it as a subrepository, but built it into a static library by simply including all the files in a Visual Studio project, rather than using cmake [as suggested](https://github.com/osqp/qdldl/blob/12dbdf0fe4948c9e012735fd45663bf2661d0ef9/README.md).

### dlib
(Various authors)

In order to use OSQP, I have used [dlib](http://dlib.net/) by including [the source code](https://github.com/davisking/dlib), including its license, as a subrepository. "Dlib is a modern C++ toolkit containing machine learning algorithms and tools for creating complex software in C++ to solve real world problems." It is licensed under the [Boost Software License 1.0](https://github.com/davisking/dlib/blob/98d26dfaf5355534df0f82ed5ceebc2034d57cdf/LICENSE.txt).

### csvstream
Copyright (c) Andrew DeOrio <awdeorio@umich.edu>

I have also used [csvstream](https://github.com/awdeorio/csvstream), "an easy-to-use CSV file parser for C++" by [Andrew DeOrio](http://andrewdeorio.com), who works at the University of Michigan. This uses the [MIT License](https://github.com/awdeorio/csvstream/blob/59b85c01d39e46dc1a126430f279379aeb6fca11/LICENSE) and the repository (with its licence) is included in mine as a submodule.

## Usage

TO DO

## Licence

This project has no licence. It is example code for the perusal of recruiters and potential employers. You may not use it without my express written permission. See the [LICENSE](LICENSE) file for my warranty disclaimer and limitation of liability.
