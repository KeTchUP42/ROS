/**
 * @brief Computes the absolute value of an integer number.
 * The behavior is undefined if the result cannot be represented by the return type.
 */
int abs(int n)
{
    if (n < 0)
        return -n;
    return n;
}
