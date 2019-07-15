# Limits and Colimits
- In CT, everything is related to everything and can be viewed from many angles.
### Generalizing products
- 2 category
  - contains two objects.
  - No morphisms between them.
  - Only identity morphsisms.
- Selecting two objects in C is the same as defining a functor(D) from 2 to C.
- Selecting a candidate object c
  - A constant functor from 2 to C
    - Selection of c in C is done using the constant functor.
- Natural transformation between constant functor (to c) and D
  - NT component - 'c' to 'a' is a morphism - p
  - NT component - 'c' to 'b' is a morphism - q
```OCaml
val p1 = compose p m
val q1 = compose q m
```
