#pragma once

#if !defined(__bool_true_false_are_defined) && __STDC_VERSION__ < 202311L
typedef _Bool bool;
#define true 1
#define false 0
#define __bool_true_false_are_defined 1
#endif
