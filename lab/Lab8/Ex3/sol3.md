# Exercise 3: Effort of Cache Miss

### Modfication

There is a theroem in struct called cache alignment. As int type under linux x64 is 4 bytes and long type is 8 bytes, so we can first define 2 int and then 1 long type to make sure the 2 int are in the same cache line. Then as for the long array char, we define them as the last, so that they will noe effected the cache line of the previous 2 int and the long type.

