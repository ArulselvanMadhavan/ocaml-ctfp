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
(('a * 'b) * 'c)
```
```OCaml
('a * ('b * 'c))
```
* Types are different but elements are in one-to-one correspondence.
```ocaml
# let alpha ((a, b), c) = (a, (b, c))
val alpha : ('a * 'b) * 'c -> 'a * ('b * 'c) = <fun>
```
* Invertible function Alpha.
```ocaml
# let alpha_inv (a, (b, c)) = ((a, b), c)
val alpha_inv : 'a * ('b * 'c) -> ('a * 'b) * 'c = <fun>
```
* Unit type is the unit of the product
* Adding unit to a type 'a is isomorphic to 'a.
```OCaml
'a * unit
```
* Isomorphism example
```ocaml
# let rho (a, ()) = a
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
# let starts_with_symbol (name, symbol, _) = String.is_prefix name ~prefix:symbol
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
# let tuple_to_elem (name, symbol, atomic_number) = {name; symbol; atomic_number}
val tuple_to_elem : string * string * int -> element = <fun>
```
```ocaml
# let elem_to_tuple {name; symbol; atomic_number} = (name, symbol, atomic_number)
val elem_to_tuple : element -> string * string * int = <fun>
```
```ocaml
# let atomic_number {atomic_number} = atomic_number
val atomic_number : element -> int = <fun>
```
* Using record syntax
```ocaml
# let starts_with_symbol {name;symbol;_} = String.is_prefix name ~prefix:symbol
val starts_with_symbol : element -> bool = <fun>
```
* Infix application only available for special characters
```ocaml
(* OCaml only allows special characters in the infix operator. So, the above function name cannot be applied be infix. *)
```
## Sum Types
* Either type (Similar to OCaml's builtin Result type)
```ocaml
type ('a, 'b) either = Left of 'a | Right of 'b
```
* Sum Types are commutative upto isomorphism.
```ocaml
type ('a, 'b, 'c) one_of_three = Sinistrial of 'a | Medial of 'b | Dextral of 'c
```
* *Set* is a symmetric monoidal category with respect to coproduct.
  * Role of the binary operation is played by the disjoint sum(Either).
  * Role of the initial operation is played by the initial object(Void).
* Pseudo OCaml  
```OCaml
'a void either
```
* Sum type example
```ocaml
type color = Red | Green | Blue
```
* Even simpler example
```ocaml
type bool = True | False
```
* Maybe type (Similar to OCaml's builtin Option type)
```ocaml
type 'a maybe = Nothing | Just of 'a
```
* Nothing type
```ocaml
type nothing_type = Nothing
```
* Just type
```ocaml
type 'a just_type = Just of 'a
```
* Maybe using Either
```ocaml
type 'a maybe = (unit, 'a) either
```
* List type
```ocaml
type 'a list = Nil | Cons of 'a * 'a list
```
* Maybe Tail
```ocaml
type 'a maybe = Nothing | Just of 'a
let maybe_tail = function | Nil -> Nothing | Cons (_, xs) -> Just xs
```
## Algebra of Types
* Pseudo Ocaml representation of types
```OCaml
'a * ('b, 'c) either
```
```OCaml
('a * 'b, 'c * 'd) either
```
* Isomorphic example
```ocaml
# let prod_to_sum (x, e) = match e with
                           | Left y -> Left (x, y)
                           | Right z -> Right (x, z)
val prod_to_sum : 'a * ('b, 'c) either -> ('a * 'b, 'a * 'c) either = <fun>
```
```ocaml
# let sum_to_prod = function | Left (x, y) -> (x, Left y) | Right (x, z) -> (x, Right z)
val sum_to_prod : ('a * 'b, 'a * 'c) either -> 'a * ('b, 'c) either = <fun>
```
* Sample value
```ocaml
# let prod1 = (2, Left "Hi!")
val prod1 : int * (string, 'a) either = (2, Left "Hi!")
```
* Defining list again
```ocaml
type 'a list = Nil | Cons of 'a * 'a list
```
