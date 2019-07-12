# Function Types
### Utilities
```ocaml
(* Utilities needed for the rest of the scripts in the document to run. *)
type ('a, 'b) either = Left of 'a | Right of 'b
```
- Function type is a set of morphisms between two sets.
- A set of morphisms between any two objects in a category is called a hom-set.
- In the category *Set*, every hom-set is itself an object in the same category, because it's also a set.
- In other categories, hom-set is external to a category - external hom-sets
- Internal hom-set - an object constructed in a category that represent the hom-sets.
### Universal Construction
- Constructing an internal hom-set(function type) from scratch.
- A pattern that involves three objects - function type we are constructing, argument type and result type.
- Function application/evaluation connects these three types.
- Pick an object z and a.
- Product of them z x a is an object.
- Arrow g from that object to b.
- g maps every such object(z x a) to b.
- *There is no function type, if there is no product type*
### Currying
- Curring is built into the syntax of OCaml as well.(Pseudo OCaml)
```OCaml
'a -> ('b -> 'c)
```
- Unparenthesized signature
```OCaml
'a -> 'b -> 'c
```
- Multi argument functions
```ocaml
# let catstr s s' -> StringLabels.concat ~sep:"" [s;s']
val catstr : string -> string -> string = <fun>
```
- Same function written using one argument functions
```ocaml
# let catstr = fun s -> fun s' -> StringLabels.concat ~sep:"" [s;s']
val catstr : string -> string -> string = <fun>
```
- Greet
```ocaml
# let greet = catstr "Hello"
val greet : string -> string = <fun>
```
- A Function of two variables
```OCaml
'a * 'b -> 'a
```
- curry
```ocaml
# let curry f a b = f (a, b)
val curry : ('a * 'b -> 'c) -> 'a -> 'b -> 'c = <fun>
```
- uncurry
```ocaml
# let uncurry f p = f (fst p) (snd p)
val uncurry : ('a -> 'b -> 'c) -> 'a * 'b -> 'c = <fun>
```
- Factorizer
```ocaml
# let factorizer g = fun a -> (fun b -> g (a, b))
val factorizer : ('a * 'b -> 'c) -> 'a -> 'b -> 'c = <fun>
```
### Exponentials
- Function object or internal hom object between a and b, is often called the exponential denoted by b^a
### Cartesian Closed Categories
- Larger family of categories called *Cartesian closed*
- *Set* is just one example of such a category.
- Cartesian closed category must contain
  - The terminal object
  - A product of any pair of objects
  - An exponential for any pair of objects.
- Terminal object is a product of zero arity.
- CCC provides models for the simply typed lambda calculus.
- Dual of product and terminal object is the coproduct and initial object.
- *Bicartesian closed category* - CCC that also supports coproduct and initial object and in which product can be distributed over coproduct
### Exponentials and ADT
- Function types as exponentials fits very well into the scheme of ADTs.
- Algebra vs BiCartesian Closed Category
  - Zero -> Initial object
  - One  -> Terminal object
  - product -> product
  - sums -> coproduct
  - exponential -> exponentials (internal hom object)
- Singleton set is the terminal object in *Set*
- Exponentials of sums
```ocaml
module type Exponential_Of_Sums_Example = sig
  val f : (int, float) either -> string
end
```
- Implementation
```ocaml
module Exp_Sum_Impl : Exponential_Of_Sums_Example = struct
  let f = function 
  | Left n -> if n < 0 then "Negative int" else "Positive int"
  | Right x -> if Float.compare x 0.4 < 0 then "Negative double" else "Positive double"  
end
```
### Curry-Howard Isomorphism
- Correspondence between logic and ADT
- Void - false
- unit -> true
- Product -> AND
- Sum type -> OR
- Function type -> logical implication
- Every type can be interpreted as a proposition.
- Writing programs is same as proving theorems.
- eval example (pseudo OCaml)
```OCaml
val eval : (('a -> 'b), 'a) -> 'b 
```
- eval implementation
```ocaml
# let eval (f,a) = f a
val eval : ('a -> 'b) * 'a -> 'b = <fun>
```
- Mapping a V b => a to types (pseudo OCaml)
```ocaml
('a, 'b) either -> 'a
```
- Absurd (pseudo OCaml)
```ocaml
val absurd : void -> 'a
```
- Absurd implementation
```ocaml
# type void
type void
# let rec absurd (a : void) = absurd a
val absurd : void -> 'a = <fun>
```
