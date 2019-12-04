# F-Algebras
## Utilities used by code below
```ocaml
```
### Introduction
- Monoid - set, single object category, object in monoidal category
- F Algebra
```ocaml
module type FAlgebra = sig
  type 'a f (* parameterized type *)
  type 'a algebra = 'a f -> 'a
end
```
- MonoidF functor
```ocaml
type 'a monF = MEmpty | Mappend of ('a * 'a)
```
- RingF functor
```ocaml
type 'a ringF = RZero 
| ROne 
| RAdd of ('a * 'a) 
| RMul of ('a * 'a) 
| RNeg of 'a
```
- evalZ function
```ocaml
module EvalZ : FAlgebra = struct
  type 'a f = 'a ringF
  type 'a algebra = 'a f -> 'a
  let evalZ : 'int algebra = function
  | RZero -> 0
  | ROne -> 1
  | RAdd (m, n) -> m + n
  | RMul (m, n) -> m * n
  | RNeg n -> -n
end
```
