# F-Algebras
## Utilities used by code below
```ocaml
module Algebra : Algebra = functor (F : sig type 'a f end) -> struct
  type 'a algebra = 'a F.f -> 'a
end
module Algebra : Algebra = functor (F : sig type 'a f end) -> struct
  type 'a algebra = 'a F.f -> 'a
end
module type Functor = sig
  type 'a t
  val fmap : ('a -> 'b) -> 'a t -> 'b t
end
```
### Introduction
- Monoid - set, single object category, object in monoidal category
- F Algebra
```ocaml
module type Algebra = functor(F : sig type 'a f end) -> sig
  type 'a algebra = 'a F.f -> 'a
end
```
- MonoidF functor
```ocaml
type 'a mon_f = MEmpty | Mappend of ('a * 'a)
```
- RingF functor
```ocaml
type 'a ring_f = RZero 
| ROne 
| RAdd of ('a * 'a) 
| RMul of ('a * 'a) 
| RNeg of 'a
```
- evalZ function
```ocaml
module Ring = struct

  module RingAlg = Algebra(struct type 'a f = 'a ring_f end)

  let eval_z : 'a RingAlg.algebra = function
    | RZero -> 0
    | ROne  -> 1
    | RAdd (m, n) -> m + n
    | RMul (m, n) -> m * n
    | RNeg n -> -n
end
```
- Recursion
```ocaml
type expr =
    RZero
  | ROne
  | RAdd of (expr * expr)
  | RMul of (expr * expr)
  | RNeg of expr
```
- Ring Evaluator with a recursive definition
```ocaml
  let rec eval_z : expr -> int = function
    | RZero -> 0
    | ROne  -> 1
    | RAdd (e1, e2) -> eval_z e1 + eval_z e2
    | RMul (e1, e2) -> eval_z e1 * eval_z e2
    | RNeg e -> -(eval_z e)
end
```
- Depth-one tree
```ocaml
type 'a ring_f1 = ('a ring_f) ring_f
```
- Depth two tree
```ocaml
type 'a ring_f2 = (('a ring_f) ring_f) ring_f
```
- D2 via D1
```ocaml
type 'a ring_f2 = 'a ring_f ring_f1
```
- Applying an endofunctor infinitely many times produces a fixed point
```ocaml
type 'a fix = Fix of ('a -> 'a fix)
```
- Constructor name - Fix is a convention
```ocaml
type 'a fix = In of ('a -> 'a fix)
```
- Fix as a function
```ocaml
let fix_const : 'a. ('a -> 'a fix) -> 'a fix = fun f -> Fix f
```
- unfix
```ocaml
let unfix : 'a.'a fix -> ('a -> 'a fix) = fun (Fix f) -> f
```
### Category of F-Algebras
- F Algebras form a category
- carrier object : a
- morphism : f : F a -> a
- Objects in that category are a pair (a, f)
- Morphisms in the F-algebra category : (a, f) -> (b, g)
  - m: a -> b
- Homomorphism of F-algebras
  - F m : F a -> F b
  - g : F b -> b
  - g compose F m = m compose f
- Initial Algebra
  - carrier i
  - j :: F i -> i
- Lambek's theorem : j is an isomorphism
- There is a unique homomorphism m from initial object to any other F-algebra
- j : F i -> i ; m : j -> i ; F m : F i -> F a ; f : F a -> a
- A new algebra
  - Carrier : F i
  - morphism : F j : F (F i) -> F i
- (i, j) is the initial algebra. Unique homomorphism 'm' must connect initial algebra (i, j) with (F i, F j)
- j : F i -> i ; m : i -> F i ; F m : F i -> F (F i); F j : F (F i) -> F i
- A new algebra 
  - F j : F (F i) -> F i
  - j : F i -> i
- j is a homomorphism of algebras (F i, F j) to (i, j)
- j compose m is a homomorphism of two algebras (i, j) and (F i, F j)
  - i compose m = id_i
- m compose j = id_Fi
- i is the inverse of m and m is the inverse of j
- j is an isomorphism between F i and i
- j is the constructor Fix
- i is the Fix f
- m is the inverse unFix
### Natural Numbers
- zero : 1 -> N ; succ : N -> N
- 1 + N -> N
- As functor
```ocaml
type 'a nat_f = ZeroF | SuccF of 'a;;
```
- Fixed point (Initial algebra)
```ocaml
type nat = Zero | Succ of nat
```
- Peano representation for natural numbers
### Catamorphisms
- Initial algebra - Fix f
- Evaluator is the constructor Fix
- Unique morphism m : Initial algebra to any other algebra
- Algebra : carrier a and evaluator is alg
- Fix : f (Fix f) -> Fix f
- m : Fix f -> a
- fmap m (f (Fix f)) -> f a
- alg : f a -> a
- m is the evaluator of the fixed point
- m evaluates the whole expression tree
- m = alg . fmap m . unfix
- cata in OCaml
```ocaml
module Cata(F : Functor) = struct
  type 'a fix = Fix of (('a fix) F.t)
  let fix : 'a. ('a fix) F.t -> 'a fix = fun f -> Fix f
  let unfix : 'a. 'a fix -> 'a fix F.t = fun (Fix f) -> f
  let rec cata : 'a. ('a F.t -> 'a) -> 'a fix -> 'a = fun alg fixf -> alg (F.fmap (cata alg) (unfix fixf))
end
```
- Functor for natural numbers
```ocaml
type 'a nat_f = ZeroF | SuccF of 'a
```
- Carrier Type : (int, int)
- Algebra 
```ocaml
let rec fib = function 
  | ZeroF -> (1, 1)
  | SuccF (m, n) -> (n, m + n)
```
- Algebra for NatF defines the recurrence relation and the catmorphism just evaluates the n-th element of that sequence
