# Exercise 4: 1000 $1 Bills

I hand you a thousand \$1 bills and ten envelopes. Your job is to find a way to put various numbers of dollar bills in those ten envelopes so that no matter what amount of money I ask you for (between $1-1000), you can simply hand me some combination of envelopes and always be assured of giving me the correct amount of cash.

Solution:

As there is 1000 bills and ten envelopes, we can have such distributions:

| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10|
| - | - | - | - | - | - | - | - | - | - |
| 1 | 2 | 4 | 8 | 16| 32| 64|128|256|489|

As for money less than 512, we can use the first 9 envelopes to implement the needed money, as for money equal or bigger than 512, we can implement it by firstly minus 489, then we can firstly implement the result as it is less than 512, then we take the 10th envelopes and we will get the correct money. 