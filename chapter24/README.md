# F-Algebras
## Utilities used by code below
```ocaml
module Algebra : Algebra = functor (F : sig type 'a f end) -> struct
  type 'a algebra = 'a F.f -> 'a
end
module Algebra : Algebra = functor (F : sig type 'a f end) -> struct
  type 'a algebra = 'a F.f -> 'a
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
