# Simple Algebraic Data Types
### Product Types
* Implemented using pairs.
* Not commutative.
* Commmutative upto isomorphism.
```ocaml
# let swap (a, b) = (b, a)
val swap : 'a * 'b -> 'b * 'a = <fun>
```
* Nesting pairs are isomorphic.(Pseudo OCaml)
```OCaml
('a * ('b * 'c))
```
* Types are different but elements are in one-to-one correspondence.
```ocaml
# let alpha = function | ((a, b), c) -> (a, (b, c))
val alpha : ('a * 'b) * 'c -> 'a * ('b * 'c) = <fun>
```
* Invertible function Alpha.
```ocaml
# let alpha_inv = function | (a, (b, c)) -> ((a, b), c)
val alpha_inv : 'a * ('b * 'c) -> ('a * 'b) * 'c = <fun>
```
* Unit type is the unit of the product
* Adding unit to a type 'a is isomorphic to 'a.
```OCaml
'a * unit
```
* Isomorphism example
```ocaml
# let rho = function | (a, ()) -> a
val rho : 'a * unit -> 'a = <fun>
```
```ocaml
# let rho_inv a = (a, ())
val rho_inv : 'a -> 'a * unit = <fun>
```
* _Set_ is a monoidal category. (A category that is also a monoid)
* Pair as ADT
```ocaml
type ('a, 'b) pair = P of 'a * 'b
```
* Example construction
```ocaml
# let stmt = P ("This statement is", false)
val stmt : (string, bool) pair = P ("This statement is", false)
```
* Type and Data constructors with same name. In Ocaml, data constructors start with an uppercase, though.
```ocaml
type ('a, 'b) pair = Pair of ('a * 'b)
```
* Pair and (,) are interchangeable.
```ocaml
# let stmt = ("This statement is", false)
val stmt : string * bool = ("This statement is", false)
```
* Named products.
```ocaml
type stmt = Stmt of string * int
```
