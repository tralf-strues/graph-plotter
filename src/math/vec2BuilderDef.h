VEC2_NAME add(const VEC2_NAME* vector1, const VEC2_NAME* vector2)
{
    assert(vector1);
    assert(vector2);

    return {vector1->x + vector2->x, vector1->y + vector2->y};
}

VEC2_NAME sub(const VEC2_NAME* vector1, const VEC2_NAME* vector2)
{
    assert(vector1);
    assert(vector2);

    return {vector1->x - vector2->x, vector1->y - vector2->y};
}

VEC2_NAME mul(const VEC2_NAME* vector, VEC2_TYPE scalar)
{
    assert(vector);

    return {vector->x * scalar, vector->y * scalar};
}

VEC2_TYPE len(const VEC2_NAME* vector)
{
    assert(vector);

    return sqrt(vector->x * vector->x + vector->y * vector->y);
}

VEC2_NAME rot(const VEC2_NAME* vector, float angle)
{
    assert(vector);

    float sina = sinf(angle);
    float cosa = cosf(angle);

    return {(VEC2_TYPE) (vector->x * cosa - vector->y * sina), 
            (VEC2_TYPE) (vector->x * sina + vector->y * cosa)};
}