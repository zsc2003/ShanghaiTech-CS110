# Exercise 2 : Matrix multiplication and Execution order

### 1. Why there is a gap between gb_v and gb_h ?

From the base test, we get that the performance of gb_h is better than gb_v. So we can conclude that the data is more continuous in the horizontal direction. So the data is more continuous in the horizontal direction, so the performance will be better even two more transpose operations are added.

### 2. Why the changed execution order will achieve a better performance even if we do more things(transpose)?

As the transpose operation is a fast enough operation, and as the question 1 answered, after the transpose operation, the data is more continuous, so the performance will be better even two more transpose operations are added.
