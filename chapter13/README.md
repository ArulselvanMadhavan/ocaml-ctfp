# Free Monoids
## Utilities used by the code below
```ocaml
module type Functor = sig 
  type 'a t 
  val fmap : ('a -> 'b) -> 'a t -> 'b t 
end
let compose f g x = f (g x)
```
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
```OCaml
2 * 3 = 6
List.append [2] [3] = [2; 3]
```
### Free monoid universal construction
```OCaml
let h (a * b) = h a * h b
```
- Homomorphism from lists of integers to integers
```OCaml
List.append [2] [3] = [2; 3]
```
- becomes multiplication
```ocaml
2 * 3 = 6
```
- Let p be the function that identifies the set of generators inside the X-ray image of monoid m.
```ocaml
module type FreeMonoidRep = functor (F : Functor) -> sig
  type x
  type m
  val p : x -> m F.t
end
```
- Similar function on a different monoid n can be
```ocaml
module type FreeMonoidRep = functor (F : Functor) -> sig
  type x
  type n
  val q : x -> n F.t
end
```
- Homomorphism between monoids
```OCaml
val h : m -> n
```
- Building q through p
```OCaml
val q = compose uh p
```
