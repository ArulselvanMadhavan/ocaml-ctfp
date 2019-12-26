# Algebra for monads
### Utilities used by code below
```ocaml
let compose f g x = f (g x)
```
### Introduction
- Endofunctors - defines expressions
- Algebras - Evaluate them
- Monads - Allows us to form and manipulate expressions
- Algebras + Monads
- Relation between monads and adjunctions
- Every adjunction defines a monad and a comonad
- Monad is an endofunctor m equipped with two natural transformations that satisfy some coherence conditions
- N_a : a -> m a
- U_a : m (m a) -> m a
- alg : m a -> a
- First coherence condition
  - alg compose N_a = id_a
- Second coherence condition
  - alg compose U_a = alg compose (m alg)
```OCaml
compose alg return = id
compose alg join = compose alg (fmap alg)
```
- Algebra for a list endofunctor
  - type a
  - function that produces a from [a]
- foldr can be used to express that algebra
```OCaml
val fold_right : ('a -> 'b -> 'b) -> 'a list -> 'b -> 'b
```
- List Algebra - foldr f z 
```OCaml
(* List module in the OCaml standard library accepts list before z *)
List.fold_right f [x] z = f x z
```
- List Algebra is compatible with the monad if
```OCaml
f x z = x
```
- z is the right unit
### T-algebras

