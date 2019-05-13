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
## Records
* Problem in working with unnamed tuples
```ocaml
# let starts_with_symbol = function | (name, symbol, _) -> String.is_prefix name ~prefix:symbol
val starts_with_symbol : string * string * 'a -> bool = <fun>
```
* Element as a Record
```ocaml
type element = 
  { name: string;
    symbol: string;
    atomic_number: int;
  }
```
* The two representations are isomorphic.
```ocaml
# let tuple_to_elem = function | (n, s, a) -> {name = n; symbol=s; atomic_number=a;}
val tuple_to_elem : string * string * int -> element = <fun>
```
```ocaml
# let elem_to_tuple = function | {name; symbol; atomic_number} -> (name, symbol, atomic_number)
val elem_to_tuple : element -> string * string * int = <fun>
```
```ocaml
# let atomic_number = function | {atomic_number} -> atomic_number
val atomic_number : element -> int = <fun>
```
* Using record syntax
```ocaml
# let starts_with_symbol = function | {name;symbol;_;} -> String.is_prefix name ~prefix:symbol
val starts_with_symbol : element -> bool = <fun>
```
* OCaml only allows special characters in the infix operator.
```ocaml
# let starts_with_symbol = function | {name;symbol;_;} -> String.is_prefix name ~prefix:symbol
val starts_with_symbol : element -> bool = <fun>
```
