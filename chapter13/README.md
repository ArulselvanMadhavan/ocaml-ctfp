# Free Monoids
- Categories - strongly typed languages
- Monoids - untyped languages
- Monoid - Category with a single object, where all logic is encoded in rules of morphism composition.
- Categorical model of monoid is fully equivalent to set theoretical definition of monoid.
- Generators of the free monoid - basic set of elements needed to generate free monoid
- Free construction - keep generating all possible combination of elements, and perform the minimum number of identifications - just enough to uphold the laws
### Free Monoid in OCaml
```ocaml
module type Monoid = sig
  type m
  val mempty : m
  val mappend : m -> m -> m
end
```
- Monoid instance for list
```ocaml
 module ListMonoid(T1 : sig type a end) : (Monoid with type m = T1.a list) = struct
  type m = T1.a list
  let mempty = []
  let mappend xs ys = List.append xs ys
end;;
```
```ocaml
2 * 3 = 6
List.append [2] [3] = [2; 3]
```
### Free monoid universal construction
